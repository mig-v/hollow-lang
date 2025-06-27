#include "bytecode_emitter.h"
#include <iostream>

BytecodeEmitter::BytecodeEmitter()
{
	this->implicitCastCtx = nullptr;
	this->shortCircuitCtx = false;
}

void BytecodeEmitter::generateBytecode(std::vector<ASTNode*>& ast)
{
	for (ASTNode* node : ast)
		node->accept(*this);
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

void BytecodeEmitter::patchLabelJumps(std::vector<uint16_t>& patchSites, uint16_t address)
{
	// only have to call patchLabelJumps when we dont know what address we're going to be jumping to.
	// E.G. when jumping over true branch to false branch, we dont know the address of the false branch
	// until after we generate code for the true branch so we have to come back and patch in those jump addresses
	std::cout << "size of patchSites for start: " << (uint32_t)patchSites.size() << std::endl;
	for (size_t i = 0; i < patchSites.size(); i++)
		insert16(address, static_cast<uint16_t>(patchSites[i]));
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

Opcode BytecodeEmitter::getTypeSpecificStoreOpcode(TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:   return Opcode::STORE_I8;
		case TypeKind::i16:  return Opcode::STORE_I16;
		case TypeKind::i32:  return Opcode::STORE_I32;
		case TypeKind::i64:  return Opcode::STORE_I64;
		case TypeKind::u8:   return Opcode::STORE_U8;
		case TypeKind::u16:  return Opcode::STORE_U16;
		case TypeKind::u32:  return Opcode::STORE_U32;
		case TypeKind::u64:  return Opcode::STORE_U64;
		case TypeKind::f32:  return Opcode::STORE_F32;
		case TypeKind::f64:  return Opcode::STORE_F64;
		case TypeKind::Char: return Opcode::STORE_CHAR;
		case TypeKind::Bool: return Opcode::STORE_BOOL;
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

	emit(getTypeSpecificStoreOpcode(type));
	
	// push the slot index to store the variable in
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitFuncDecl(ASTFuncDecl& node)
{
	
}

void BytecodeEmitter::visitIdentifier(ASTIdentifier& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	switch (type)
	{
		case TypeKind::i8:
			emit(Opcode::LOAD_I8);
			break;
		case TypeKind::i16:
			emit(Opcode::LOAD_I16);
			break;
		case TypeKind::i32:
			emit(Opcode::LOAD_I32);
			break;
		case TypeKind::i64:
			emit(Opcode::LOAD_I64);
			break;
		case TypeKind::u8:
			emit(Opcode::LOAD_U8);
			break;
		case TypeKind::u16:
			emit(Opcode::LOAD_U16);
			break;
		case TypeKind::u32:
			emit(Opcode::LOAD_U32);
			break;
		case TypeKind::u64:
			emit(Opcode::LOAD_U64);
			break;
		case TypeKind::f32:
			emit(Opcode::LOAD_F32);
			break;
		case TypeKind::f64:
			emit(Opcode::LOAD_F64);
			break;
		case TypeKind::Char:
			emit(Opcode::LOAD_CHAR);
			break;
		case TypeKind::Bool:
			emit(Opcode::LOAD_BOOL);
			break;
	}

	// push the slot index to store the variable in
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void BytecodeEmitter::visitAssign(ASTAssign& node)
{

}

void BytecodeEmitter::visitReturn(ASTReturn& node)
{

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
	conditionCtxStack.back().end.jumpPatches.push_back(bytecode.size());
	emit16(0);

	node.body->accept(*this);

	if (node.increment)
		node.increment->accept(*this);
	
	emit(Opcode::JMP);
	emit16(startAddress);

	patchLabelJumps(conditionCtxStack.back().falseBranch.jumpPatches, bytecode.size());
	patchLabelJumps(conditionCtxStack.back().end.jumpPatches, bytecode.size());
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
	conditionCtxStack.back().end.jumpPatches.push_back(bytecode.size());
	emit16(0);

	// visit the body
	node.body->accept(*this);

	// we know the address of the start of the while loop, so we dont need to patch in the start jump address
	// we can just directly emit it into the bytecode
	emit(Opcode::JMP);
	emit16(startAddress);

	// false and end jumps are both jumping to the same place, they're just jumping to the next valid instruction after the while loop
	patchLabelJumps(conditionCtxStack.back().falseBranch.jumpPatches, bytecode.size());
	patchLabelJumps(conditionCtxStack.back().end.jumpPatches, bytecode.size());
	conditionCtxStack.pop_back();
}

void BytecodeEmitter::visitIfStatement(ASTIfStatement& node)
{
	conditionCtxStack.emplace_back(ConditionContext());
	shortCircuitCtx = true;
	node.condition->accept(*this);
	shortCircuitCtx = false;

	// patch in label jumps for whatever Label right BEFORE the code for that label should start
	patchLabelJumps(conditionCtxStack.back().trueBranch.jumpPatches, bytecode.size());
	node.trueBranch->accept(*this);

	// if there's a false branch, we need to emit a JMP after the true branch to skip over the true branch if the condition is false
	if (node.falseBranch)
	{
		emit(Opcode::JMP);
		conditionCtxStack.back().end.jumpPatches.push_back(bytecode.size());
		emit16(0);
		patchLabelJumps(conditionCtxStack.back().falseBranch.jumpPatches, bytecode.size());
		node.falseBranch->accept(*this);
		patchLabelJumps(conditionCtxStack.back().end.jumpPatches, bytecode.size());
	}

	// otherwise, if there's not a false branch, we just need to fill in the patchSites to jump to the end of the if statement
	else
	{
		patchLabelJumps(conditionCtxStack.back().falseBranch.jumpPatches, bytecode.size());
	}

	conditionCtxStack.pop_back();
}

void BytecodeEmitter::visitLogical(ASTLogical& node)
{
	// do short circuit logic for conditions inside if, while, and for loops
	if (shortCircuitCtx)
	{
		if (node.logicalOperator.type == TokenType::LogicalAnd)
		{
			node.lhs->accept(*this);
			emit(Opcode::JMP_IF_FALSE);
			conditionCtxStack.back().falseBranch.jumpPatches.push_back(bytecode.size());
			emit16(0);	// emit temp jump address, since we dont know where the false branch will be located

			node.rhs->accept(*this);
			emit(Opcode::JMP_IF_FALSE);
			conditionCtxStack.back().falseBranch.jumpPatches.push_back(bytecode.size());
			emit16(0);
		}
		else if (node.logicalOperator.type == TokenType::LogicalOr)
		{
			node.lhs->accept(*this);
			emit(Opcode::JMP_IF_TRUE);
			conditionCtxStack.back().trueBranch.jumpPatches.push_back(bytecode.size());
			emit16(0);

			node.rhs->accept(*this);
			emit(Opcode::JMP_IF_TRUE);
			conditionCtxStack.back().trueBranch.jumpPatches.push_back(bytecode.size());
			emit16(0);

			emit(Opcode::JMP);
			conditionCtxStack.back().falseBranch.jumpPatches.push_back(bytecode.size());
			emit16(0);
		}
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
		case TokenType::NotEq:
			emit(getTypeSpecificNeqOpcode(type));
			break;
		case TokenType::Equality:
			emit(getTypeSpecificEqOpcode(type));
			break;
		case TokenType::GreaterThan:
			emit(getTypeSpecificGtOpcode(type));
			break;
		case TokenType::GreaterThanEq:
			emit(getTypeSpecificGteOpcode(type));
			break;
		case TokenType::LessThan:
			emit(getTypeSpecificLtOpcode(type));
			break;
		case TokenType::LessThanEq:
			emit(getTypeSpecificLteOpcode(type));
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
			emit(Opcode::INC);
			emit(Opcode::DUP);
			emit(getTypeSpecificStoreOpcode(type));
			emit16(static_cast<uint16_t>(node.slotIndex));
			break;
		case TokenType::Decrement:
			emit(Opcode::DEC);
			emit(Opcode::DUP);
			emit(getTypeSpecificStoreOpcode(type));
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
		emit(Opcode::INC);
	}
	else if (node.op.type == TokenType::Decrement)
	{
		emit(Opcode::DUP);
		emit(Opcode::DEC);
	}

	// have to output store instruction AND the variables slot, forgetting to emit the slot was the cause of a nasty bug
	emit(getTypeSpecificStoreOpcode(type));
	emit16(static_cast<uint16_t>(node.slotIndex));
}

void BytecodeEmitter::visitParameter(ASTParameter& node)
{

}

void BytecodeEmitter::visitArgument(ASTArgument& node)
{

}

void BytecodeEmitter::visitParamList(ASTParamList& node)
{

}

void BytecodeEmitter::visitArgList(ASTArgList& node)
{

}

void BytecodeEmitter::visitCast(ASTCast& node)
{
	implicitCastCtx = node.typeInfo;
	node.expr->accept(*this);
	implicitCastCtx = nullptr;
}