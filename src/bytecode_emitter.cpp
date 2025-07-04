#include "bytecode_emitter.h"
#include "ast_utils.h"

#include <iostream>

BytecodeEmitter::BytecodeEmitter()
{
	this->implicitCastCtx = nullptr;
	this->shortCircuitCtx = false;
}

void BytecodeEmitter::generateBytecode(std::vector<ASTNode*>& ast)
{
	// emit all top level code
	for (ASTNode* node : ast)
		node->accept(*this);


	// emit a call to main followed by a HALT instruction after top level code. Everything below this point are functions, which should not
	// be executed directly they must be executed via a CALL instruction
	// bytecode layout looks like -> [Top level statements ... ] -> [call main] -> [HALT] -> [function declarations]
	emitCallToMain();
	emit(Opcode::HALT);

	// go back and generate bytecode for all deferred functions
	for (ASTFuncDecl* func : deferredFunctions)
		resolveDeferredFunction(func);

	for (DeferredCall& call : deferredCalls)
		resolveDeferredCall(call);

	functionTable.dumpTable();
}

void BytecodeEmitter::emitCallToMain()
{
	VMFunctionEntry* main = functionTable.lookupFunctionByName("main");
	emit(Opcode::CALL);
	emit16(main->funcIndex);

	// right now, main has 0 args, but eventually command line args will be added and this will have to change
	emit8(0);
}

void BytecodeEmitter::emitDefaultValue(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8: 
			emit(Opcode::PUSH_I8);
			emit8(0);
			break;
		case TypeKind::i16:
			emit(Opcode::PUSH_I16);
			emit16(0);
			break;
		case TypeKind::i32:
			emit(Opcode::PUSH_I32);
			emit32(0);
			break;
		case TypeKind::i64:
			emit(Opcode::PUSH_I64);
			emit64(0);
			break;
		case TypeKind::u8:
			emit(Opcode::PUSH_U8);
			emit8(0);
			break;
		case TypeKind::u16:
			emit(Opcode::PUSH_U16);
			emit16(0);
			break;
		case TypeKind::u32:
			emit(Opcode::PUSH_U32);
			emit32(0);
			break;
		case TypeKind::u64:
			emit(Opcode::PUSH_U64);
			emit64(0);
			break;
		case TypeKind::f32:
			emit(Opcode::PUSH_F32);
			emit32(0);
			break;
		case TypeKind::f64:
			emit(Opcode::PUSH_F64);
			emit64(0);
			break;
		case TypeKind::Char:
			emit(Opcode::PUSH_CHAR);
			emit8(0);
			break;
		case TypeKind::Bool:
			emit(Opcode::PUSH_BOOL);
			emit8(0);
			break;
	}
}

void BytecodeEmitter::insert16(uint16_t value, size_t offset)
{
	bytecode[offset] = (value & 0xFF);
	bytecode[offset + 1] = ((value >> 8) & 0xFF);
}

void BytecodeEmitter::insert8(uint8_t value, size_t offset)
{
	bytecode[offset] = value;
}

void BytecodeEmitter::patchLabelJumps(std::vector<uint16_t>& patchSites, uint16_t address)
{
	// only have to call patchLabelJumps when we dont know what address we're going to be jumping to.
	// E.G. when jumping over true branch to false branch, we dont know the address of the false branch
	// until after we generate code for the true branch so we have to come back and patch in those jump addresses
	for (size_t i = 0; i < patchSites.size(); i++)
		insert16(address, static_cast<uint16_t>(patchSites[i]));
}

void BytecodeEmitter::resolveDeferredFunction(ASTFuncDecl* function)
{
	// this is guaranteed to be in the function table at this point, no need to check against nullptr
	VMFunctionEntry* entry = functionTable.lookupFunctionByName(std::get<std::string>(function->funcIdentifier.value));
	entry->startAddress = bytecode.size();
	std::cout << "resolving deferred function for " << entry->name << std::endl;

	function->body->accept(*this);
}

void BytecodeEmitter::resolveDeferredCall(DeferredCall& deferredCall)
{
	// need to patch in the calls func index and arg count operands
	// bytecode looks like [..., CALL, 0, 0, 0, ...]
	// where the first two 0's are dummy values for the funcIndex, and the last 0 is a dummy value for the arg count
	
	VMFunctionEntry* function = functionTable.lookupFunctionByName(deferredCall.call->callee->typeInfo->name);
	std::cout << "resolving deferred call for function " 
		<< function->name 
		<< " func index = " 
		<< function->funcIndex 
		<< " arg count = " 
		<< function->argCount 
		<< " start address = "
		<< function->startAddress
		<< std::endl;
	insert16(function->funcIndex, deferredCall.callAddress);
	insert8(function->argCount, deferredCall.callAddress + 2);
}

void BytecodeEmitter::patchLabel(JumpLabel& label)
{
	for (size_t i = 0; i < label.jumpPatches.size(); i++)
		insert16(label.jumpAddress, label.jumpPatches[i]);
}

bool BytecodeEmitter::tryOverwriteLastJump(Opcode newJump)
{
	// 2 bytes 2 bytes  curr
	// JMP = [bytecode.size() - 4, bytecode.size() - 3], ADDRESS = [bytecode.size() - 2, bytecode.size() - 1]
	// [JMP,   ADDRESS, PC]

	// make sure there's enough bytecode, if not, return false
	if (bytecode.size() < 4)
		return false;

	// extract the low and high bits of the opcode and cast it to the Opcode enum
	uint8_t opcodeLow = bytecode[bytecode.size() - 4];
	uint8_t opcodeHigh = bytecode[bytecode.size() - 3];
	uint16_t combined = (static_cast<uint16_t>(opcodeHigh) << 8) | static_cast<uint16_t>(opcodeLow);
	Opcode opcode = static_cast<Opcode>(combined);

	// if opcode is not a JMP instruction, don't overwrite
	if (opcode != Opcode::JMP_IF_FALSE && opcode != Opcode::JMP_IF_TRUE)
		return false;

	// at this point, we know its a jmp instruction, so overwrite it and patch a jump in at the next two bytes after the JMP instruction
	insert16(static_cast<uint16_t>(newJump), bytecode.size() - 4);
	conditionCtxStack.back().falseJump.jumpPatches.push_back(static_cast<uint16_t>(bytecode.size() - 2));
	return true;
}

void BytecodeEmitter::emitShortCircuitOr(ASTLogical& node, JumpLabel* trueTarget, JumpLabel* falseTarget)
{
	JumpLabel rhsLabel;

	// if lhs is ASTLogical node
	ASTExpr* lhs = ASTUtils::unwrapGroupExpr(node.lhs);
	if (lhs->shortCircuitable)
	{
		ASTLogical& lhsExpr = static_cast<ASTLogical&>(*lhs);

		if (lhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(lhsExpr, trueTarget, &rhsLabel);
		else
			emitShortCircuitAnd(lhsExpr, &rhsLabel, falseTarget);
	}
	else
	{
		node.lhs->accept(*this);

		emit(Opcode::JMP_IF_TRUE);
		trueTarget->jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
		emit16(0);
	}

	rhsLabel.jumpAddress = (static_cast<uint16_t>(bytecode.size()));
	patchLabel(rhsLabel);

	ASTExpr* rhs = ASTUtils::unwrapGroupExpr(node.rhs);
	if (rhs->shortCircuitable)
	{
		ASTLogical& rhsExpr = static_cast<ASTLogical&>(*rhs);

		if (rhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(rhsExpr, trueTarget, falseTarget);
		else
			emitShortCircuitAnd(rhsExpr, &rhsLabel, falseTarget);
	}
		
	else
	{
		node.rhs->accept(*this);

		emit(Opcode::JMP_IF_TRUE);
		trueTarget->jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
		emit16(0);
	}
}

void BytecodeEmitter::emitShortCircuitAnd(ASTLogical& node, JumpLabel* trueTarget, JumpLabel* falseTarget)
{
	JumpLabel rhsLabel;

	// if lhs is ASTLogical node
	ASTExpr* lhs = ASTUtils::unwrapGroupExpr(node.lhs);
	if (lhs->shortCircuitable)
	{
		ASTLogical& lhsExpr = static_cast<ASTLogical&>(*lhs);

		if (lhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(lhsExpr, &rhsLabel, falseTarget);
		else
			emitShortCircuitAnd(lhsExpr, &rhsLabel, falseTarget);
	}
	else
	{
		node.lhs->accept(*this);

		emit(Opcode::JMP_IF_FALSE);
		rhsLabel.jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
		emit16(0);
	}

	rhsLabel.jumpAddress = (static_cast<uint16_t>(bytecode.size()));
	patchLabel(rhsLabel);

	ASTExpr* rhs = ASTUtils::unwrapGroupExpr(node.rhs);
	if (rhs->shortCircuitable)
	{
		ASTLogical& rhsExpr = static_cast<ASTLogical&>(*rhs);

		if (rhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(rhsExpr, trueTarget, falseTarget);
		else
			emitShortCircuitAnd(rhsExpr, trueTarget, falseTarget);
	}

	else
	{
		node.rhs->accept(*this);

		emit(Opcode::JMP_IF_FALSE);
		falseTarget->jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
		emit16(0);
	}
}

Opcode BytecodeEmitter::getTypeSpecificAddOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::ADD_I8;
		case TypeKind::i16: return Opcode::ADD_I16;
		case TypeKind::i32: return Opcode::ADD_I32;
		case TypeKind::i64: return Opcode::ADD_I64;
		case TypeKind::u8:  return Opcode::ADD_U8;
		case TypeKind::u16: return Opcode::ADD_U16;
		case TypeKind::u32: return Opcode::ADD_U32;
		case TypeKind::u64: return Opcode::ADD_U64;
		case TypeKind::f32: return Opcode::ADD_F32;
		case TypeKind::f64: return Opcode::ADD_F64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificSubOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::SUB_I8;
		case TypeKind::i16: return Opcode::SUB_I16;
		case TypeKind::i32: return Opcode::SUB_I32;
		case TypeKind::i64: return Opcode::SUB_I64;
		case TypeKind::u8:  return Opcode::SUB_U8;
		case TypeKind::u16: return Opcode::SUB_U16;
		case TypeKind::u32: return Opcode::SUB_U32;
		case TypeKind::u64: return Opcode::SUB_U64;
		case TypeKind::f32: return Opcode::SUB_F32;
		case TypeKind::f64: return Opcode::SUB_F64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificMulOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::MUL_I8;
		case TypeKind::i16: return Opcode::MUL_I16;
		case TypeKind::i32: return Opcode::MUL_I32;
		case TypeKind::i64: return Opcode::MUL_I64;
		case TypeKind::u8:  return Opcode::MUL_U8;
		case TypeKind::u16: return Opcode::MUL_U16;
		case TypeKind::u32: return Opcode::MUL_U32;
		case TypeKind::u64: return Opcode::MUL_U64;
		case TypeKind::f32: return Opcode::MUL_F32;
		case TypeKind::f64: return Opcode::MUL_F64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificDivOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::DIV_I8;
		case TypeKind::i16: return Opcode::DIV_I16;
		case TypeKind::i32: return Opcode::DIV_I32;
		case TypeKind::i64: return Opcode::DIV_I64;
		case TypeKind::u8:  return Opcode::DIV_U8;
		case TypeKind::u16: return Opcode::DIV_U16;
		case TypeKind::u32: return Opcode::DIV_U32;
		case TypeKind::u64: return Opcode::DIV_U64;
		case TypeKind::f32: return Opcode::DIV_F32;
		case TypeKind::f64: return Opcode::DIV_F64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificStoreLocalOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::STL_I8;
		case TypeKind::i16:  return Opcode::STL_I16;
		case TypeKind::i32:  return Opcode::STL_I32;
		case TypeKind::i64:  return Opcode::STL_I64;
		case TypeKind::u8:   return Opcode::STL_U8;
		case TypeKind::u16:  return Opcode::STL_U16;
		case TypeKind::u32:  return Opcode::STL_U32;
		case TypeKind::u64:  return Opcode::STL_U64;
		case TypeKind::f32:  return Opcode::STL_F32;
		case TypeKind::f64:  return Opcode::STL_F64;
		case TypeKind::Char: return Opcode::STL_CHAR;
		case TypeKind::Bool: return Opcode::STL_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificStoreGlobalOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::STG_I8;
		case TypeKind::i16:  return Opcode::STG_I16;
		case TypeKind::i32:  return Opcode::STG_I32;
		case TypeKind::i64:  return Opcode::STG_I64;
		case TypeKind::u8:   return Opcode::STG_U8;
		case TypeKind::u16:  return Opcode::STG_U16;
		case TypeKind::u32:  return Opcode::STG_U32;
		case TypeKind::u64:  return Opcode::STG_U64;
		case TypeKind::f32:  return Opcode::STG_F32;
		case TypeKind::f64:  return Opcode::STG_F64;
		case TypeKind::Char: return Opcode::STG_CHAR;
		case TypeKind::Bool: return Opcode::STG_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificNegOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::NEG_I8;
		case TypeKind::i16: return Opcode::NEG_I16;
		case TypeKind::i32: return Opcode::NEG_I32;
		case TypeKind::i64: return Opcode::NEG_I64;
		case TypeKind::f32: return Opcode::NEG_F32;
		case TypeKind::f64: return Opcode::NEG_F64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificNotOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::NOT_I8;
		case TypeKind::i16: return Opcode::NOT_I16;
		case TypeKind::i32: return Opcode::NOT_I32;
		case TypeKind::i64: return Opcode::NOT_I64;
		case TypeKind::u8:  return Opcode::NOT_U8;
		case TypeKind::u16: return Opcode::NOT_U16;
		case TypeKind::u32: return Opcode::NOT_U32;
		case TypeKind::u64: return Opcode::NOT_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificAndOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::AND_I8;
		case TypeKind::i16: return Opcode::AND_I16;
		case TypeKind::i32: return Opcode::AND_I32;
		case TypeKind::i64: return Opcode::AND_I64;
		case TypeKind::u8:  return Opcode::AND_U8;
		case TypeKind::u16: return Opcode::AND_U16;
		case TypeKind::u32: return Opcode::AND_U32;
		case TypeKind::u64: return Opcode::AND_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificOrOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::OR_I8;
		case TypeKind::i16: return Opcode::OR_I16;
		case TypeKind::i32: return Opcode::OR_I32;
		case TypeKind::i64: return Opcode::OR_I64;
		case TypeKind::u8:  return Opcode::OR_U8;
		case TypeKind::u16: return Opcode::OR_U16;
		case TypeKind::u32: return Opcode::OR_U32;
		case TypeKind::u64: return Opcode::OR_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificXorOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::XOR_I8;
		case TypeKind::i16: return Opcode::XOR_I16;
		case TypeKind::i32: return Opcode::XOR_I32;
		case TypeKind::i64: return Opcode::XOR_I64;
		case TypeKind::u8:  return Opcode::XOR_U8;
		case TypeKind::u16: return Opcode::XOR_U16;
		case TypeKind::u32: return Opcode::XOR_U32;
		case TypeKind::u64: return Opcode::XOR_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificShlOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::SHL_I8;
		case TypeKind::i16: return Opcode::SHL_I16;
		case TypeKind::i32: return Opcode::SHL_I32;
		case TypeKind::i64: return Opcode::SHL_I64;
		case TypeKind::u8:  return Opcode::SHL_U8;
		case TypeKind::u16: return Opcode::SHL_U16;
		case TypeKind::u32: return Opcode::SHL_U32;
		case TypeKind::u64: return Opcode::SHL_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificShrOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return Opcode::SHR_I8;
		case TypeKind::i16: return Opcode::SHR_I16;
		case TypeKind::i32: return Opcode::SHR_I32;
		case TypeKind::i64: return Opcode::SHR_I64;
		case TypeKind::u8:  return Opcode::SHR_U8;
		case TypeKind::u16: return Opcode::SHR_U16;
		case TypeKind::u32: return Opcode::SHR_U32;
		case TypeKind::u64: return Opcode::SHR_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificEqOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::EQ_I8;
		case TypeKind::i16:  return Opcode::EQ_I16;
		case TypeKind::i32:  return Opcode::EQ_I32;
		case TypeKind::i64:  return Opcode::EQ_I64;
		case TypeKind::u8:   return Opcode::EQ_U8;
		case TypeKind::u16:  return Opcode::EQ_U16;
		case TypeKind::u32:  return Opcode::EQ_U32;
		case TypeKind::u64:  return Opcode::EQ_U64;
		case TypeKind::f32:  return Opcode::EQ_F32;
		case TypeKind::f64:  return Opcode::EQ_F64;
		case TypeKind::Char: return Opcode::EQ_CHAR;
		case TypeKind::Bool: return Opcode::EQ_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificNeqOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::NEQ_I8;
		case TypeKind::i16:  return Opcode::NEQ_I16;
		case TypeKind::i32:  return Opcode::NEQ_I32;
		case TypeKind::i64:  return Opcode::NEQ_I64;
		case TypeKind::u8:   return Opcode::NEQ_U8;
		case TypeKind::u16:  return Opcode::NEQ_U16;
		case TypeKind::u32:  return Opcode::NEQ_U32;
		case TypeKind::u64:  return Opcode::NEQ_U64;
		case TypeKind::f32:  return Opcode::NEQ_F32;
		case TypeKind::f64:  return Opcode::NEQ_F64;
		case TypeKind::Char: return Opcode::NEQ_CHAR;
		case TypeKind::Bool: return Opcode::NEQ_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificGtOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::GT_I8;
		case TypeKind::i16:  return Opcode::GT_I16;
		case TypeKind::i32:  return Opcode::GT_I32;
		case TypeKind::i64:  return Opcode::GT_I64;
		case TypeKind::u8:   return Opcode::GT_U8;
		case TypeKind::u16:  return Opcode::GT_U16;
		case TypeKind::u32:  return Opcode::GT_U32;
		case TypeKind::u64:  return Opcode::GT_U64;
		case TypeKind::f32:  return Opcode::GT_F32;
		case TypeKind::f64:  return Opcode::GT_F64;
		case TypeKind::Char: return Opcode::GT_CHAR;
	}
}

Opcode BytecodeEmitter::getTypeSpecificGteOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::GTE_I8;
		case TypeKind::i16:  return Opcode::GTE_I16;
		case TypeKind::i32:  return Opcode::GTE_I32;
		case TypeKind::i64:  return Opcode::GTE_I64;
		case TypeKind::u8:   return Opcode::GTE_U8;
		case TypeKind::u16:  return Opcode::GTE_U16;
		case TypeKind::u32:  return Opcode::GTE_U32;
		case TypeKind::u64:  return Opcode::GTE_U64;
		case TypeKind::f32:  return Opcode::GTE_F32;
		case TypeKind::f64:  return Opcode::GTE_F64;
		case TypeKind::Char: return Opcode::GTE_CHAR;
	}
}

Opcode BytecodeEmitter::getTypeSpecificLtOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::LT_I8;
		case TypeKind::i16:  return Opcode::LT_I16;
		case TypeKind::i32:  return Opcode::LT_I32;
		case TypeKind::i64:  return Opcode::LT_I64;
		case TypeKind::u8:   return Opcode::LT_U8;
		case TypeKind::u16:  return Opcode::LT_U16;
		case TypeKind::u32:  return Opcode::LT_U32;
		case TypeKind::u64:  return Opcode::LT_U64;
		case TypeKind::f32:  return Opcode::LT_F32;
		case TypeKind::f64:  return Opcode::LT_F64;
		case TypeKind::Char: return Opcode::LT_CHAR;
	}
}

Opcode BytecodeEmitter::getTypeSpecificLteOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::LTE_I8;
		case TypeKind::i16:  return Opcode::LTE_I16;
		case TypeKind::i32:  return Opcode::LTE_I32;
		case TypeKind::i64:  return Opcode::LTE_I64;
		case TypeKind::u8:   return Opcode::LTE_U8;
		case TypeKind::u16:  return Opcode::LTE_U16;
		case TypeKind::u32:  return Opcode::LTE_U32;
		case TypeKind::u64:  return Opcode::LTE_U64;
		case TypeKind::f32:  return Opcode::LTE_F32;
		case TypeKind::f64:  return Opcode::LTE_F64;
		case TypeKind::Char: return Opcode::LTE_CHAR;
	}
}

Opcode BytecodeEmitter::getTypeSpecificLoadLocalOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::LDL_I8;
		case TypeKind::i16:  return Opcode::LDL_I16;
		case TypeKind::i32:  return Opcode::LDL_I32;
		case TypeKind::i64:  return Opcode::LDL_I64;
		case TypeKind::u8:   return Opcode::LDL_U8;
		case TypeKind::u16:  return Opcode::LDL_U16;
		case TypeKind::u32:  return Opcode::LDL_U32;
		case TypeKind::u64:  return Opcode::LDL_U64;
		case TypeKind::f32:  return Opcode::LDL_F32;
		case TypeKind::f64:  return Opcode::LDL_F64;
		case TypeKind::Char: return Opcode::LDL_CHAR;
		case TypeKind::Bool: return Opcode::LDL_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificLoadGlobalOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::LDG_I8;
		case TypeKind::i16:  return Opcode::LDG_I16;
		case TypeKind::i32:  return Opcode::LDG_I32;
		case TypeKind::i64:  return Opcode::LDG_I64;
		case TypeKind::u8:   return Opcode::LDG_U8;
		case TypeKind::u16:  return Opcode::LDG_U16;
		case TypeKind::u32:  return Opcode::LDG_U32;
		case TypeKind::u64:  return Opcode::LDG_U64;
		case TypeKind::f32:  return Opcode::LDG_F32;
		case TypeKind::f64:  return Opcode::LDG_F64;
		case TypeKind::Char: return Opcode::LDG_CHAR;
		case TypeKind::Bool: return Opcode::LDG_BOOL;
	}
}

Opcode BytecodeEmitter::getTypeSpecificIncOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::INC_I8;
		case TypeKind::i16:  return Opcode::INC_I16;
		case TypeKind::i32:  return Opcode::INC_I32;
		case TypeKind::i64:  return Opcode::INC_I64;
		case TypeKind::u8:   return Opcode::INC_U8;
		case TypeKind::u16:  return Opcode::INC_U16;
		case TypeKind::u32:  return Opcode::INC_U32;
		case TypeKind::u64:  return Opcode::INC_U64;
	}
}

Opcode BytecodeEmitter::getTypeSpecificDecOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::DEC_I8;
		case TypeKind::i16:  return Opcode::DEC_I16;
		case TypeKind::i32:  return Opcode::DEC_I32;
		case TypeKind::i64:  return Opcode::DEC_I64;
		case TypeKind::u8:   return Opcode::DEC_U8;
		case TypeKind::u16:  return Opcode::DEC_U16;
		case TypeKind::u32:  return Opcode::DEC_U32;
		case TypeKind::u64:  return Opcode::DEC_U64;
	}
}

void BytecodeEmitter::emit(Opcode opcode)
{
	uint16_t val = static_cast<uint16_t>(opcode);
	emit16(val);
}

void BytecodeEmitter::emit8(uint8_t value)
{
	bytecode.push_back(value);
}

void BytecodeEmitter::emit16(uint16_t value)
{
	bytecode.push_back(value & 0xFF);
	bytecode.push_back((value >> 8) & 0xFF);
}

void BytecodeEmitter::emit32(uint32_t value)
{
	bytecode.push_back(value & 0xFF);
	bytecode.push_back((value >> 8) & 0xFF);
	bytecode.push_back((value >> 16) & 0xFF);
	bytecode.push_back((value >> 24) & 0xFF);
}

void BytecodeEmitter::emit64(uint64_t value)
{
	bytecode.push_back(value & 0xFF);
	bytecode.push_back((value >> 8) & 0xFF);
	bytecode.push_back((value >> 16) & 0xFF);
	bytecode.push_back((value >> 24) & 0xFF);
	bytecode.push_back((value >> 32) & 0xFF);
	bytecode.push_back((value >> 40) & 0xFF);
	bytecode.push_back((value >> 48) & 0xFF);
	bytecode.push_back((value >> 56) & 0xFF);
}

void BytecodeEmitter::visitIntLiteral(ASTIntLiteral& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	switch (type)
	{
		case TypeKind::i8:  
			emit(Opcode::PUSH_I8);  
			emit8(static_cast<uint8_t>(node.value));
			break;
		case TypeKind::i16: 
			emit(Opcode::PUSH_I16); 
			emit16(static_cast<uint16_t>(node.value));
			break;
		case TypeKind::i32: 
			emit(Opcode::PUSH_I32); 
			emit32(static_cast<uint32_t>(node.value));
			break;
		case TypeKind::i64: 
			emit(Opcode::PUSH_I64); 
			emit64(static_cast<uint64_t>(node.value));
			break;
		case TypeKind::u8:  
			emit(Opcode::PUSH_U8); 
			emit8(static_cast<uint8_t>(node.value));
			break;
		case TypeKind::u16: 
			emit(Opcode::PUSH_U16);
			emit16(static_cast<uint16_t>(node.value));
			break;
		case TypeKind::u32: 
			emit(Opcode::PUSH_U32); 
			emit32(static_cast<uint32_t>(node.value));
			break;
		case TypeKind::u64: 
			emit(Opcode::PUSH_U64); 
			emit64(static_cast<uint64_t>(node.value));
			break;
	}
}

void BytecodeEmitter::visitDoubleLiteral(ASTDoubleLiteral& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	switch (type)
	{
		case TypeKind::f32:
		{
			float value = static_cast<float>(node.value);
			uint32_t bits;
			std::memcpy(&bits, &value, sizeof(bits));
			emit(Opcode::PUSH_F32);
			emit32(bits);
			break;
		}

		case TypeKind::f64:
		{
			double value = node.value;
			uint64_t bits;
			std::memcpy(&bits, &value, sizeof(bits));
			emit(Opcode::PUSH_F64);
			emit64(bits);
			break;
		}
	}
}

void BytecodeEmitter::visitCharLiteral(ASTCharLiteral& node)
{
	emit(Opcode::PUSH_CHAR);
	emit8(static_cast<uint8_t>(node.value));
}

void BytecodeEmitter::visitBoolLiteral(ASTBoolLiteral& node)
{
	emit(Opcode::PUSH_BOOL);
	emit8(static_cast<uint8_t>(node.value));
}

void BytecodeEmitter::visitVarDecl(ASTVarDecl& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;
	
	// push the initialization value or a default value for the type if no initializer is provided
	if (node.initialization)
		node.initialization->accept(*this);
	else
		emitDefaultValue(type);

	emit(node.scope == 0 ? getTypeSpecificStoreGlobalOpcode(type) : getTypeSpecificStoreLocalOpcode(type));

	// push the slot index to store the variable in
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitFuncDecl(ASTFuncDecl& node)
{
	// need to add to deferredFunctions vector, don't emit any code here
	deferredFunctions.push_back(&node);

	// add function meta data to FunctionTable, startAddress is defaulted to 0 and will later get patched in when iterating over
	// the deferredFunctions vector
	functionTable.addFunction(std::get<std::string>(node.funcIdentifier.value), static_cast<uint8_t>(node.params->params.size()), 0, node.typeInfo->localsCount);
}

void BytecodeEmitter::visitIdentifier(ASTIdentifier& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;
	emit(node.scope == 0 ? getTypeSpecificLoadGlobalOpcode(type) : getTypeSpecificLoadLocalOpcode(type));

	// push the slot index to store the variable in
	emit16(static_cast<uint16_t>(node.slotIndex));

	if (node.scope == 0)
		std::cout << "Emitter loading global variable \"" << node.typeInfo->name << "\"\n";
	else
		std::cout << "Emitter loading local variable \"" << node.typeInfo->name << "\"\n";
}

void BytecodeEmitter::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void BytecodeEmitter::visitAssign(ASTAssign& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;
	
	// simple assigns:
	// PUSH value
	// STORE slot
	//
	// compound assignments:
	// LOAD slot
	// PUSH value
	// ADD/SUB/MUL
	// STORE slot

	// for compound assignment operators, we need to load the variable before pushing the value we're assigning
	if (node.op.type != TokenType::Assign)
	{
		
		emit(node.scope == 0 ? getTypeSpecificLoadGlobalOpcode(type) : getTypeSpecificLoadLocalOpcode(type));
		emit16(static_cast<uint16_t>(node.slotIndex));
	}

	// push the value we're assigning onto the stack
	node.value->accept(*this);

	// we dont need to switch on TokenType::Assign, since the logic is the same for every assign node at this point -> ADD/SUB/..., STORE_*, SLOT
	// but normal assign tokens dont have to emit an add/sub/... instruction, just a store, then the slot
	switch (node.op.type)
	{
		case TokenType::PlusEquals:
			emit(getTypeSpecificAddOpcode(type));
			break;
		case TokenType::MinusEquals:
			emit(getTypeSpecificSubOpcode(type));
			break;
		case TokenType::TimesEquals:
			emit(getTypeSpecificMulOpcode(type));
			break;
		case TokenType::DividedEquals:
			emit(getTypeSpecificDivOpcode(type));
			break;
		case TokenType::BitwiseAndEquals:
			emit(getTypeSpecificAndOpcode(type));
			break;
		case TokenType::BitwiseOrEquals:
			emit(getTypeSpecificOrOpcode(type));
			break;
		case TokenType::BitwiseXorEquals:
			emit(getTypeSpecificXorOpcode(type));
			break;
		case TokenType::BitwiseLeftShiftEquals:
			emit(getTypeSpecificShlOpcode(type));
			break;
		case TokenType::BitwiseRightShiftEquals:
			emit(getTypeSpecificShrOpcode(type));
			break;
	}

	emit(node.scope == 0 ? getTypeSpecificStoreGlobalOpcode(type) : getTypeSpecificStoreLocalOpcode(type));
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitReturn(ASTReturn& node)
{
	if (node.returnVal)
		node.returnVal->accept(*this);
	
	emit(Opcode::RET);
}

void BytecodeEmitter::visitBlock(ASTBlock& node)
{
	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);
}

void BytecodeEmitter::visitForLoop(ASTForLoop& node)
{
	conditionCtxStack.emplace_back(ConditionContext());
	if (node.initializer)
		node.initializer->accept(*this);

	// store startAddress as the condition code, for loops need to execute: condition -> body -> increment, every loop
	uint16_t startAddress = static_cast<uint16_t>(bytecode.size());
	if (node.condition)
	{
		shortCircuitCtx = true;
		node.condition->accept(*this);
		shortCircuitCtx = false;
	}
	
	emit(Opcode::JMP_IF_FALSE);
	conditionCtxStack.back().end.jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
	emit16(0);

	node.body->accept(*this);

	if (node.increment)
		node.increment->accept(*this);
	
	emit(Opcode::JMP);
	emit16(startAddress);

	patchLabelJumps(conditionCtxStack.back().falseJump.jumpPatches, static_cast<uint16_t>(bytecode.size()));
	patchLabelJumps(conditionCtxStack.back().end.jumpPatches, static_cast<uint16_t>(bytecode.size()));
	conditionCtxStack.pop_back();
}

void BytecodeEmitter::visitWhileLoop(ASTWhileLoop& node)
{	
	// push a new condition context and set the short circuit context for the while loops condition
	conditionCtxStack.emplace_back(ConditionContext());
	shortCircuitCtx = true;
	
	// save the start address of the while loop so we can jump back to the start at the end of the body
	uint16_t startAddress = static_cast<uint16_t>(bytecode.size());
	node.condition->accept(*this);
	shortCircuitCtx = false;

	// if the condition is false, we need to jump to the end of the while loop. we don't know yet where the end is so we
	// put in a temp address of 0, then patch in the real address at the end of the while loop
	emit(Opcode::JMP_IF_FALSE);
	conditionCtxStack.back().end.jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
	emit16(0);

	// visit the body
	node.body->accept(*this);

	// we know the address of the start of the while loop, so we dont need to patch in the start jump address
	// we can just directly emit it into the bytecode
	emit(Opcode::JMP);
	emit16(startAddress);

	// false and end jumps are both jumping to the same place, they're just jumping to the next valid instruction after the while loop
	patchLabelJumps(conditionCtxStack.back().falseJump.jumpPatches, static_cast<uint16_t>(bytecode.size()));
	patchLabelJumps(conditionCtxStack.back().end.jumpPatches, static_cast<uint16_t>(bytecode.size()));
	conditionCtxStack.pop_back();
}

void BytecodeEmitter::visitIfStatement(ASTIfStatement& node)
{
	conditionCtxStack.emplace_back(ConditionContext());
	shortCircuitCtx = true;
	node.condition->accept(*this);
	shortCircuitCtx = false;

	// try to replace the last JMP instruction emitted through short circuiting with a JMP_IF_FALSE to skip the true branch
	if (!tryOverwriteLastJump(Opcode::JMP_IF_FALSE))
	{
		emit(Opcode::JMP_IF_FALSE);
		conditionCtxStack.back().falseJump.jumpPatches.push_back(static_cast<uint16_t>(bytecode.size()));
		emit16(0);
	}

	uint16_t trueBranchAddress = static_cast<uint16_t>(bytecode.size());
	node.trueBranch->accept(*this);
	
	// if there's no false branch, then this is the end of the if statement, so this works as a jump target for JMP_IF_FALSE instructions
	uint16_t falseBranchAddress = static_cast<uint16_t>(bytecode.size());

	// if there's a false branch, we need an unconditional jump to jump over the false branch in the event that the true branch was taken
	if (node.falseBranch)
	{
		emit(Opcode::JMP);
		size_t jumpAddress = bytecode.size();
		emit16(0);

		// if there is a false branch, overwrite the falseBranchAddress value to point to the false branch
		falseBranchAddress = static_cast<uint16_t>(bytecode.size());
		node.falseBranch->accept(*this);

		// manually patch the unconditional jump to skip the false branch
		insert16(static_cast<uint16_t>(bytecode.size()), jumpAddress);
	}

	patchLabelJumps(conditionCtxStack.back().trueJump.jumpPatches, trueBranchAddress);
	patchLabelJumps(conditionCtxStack.back().falseJump.jumpPatches, falseBranchAddress);

	conditionCtxStack.pop_back();
}

void BytecodeEmitter::visitLogical(ASTLogical& node)
{
	// do short circuit logic for conditions inside if, while, and for loops
	if (shortCircuitCtx)
	{
		if (node.logicalOperator.type == TokenType::LogicalAnd)
			emitShortCircuitAnd(node, &conditionCtxStack.back().trueJump, &conditionCtxStack.back().falseJump);
		else if (node.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(node, &conditionCtxStack.back().trueJump, &conditionCtxStack.back().falseJump);
	}

	// otherwise, no short circuiting behavior is necessary, just push the two operands, then the corresponding logical instruction
	else
	{
		node.lhs->accept(*this);
		node.rhs->accept(*this);

		if (node.logicalOperator.type == TokenType::LogicalAnd)
			emit(Opcode::LOGICAL_AND);
		else if (node.logicalOperator.type == TokenType::LogicalOr)
			emit(Opcode::LOGICAL_OR);
	}
}

void BytecodeEmitter::visitBinaryExpr(ASTBinaryExpr& node)
{
	std::cout << "Emitting binary expr, shortCircuitCtx = " << shortCircuitCtx << std::endl;
	node.lhs->accept(*this);
	node.rhs->accept(*this);

	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	switch (node.op.type)
	{
		case TokenType::Plus:
			emit(getTypeSpecificAddOpcode(type));
			break;
		case TokenType::Minus:
			emit(getTypeSpecificSubOpcode(type));
			break;
		case TokenType::Asterisk:
			emit(getTypeSpecificMulOpcode(type));
			break;
		case TokenType::ForwardSlash:
			emit(getTypeSpecificDivOpcode(type));
			break;
		case TokenType::BitwiseAnd:
			emit(getTypeSpecificAndOpcode(type));
			break;
		case TokenType::BitwiseOr:
			emit(getTypeSpecificOrOpcode(type));
			break;
		case TokenType::BitwiseXor:
			emit(getTypeSpecificXorOpcode(type));
			break;
		case TokenType::BitwiseLeftShift:
			emit(getTypeSpecificShlOpcode(type));
			break;
		case TokenType::BitwiseRightShift:
			emit(getTypeSpecificShrOpcode(type));
			break;

		// For comparison operations, the binary expr node is ALWAYS set to Bool type, so we need to get the type
		// from one of the sub expressions. They should match because of the type checking phase, so we can take either one

			/*
			
			need to check if in condition context, can use the ConditionCtxStack for this, for comparison operations,
			if in a condition context, push the normal GT, LT, NEQ, etc. instruction, but also emit JMP_IF_FALSE instructions
			and patch it in with the conditionCtx.back().patch.... like how im doing in visitLogical.
			Maybe even refactor this into a function specific to this like doComparisonInCondition, doLogicalInCondition, etc 
			to make things more clean. MIGHT even need to do this in pure visitBoolLiteral, need to test with different source files

			test with:
			if (true) {} else {}
			
			if (true || false) {} else {}

			if (true && false) {} else {}

			if ((true && false) || true) {} else {}

			if (10 > 100) {} else {}

			if ((true) || (10 > 100)) {} else {}

			*/
		case TokenType::NotEq:
			emit(getTypeSpecificNeqOpcode(node.lhs->typeInfo->type));
			break;
		case TokenType::Equality:
			emit(getTypeSpecificEqOpcode(node.lhs->typeInfo->type));
			break;
		case TokenType::GreaterThan:
			emit(getTypeSpecificGtOpcode(node.lhs->typeInfo->type));
			break;
		case TokenType::GreaterThanEq:
			emit(getTypeSpecificGteOpcode(node.lhs->typeInfo->type));
			break;
		case TokenType::LessThan:
			emit(getTypeSpecificLtOpcode(node.lhs->typeInfo->type));
			break;
		case TokenType::LessThanEq:
			emit(getTypeSpecificLteOpcode(node.lhs->typeInfo->type));
			break;
	}
}

void BytecodeEmitter::visitUnaryExpr(ASTUnaryExpr& node)
{
	// load
	node.expr->accept(*this);
	
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	// --x or ++x
	// LOAD  -> [..., x]
	// INC   -> [..., x - 1]
	// DUP   -> [..., x - 1, x - 1]
	// STORE -> [..., x - 1]

	// !x, -x, or ~x
	// LOAD  -> [..., x]
	// NOT   -> [..., !x]
	switch (node.op.type)
	{
		case TokenType::Increment:
			emit(getTypeSpecificIncOpcode(type));
			emit(Opcode::DUP);
			emit(node.scope == 0 ? getTypeSpecificStoreGlobalOpcode(type) : getTypeSpecificStoreLocalOpcode(type));
			emit16(static_cast<uint16_t>(node.slotIndex));
			break;
		case TokenType::Decrement:
			emit(getTypeSpecificDecOpcode(type));
			emit(Opcode::DUP);
			emit(node.scope == 0 ? getTypeSpecificStoreGlobalOpcode(type) : getTypeSpecificStoreLocalOpcode(type));
			emit16(static_cast<uint16_t>(node.slotIndex));
			break;
		case TokenType::LogicalNot:
			emit(Opcode::LOGICAL_NOT);
			break;
		case TokenType::Minus:
			emit(getTypeSpecificNegOpcode(type));
			break;
		case TokenType::BitwiseNot:
			emit(getTypeSpecificNotOpcode(type));
			break;
	}
}

void BytecodeEmitter::visitCall(ASTCall& node)
{
	// i dont need to do this yet. Right now, callee nodes are only ever ASTIdentifier nodes, when i support more complex things like
	// foo()(), or functionPointers, i will need to visit the callee and do more work, but right now this should work
	//node.callee->accept(*this);

	// if the callee resolves to a function already in the function table, great, emit code for the call
	if (VMFunctionEntry* function = functionTable.lookupFunctionByName(node.callee->typeInfo->name))
	{
		std::cout << "found function in VM Function Table: " << node.callee->typeInfo->name << std::endl;
		node.args->accept(*this);
		emit(Opcode::CALL);
		emit16(function->funcIndex);
		emit8(function->argCount);
	}
	else
	{
		DeferredCall deferredCall;
		deferredCall.call = &node;
		node.args->accept(*this);
		emit(Opcode::CALL);

		// callAddress will point to the beginning of the CALL instruction's operands, for now we emit dummy values for the func index and arg count
		deferredCall.callAddress = bytecode.size();
		emit16(0);
		emit8(0);
		deferredCalls.emplace_back(deferredCall);
		std::cout << "deferring function call for: " << node.callee->typeInfo->name << " setting call address to " << deferredCall.callAddress << std::endl;

	}
}

void BytecodeEmitter::visitGroupExpr(ASTGroupExpr& node)
{
	node.expr->accept(*this);
}

void BytecodeEmitter::visitPostfix(ASTPostfix& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	// this should get the LOAD_* instruction, more complex types later on like arrary access will emit a different instruction? we'll see
	node.expr->accept(*this);

	// LOAD ->  [..., x]
	// DUP  ->  [..., x, x]
	// INC  ->  [..., x, x + 1]
	// STORE -> [..., x]
	if (node.op.type == TokenType::Increment)
	{
		emit(Opcode::DUP);
		emit(getTypeSpecificIncOpcode(type));
	}
	else if (node.op.type == TokenType::Decrement)
	{
		emit(Opcode::DUP);
		emit(getTypeSpecificDecOpcode(type));
	}

	// have to output store instruction AND the variables slot, forgetting to emit the slot was the cause of a nasty bug
	emit(node.scope == 0 ? getTypeSpecificStoreGlobalOpcode(type) : getTypeSpecificStoreLocalOpcode(type));
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitArgument(ASTArgument& node)
{
	node.value->accept(*this);
}

void BytecodeEmitter::visitArgList(ASTArgList& node)
{
	for (ASTArgument* arg : node.args)
		arg->accept(*this);
}

void BytecodeEmitter::visitCast(ASTCast& node)
{
	implicitCastCtx = node.typeInfo;
	node.expr->accept(*this);
	implicitCastCtx = nullptr;
}