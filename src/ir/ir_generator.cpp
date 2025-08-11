#include "ir/ir_generator.h"

#include "semantics/constant_evaluator.h"

#include "utils/debug_utils.h"
#include "utils/ast_utils.h"
#include "utils/semantic_utils.h"

#include <iostream>

IRGenerator::IRGenerator()
{
	this->currID = 0;
	this->funcID = 0;
	this->labelID = 0;
	this->shortCircuitCtx = false;
	this->implicitCastCtx = nullptr;
	this->typeArena = nullptr;
	this->currentFunction = nullptr;
	this->typeTable = nullptr;
}

void IRGenerator::generate(std::vector<ASTNode*>& ast, MemoryArena* typeArena, TypeTable* typeTable)
{
	currID = 0;
	funcID = 0;
	labelID = 0;
	shortCircuitCtx = false;
	implicitCastCtx = nullptr;
	this->typeArena = typeArena;
	this->typeTable = typeTable;
	nullOperand = makeNullOperand();

	std::cout << "generating...\n";
	for (ASTNode* node : ast)
		node->accept(*this);
}

uint32_t IRGenerator::newID()
{
	return currID++;
}

uint32_t IRGenerator::newFuncID()
{
	return funcID++;
}

uint32_t IRGenerator::newLabelID()
{
	return labelID++;
}

void IRGenerator::pushValue(const TacValue& value)
{
	// NOTE: this assumes ids are generated and pushed sequentially. They cannot be out of order or else things will break
	irData.values.push_back(value);
}

void IRGenerator::setValue(uint32_t id, const TacValue& value)
{
	//values[id] = value;
	std::cout << "set value called\n";
	irData.values[id].typeInfo = value.typeInfo;
	irData.values[id].id = value.id;
	irData.values[id].kind = value.kind;
}

void IRGenerator::pushEvalMode(EvalMode mode)
{
	evalModeStack.push_back(mode);
}

void IRGenerator::popEvalMode()
{
	evalModeStack.pop_back();
}

EvalMode IRGenerator::getEvalMode()
{
	// default to RValue if eval mode stack is empty, this should be safe in all scenarios since any node that needs LValue behavior would have set it
	return evalModeStack.empty() ? EvalMode::RValue : evalModeStack.back();
}

uint32_t IRGenerator::makeTemporary(TypeKind type)
{
	TacValue value;
	value.id = newID();
	value.kind = ValueKind::Temporary;
	value.temporaryType = type;

	pushValue(value);
	return value.id;
}

IROperand IRGenerator::makeIntConstant(TypeKind type)
{
	//TacValue constant;
	//constant.kind = ValueKind::Constant;
	//constant.temporaryType = type;
	//constant.id = newID();

	IROperand constant;
	constant.kind = ValueKind::Constant;
	constant.type = type;

	if (SemanticUtils::isSigned(constant.type))
		constant.data.emplace<int64_t>(1);
	else
		constant.data.emplace<uint64_t>(1);

	//pushValue(constant);
	return constant;
}

IROperand IRGenerator::makeNullOperand()
{
	IROperand operand;
	operand.id = 0;
	operand.kind = ValueKind::None;
	operand.type = TypeKind::Unknown;
	return operand;
}

IROperand IRGenerator::makeTemporaryOperand(TypeKind type)
{
	IROperand operand;
	operand.kind = ValueKind::Temporary;
	operand.type = type;

	uint32_t id = makeTemporary(type);
	operand.id = id;
	return operand;
}

IROperand IRGenerator::makeLabelOperand()
{
	IROperand label;
	label.kind = ValueKind::Label;
	label.id = newLabelID();
	return label;
}

void IRGenerator::emitInstruction(const TacInstruction& instruction)
{
	// if we're currently in a function, reroute instructions to the function body
	// if we're not in a function, then somethings gone horribly wrong, but it's not possible to not be in a function so we're good
	if (currentFunction)
		currentFunction->body.emplace_back(instruction);
}

void IRGenerator::emitShortCircuitOr(ASTLogical& node, IROperand trueLabel, IROperand falseLabel)
{
	IROperand rhsLabel = makeLabelOperand();
	ASTExpr* lhs = ASTUtils::unwrapGroupExpr(node.lhs);
	ASTExpr* rhs = ASTUtils::unwrapGroupExpr(node.rhs);

	if (lhs->shortCircuitable)
	{
		ASTLogical& lhsExpr = static_cast<ASTLogical&>(*lhs);
		if (lhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(lhsExpr, trueLabel, rhsLabel);
		else
			emitShortCircuitAnd(lhsExpr, trueLabel, rhsLabel);
	}
	else
	{
		IROperand conditionID = node.lhs->accept(*this);
		emitInstruction({ TacOpcode::JmpIfTrue, trueLabel, conditionID, nullOperand });
	}

	emitInstruction({ TacOpcode::Label, rhsLabel, nullOperand, nullOperand });
	
	if (rhs->shortCircuitable)
	{
		ASTLogical& rhsExpr = static_cast<ASTLogical&>(*rhs);
		if (rhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(rhsExpr, trueLabel, falseLabel);
		else
			emitShortCircuitAnd(rhsExpr, trueLabel, falseLabel);
	}
	else
	{
		IROperand conditionID = node.rhs->accept(*this);
		emitInstruction({ TacOpcode::JmpIfFalse, falseLabel, conditionID, nullOperand });
	}
}

void IRGenerator::emitShortCircuitAnd(ASTLogical& node, IROperand trueLabel, IROperand falseLabel)
{
	IROperand rhsLabel = makeLabelOperand();
	ASTExpr* lhs = ASTUtils::unwrapGroupExpr(node.lhs);
	ASTExpr* rhs = ASTUtils::unwrapGroupExpr(node.rhs);

	if (lhs->shortCircuitable)
	{
		ASTLogical& lhsExpr = static_cast<ASTLogical&>(*lhs);
		if (lhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(lhsExpr, rhsLabel, falseLabel);
		else
			emitShortCircuitAnd(lhsExpr, rhsLabel, falseLabel);
	}
	else
	{
		IROperand conditionID = node.lhs->accept(*this);
		emitInstruction({ TacOpcode::JmpIfFalse, falseLabel, conditionID, nullOperand });
	}

	emitInstruction({ TacOpcode::Label, rhsLabel, nullOperand, nullOperand });

	if (rhs->shortCircuitable)
	{
		ASTLogical& rhsExpr = static_cast<ASTLogical&>(*rhs);
		if (rhsExpr.logicalOperator.type == TokenType::LogicalOr)
			emitShortCircuitOr(rhsExpr, trueLabel, falseLabel);
		else
			emitShortCircuitAnd(rhsExpr, trueLabel, falseLabel);
	}
	else
	{
		IROperand conditionID = node.rhs->accept(*this);
		emitInstruction({ TacOpcode::JmpIfFalse, falseLabel, conditionID, nullOperand });
	}
}

IROperand IRGenerator::emitArrayIndexCalculation(ASTExpr* base)
{
	std::vector<ASTArrayAccess*> accesses;
	ASTExpr* curr = base;

	while (ASTArrayAccess* access = dynamic_cast<ASTArrayAccess*>(ASTUtils::unwrapGroupExpr(curr)))
	{
		accesses.push_back(access);
		curr = access->arrayExpr;
	}

	LValue baseLValue = ASTUtils::unwrapGroupExpr(accesses.back()->arrayExpr)->getLValue();
	IROperand offset = makeTemporaryOperand(TypeKind::u64);
	for (size_t i = 0; i < accesses.size(); i++)
	{
		IROperand index = accesses[i]->indexExpr->accept(*this);
		IROperand result = makeTemporaryOperand(TypeKind::u64);
		//uint32_t stride = newID();
		//TacValue strideVal;
		IROperand stride;
		stride.type = TypeKind::u64;
		stride.kind = ValueKind::Constant;
		stride.data.emplace<uint64_t>(accesses[i]->arrayExpr->typeInfo->slotCountPerElement);
		
		emitInstruction({ TacOpcode::Constant, stride, nullOperand, nullOperand });
		emitInstruction({ TacOpcode::Mul, result, stride, index });

		// for 1-d array access, the offset is just stride * index
		if (i == 0)
			emitInstruction({ TacOpcode::Move, offset, result, nullOperand });

		// for 2-d and up array accesses, we need to accumulate each (stride_n * index) calculation and store it in offset
		if (i != 0)
			emitInstruction({ TacOpcode::Add, offset, offset, result });
	}

	return offset;
}

IROperand IRGenerator::visitIntLiteral(ASTIntLiteral& node)
{
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;

	IROperand constant;
	constant.kind = ValueKind::Constant;
	constant.type = typeInfo->type;
	
	switch (typeInfo->type)
	{
		case TypeKind::u8:
		case TypeKind::u16:
		case TypeKind::u32:
		case TypeKind::u64:
			constant.data.emplace<uint64_t>(node.value);
			break;
		case TypeKind::i8:
		case TypeKind::i16:
		case TypeKind::i32:
		case TypeKind::i64:
			constant.data.emplace<int64_t>(static_cast<int64_t>(node.value));
			break;
	}

	//pushValue(value);
	//emitInstruction({ TacOpcode::Constant, constant, nullOperand, nullOperand });
	return constant;
}

IROperand IRGenerator::visitDoubleLiteral(ASTDoubleLiteral& node)
{
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;

	IROperand constant;
	constant.kind = ValueKind::Constant;
	constant.type = typeInfo->type;

	switch (typeInfo->type)
	{
		case TypeKind::f32:
		case TypeKind::f64:
			constant.data.emplace<double>(node.value);
			break;
	}

	//pushValue(value);
	//emitInstruction({ TacOpcode::Constant, nullOperand , constant, nullOperand });
	return constant;
}

IROperand IRGenerator::visitCharLiteral(ASTCharLiteral& node)
{
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;

	IROperand constant;
	constant.kind = ValueKind::Constant;
	constant.data.emplace<char>(node.value);
	constant.type = typeInfo->type;

	//pushValue(value);
	//emitInstruction({ TacOpcode::Constant, nullOperand, constant, nullOperand });
	return constant;
}

IROperand IRGenerator::visitBoolLiteral(ASTBoolLiteral& node)
{
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;

	IROperand constant;
	constant.kind = ValueKind::Constant;
	constant.data.emplace<bool>(node.value);
	constant.type = TypeKind::Bool;

	//pushValue(value);
	//emitInstruction({ TacOpcode::Constant, nullOperand, constant, nullOperand });
	return constant;
}

IROperand IRGenerator::visitVarDecl(ASTVarDecl& node)
{
	IROperand initID = node.initialization ? node.initialization->accept(*this) : nullOperand;

	IROperand var;
	var.kind = ValueKind::Variable;
	var.id = newID();
	//uint32_t id = newID();
	const std::string& identifier = std::get<std::string>(node.varIdentifier.value);
	IRSymbol* sym = env.set(identifier, var.id);

	TacValue value;
	value.kind = ValueKind::Variable;
	value.id = var.id;
	value.typeInfo = node.symbol->typeInfo;
	value.data.emplace<std::string>(identifier);
	pushValue(value);

	std::cout << "Var decl with id = " << var.id << std::endl;
	if (node.initialization)
		emitInstruction({ TacOpcode::Store, var, initID, nullOperand });

	if (currentFunction)
		currentFunction->localIDs.push_back(var.id);

	return initID;
}

IROperand IRGenerator::visitFuncDecl(ASTFuncDecl& node)
{
	IRFunction* function = funcArena.alloc<IRFunction>();
	function->name = std::get<std::string>(node.funcIdentifier.value);
	function->funcID = newFuncID();
	function->returnType = node.typeInfo->returnType;
	irData.functionTable.push_back(function);
	irData.funcToId[function->name] = function->funcID;

	currentFunction = function;
	env.pushScope();
	node.params->accept(*this);
	node.body->accept(*this);
	env.popScope();
	currentFunction = nullptr;

	// dont think a return is needed
	return makeNullOperand();
}

IROperand IRGenerator::visitIdentifier(ASTIdentifier& node)
{
	uint32_t identifierID = env.get(std::get<std::string>(node.identifier.value))->id;
	IROperand identifierOperand;
	identifierOperand.id = identifierID;
	identifierOperand.kind = irData.values[identifierID].kind;
	identifierOperand.type = node.typeInfo->type;

	if (getEvalMode() == EvalMode::RValue)
	{
		// uint32_t tempID = makeTemporary(node.typeInfo->type);
		IROperand tempOperand = makeTemporaryOperand(node.typeInfo->type);

		emitInstruction({ TacOpcode::Load, tempOperand, identifierOperand, nullOperand });
		return tempOperand;
	}

	return identifierOperand;
}

IROperand IRGenerator::visitExprStmt(ASTExprStmt& node)
{
	return node.expression->accept(*this);
}

IROperand IRGenerator::visitAssign(ASTAssign& node)
{
	// x = 10;
	// need Assign Op -> dest = assigneeID, src1 = valueID
	// TODO: add support for all the compound assignment operators, +=, -=, &=, etc.
	pushEvalMode(EvalMode::RValue);
	IROperand valueID = node.value->accept(*this);
	popEvalMode();

	pushEvalMode(EvalMode::LValue);
	IROperand assigneeID = node.assignee->accept(*this);
	popEvalMode();

	TacValue value;
	value.typeInfo = node.typeInfo;
	//value.kind = ValueKind::Variable;
	value.kind = irData.values[assigneeID.id].kind;
	value.id = assigneeID.id;
	setValue(assigneeID.id, value);

	emitInstruction({ TacOpcode::Store, assigneeID, valueID, nullOperand });
	return valueID;
	//return assigneeID;
}

IROperand IRGenerator::visitReturn(ASTReturn& node)
{
	pushEvalMode(EvalMode::RValue);
	IROperand returnID = node.returnVal->accept(*this);
	popEvalMode();

	emitInstruction({ TacOpcode::Return, returnID, nullOperand, nullOperand });
	return returnID;
}

IROperand IRGenerator::visitBlock(ASTBlock& node)
{
	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);

	return nullOperand;
}

IROperand IRGenerator::visitForLoop(ASTForLoop& node)
{
	// for loop executes -> initializer once, run condition, run body, run increment, jmp to condition
	IRConditionContext ctx;
	IROperand conditionLabel = makeLabelOperand();
	ctx.trueLabel = makeLabelOperand();
	ctx.falseLabel = makeLabelOperand();
	conditionCtxStack.emplace_back(ctx);

	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
	{
		shortCircuitCtx = true;
		emitInstruction({ TacOpcode::Label, conditionLabel, nullOperand, nullOperand });
		IROperand conditionID = node.condition->accept(*this);
		shortCircuitCtx = false;

		if (!node.condition->shortCircuitable)
			emitInstruction({ TacOpcode::JmpIfFalse, conditionCtxStack.back().falseLabel, conditionID, nullOperand });
	}

	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().trueLabel, nullOperand, nullOperand });
	node.body->accept(*this);

	if (node.increment)
		node.increment->accept(*this);

	emitInstruction({ TacOpcode::Jmp, conditionLabel, nullOperand, nullOperand });
	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().falseLabel, nullOperand, nullOperand });
	
	conditionCtxStack.pop_back();

	return nullOperand;
}

IROperand IRGenerator::visitWhileLoop(ASTWhileLoop& node)
{
	// while loop executes -> condition, body, jmp condition
	IRConditionContext ctx;
	IROperand conditionLabel = makeLabelOperand();
	ctx.trueLabel = makeLabelOperand();
	ctx.falseLabel = makeLabelOperand();
	conditionCtxStack.emplace_back(ctx);

	shortCircuitCtx = true;
	emitInstruction({ TacOpcode::Label, conditionLabel, nullOperand, nullOperand });
	IROperand conditionID = node.condition->accept(*this);
	shortCircuitCtx = false;

	if (!node.condition->shortCircuitable)
		emitInstruction({ TacOpcode::JmpIfFalse, conditionCtxStack.back().falseLabel, conditionID, nullOperand });

	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().trueLabel, nullOperand, nullOperand });
	node.body->accept(*this);
	emitInstruction({ TacOpcode::Jmp, conditionLabel, nullOperand, nullOperand });
	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().falseLabel, nullOperand, nullOperand });

	return nullOperand;
}

IROperand IRGenerator::visitIfStatement(ASTIfStatement& node)
{
	IRConditionContext ctx;
	ctx.trueLabel = makeLabelOperand();
	ctx.falseLabel = makeLabelOperand();
	ctx.endLabel = makeLabelOperand();

	conditionCtxStack.emplace_back(ctx);
	shortCircuitCtx = true;
	IROperand conditionID = node.condition->accept(*this);
	shortCircuitCtx = false;

	// might be a hack, watch out for this line. But essentially, if the condition isn't short circuitable
	// we need to emit a jmp just in case the condition is false. Non short circuitable expressions are simple things
	// like if (true) or if (false) etc
	if (!node.condition->shortCircuitable)
		emitInstruction({ TacOpcode::JmpIfFalse, conditionCtxStack.back().falseLabel, conditionID, nullOperand });

	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().trueLabel, nullOperand, nullOperand });
	node.trueBranch->accept(*this);

	// if there's a false branch, we need to emit an uncondition jump over it in the event the true branch was taken
	// we emit the false branch label after the jmp instruction
	if (node.falseBranch)
	{
		emitInstruction({ TacOpcode::Jmp, conditionCtxStack.back().endLabel, nullOperand, nullOperand });
		emitInstruction({ TacOpcode::Label, conditionCtxStack.back().falseLabel, nullOperand, nullOperand });
		node.falseBranch->accept(*this);
	}

	// if there's no false branch, we still emit the false label but use it to function as the 'end' of the if statement
	else
	{
		emitInstruction({ TacOpcode::Label, conditionCtxStack.back().falseLabel, nullOperand, nullOperand });
	}

	emitInstruction({ TacOpcode::Label, conditionCtxStack.back().endLabel, nullOperand, nullOperand });

	conditionCtxStack.pop_back();
	return nullOperand;
}

IROperand IRGenerator::visitLogical(ASTLogical& node)
{
	//uint32_t result = makeTemporary(TypeKind::Bool);

	if (shortCircuitCtx)
	{
		if (node.logicalOperator.type == TokenType::LogicalAnd)
			emitShortCircuitAnd(node, conditionCtxStack.back().trueLabel, conditionCtxStack.back().falseLabel);
		else
			emitShortCircuitOr(node, conditionCtxStack.back().trueLabel, conditionCtxStack.back().falseLabel);
	}
	else
	{
		// x || y, x && y
		IROperand result = makeTemporaryOperand(TypeKind::Bool);

		IROperand trueLabel = makeLabelOperand();
		IROperand falseLabel = makeLabelOperand();
		IROperand endLabel = makeLabelOperand();

		if (node.logicalOperator.type == TokenType::LogicalAnd)
			emitShortCircuitAnd(node, trueLabel, falseLabel);
		else
			emitShortCircuitOr(node, trueLabel, falseLabel);

		// emit labels to jump to, these labels will just store the result in the flag and return it
		IROperand value;
		value.kind = ValueKind::Constant;
		value.data.emplace<bool>(true);
		value.type = TypeKind::Bool;
		emitInstruction({ TacOpcode::Label, trueLabel, nullOperand, nullOperand });
		emitInstruction({ TacOpcode::Move, result, value, nullOperand });
		emitInstruction({ TacOpcode::Jmp, endLabel, nullOperand, nullOperand });
		
		value.data.emplace<bool>(false);
		emitInstruction({ TacOpcode::Label, falseLabel, nullOperand, nullOperand });
		emitInstruction({ TacOpcode::Move, result, value, nullOperand });

		emitInstruction({ TacOpcode::Label, endLabel, nullOperand, nullOperand });

		return result;

	}

	//return result;
	return nullOperand;
}

IROperand IRGenerator::visitBinaryExpr(ASTBinaryExpr& node)
{
	pushEvalMode(EvalMode::RValue);
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;
	IROperand lhsID = node.lhs->accept(*this);
	IROperand rhsID = node.rhs->accept(*this);
	popEvalMode();

	//if (node.lhs->isLValue())
	//{
	//	uint32_t lhsTemporary = makeTemporary(typeInfo->type);
	//	emitInstruction({ TacOpcode::Load, lhsTemporary, lhsID , 0 });
	//	lhsID = lhsTemporary;
	//}

	//if (node.rhs->isLValue())
	//{
	//	uint32_t rhsTemporary = makeTemporary(typeInfo->type);
	//	emitInstruction({ TacOpcode::Load, rhsTemporary, rhsID, 0 });
	//	rhsID = rhsTemporary;
	//}

	//uint32_t resultID = newID();
	TacOpcode op = TacOpcode::None;

	switch (node.op.type)
	{
		case TokenType::Plus: 
			op = TacOpcode::Add; 
			break;
		case TokenType::Minus:
			op = TacOpcode::Sub;
			break;
		case TokenType::Asterisk:
			op = TacOpcode::Mul;
			break;
		case TokenType::ForwardSlash:
			op = TacOpcode::Div;
			break;
		case TokenType::BitwiseAnd:
			op = TacOpcode::BitAnd;
			break;
		case TokenType::BitwiseOr:
			op = TacOpcode::BitOr;
			break;
		case TokenType::BitwiseXor:
			op = TacOpcode::BitXor;
			break;
		case TokenType::BitwiseLeftShift:
			op = TacOpcode::BitShiftLeft;
			break;
		case TokenType::BitwiseRightShift:
			op = TacOpcode::BitShiftRight;
			break;
		case TokenType::NotEq:
			op = TacOpcode::NotEq;
			break;
		case TokenType::Equality:
			op = TacOpcode::Eq;
			break;
		case TokenType::GreaterThan:
			op = TacOpcode::Gt;
			break;
		case TokenType::GreaterThanEq:
			op = TacOpcode::Gte;
			break;
		case TokenType::LessThan:
			op = TacOpcode::Lt;
			break;
		case TokenType::LessThanEq:
			op = TacOpcode::Lte;
			break;
	}

	//TacValue value;
	//value.id = resultID;
	//value.kind = ValueKind::Temporary;
	//value.temporaryType = node.typeInfo->type;
	//pushValue(value);
	//uint32_t resultID = makeTemporary(typeInfo->type);
	IROperand result = makeTemporaryOperand(typeInfo->type);
	emitInstruction({ op, result, lhsID, rhsID });
	return result;
}

IROperand IRGenerator::visitUnaryExpr(ASTUnaryExpr& node)
{
	// !x, --x, ++x, ~x, and -x
	TypeInfo* typeInfo = implicitCastCtx ? implicitCastCtx : node.typeInfo;

	// TODO: duplicated code, can be fixed up but not too big of a deal, I can refactor it later, just worried about it working correctly for now
	switch (node.op.type)
	{
		case TokenType::Increment:
		{
			pushEvalMode(EvalMode::LValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();

			IROperand initialVal = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Load, initialVal, lValueID, nullOperand });

			IROperand constantID = makeIntConstant(typeInfo->type);
			emitInstruction({ TacOpcode::Constant, constantID, nullOperand, nullOperand });

			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Add, result, initialVal, constantID });
			emitInstruction({ TacOpcode::Store, lValueID, result, nullOperand });
			return result;
		}
		case TokenType::Decrement:
		{
			pushEvalMode(EvalMode::LValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();

			IROperand initialVal = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Load, initialVal, lValueID, nullOperand });

			IROperand constantID = makeIntConstant(typeInfo->type);
			emitInstruction({ TacOpcode::Constant, constantID, nullOperand, nullOperand });

			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Sub, result, initialVal, constantID });
			emitInstruction({ TacOpcode::Store, lValueID, result, nullOperand });
			return result;
		}
		case TokenType::LogicalNot:
		{
			// return !x
			pushEvalMode(EvalMode::RValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();
			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Not, result, lValueID, nullOperand });
			return result;
		}
		case TokenType::BitwiseNot:
		{
			// return ~x
			pushEvalMode(EvalMode::RValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();
			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::BitNot, result, lValueID, nullOperand });
			return result;
		}
		case TokenType::Minus:
		{
			// return -x
			pushEvalMode(EvalMode::RValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();
			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Neg, result, lValueID, nullOperand });
			return result;
		}
		case TokenType::BitwiseAnd:
		{
			// return &x
			pushEvalMode(EvalMode::LValue);
			IROperand lValueID = node.expr->accept(*this);
			popEvalMode();
			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Address, result, lValueID, nullOperand });
			return result;
		}
		case TokenType::Asterisk:
		{
			// return *x
			// if eval mode is LValue, we just return the address of the pointer
			IROperand lValueID = node.expr->accept(*this);
			if (getEvalMode() == EvalMode::LValue)
			{
				IROperand dereferenced = makeTemporaryOperand(node.expr->typeInfo->ptrBaseType->type);
				emitInstruction({ TacOpcode::Load, dereferenced, lValueID, nullOperand });
				return dereferenced;
			}

			IROperand result = makeTemporaryOperand(typeInfo->type);
			emitInstruction({ TacOpcode::Dereference, result, lValueID, nullOperand });
			return result;
		}
	}

	return nullOperand;
}

IROperand IRGenerator::visitCall(ASTCall& node)
{
	// we want to visit the arg list -> emit code for args
	// node.callee.typeinfo.name -> func name, use it to get the func index
	// store result of the function call and return it
	pushEvalMode(EvalMode::RValue);
	node.args->accept(*this);
	popEvalMode();
	std::string& funcIdentifier = node.callee->typeInfo->name;

	IROperand funcOperand;
	funcOperand.kind = ValueKind::FuncID;
	funcOperand.id = irData.funcToId[funcIdentifier];

	if (node.typeInfo->type == TypeKind::Void)
	{
		emitInstruction({ TacOpcode::CallVoid, funcOperand, nullOperand, nullOperand });
		return nullOperand;
	}
	else
	{
		IROperand returnID = makeTemporaryOperand(node.typeInfo->type);
		emitInstruction({ TacOpcode::Call, returnID, funcOperand, nullOperand });
		return returnID;
	}
}

IROperand IRGenerator::visitGroupExpr(ASTGroupExpr& node)
{
	return node.expr->accept(*this);
}

IROperand IRGenerator::visitPostfix(ASTPostfix& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;

	// x
	IROperand lValueID = node.expr->accept(*this);

	// t_n = x
	IROperand tempID = makeTemporaryOperand(type);
	emitInstruction({ TacOpcode::Move, tempID, lValueID, nullOperand });

	// 1
	IROperand constantID = makeIntConstant(type);
	emitInstruction({ TacOpcode::Constant, constantID, nullOperand, nullOperand });

	if (node.op.type == TokenType::Increment)
	{
		// result = x + 1
		IROperand result = makeTemporaryOperand(type);
		emitInstruction({ TacOpcode::Add, result, lValueID, constantID });
		emitInstruction({ TacOpcode::Store, lValueID, result, nullOperand });
	}
	else if (node.op.type == TokenType::Decrement)
	{
		// result = x - 1
		IROperand result = makeTemporaryOperand(type);
		emitInstruction({ TacOpcode::Sub, result, lValueID, constantID });
		emitInstruction({ TacOpcode::Store, lValueID, result, nullOperand });
	}

	return tempID;
}

IROperand IRGenerator::visitParameter(ASTParameter& node)
{
	TypeInfo* type = implicitCastCtx ? implicitCastCtx : node.typeInfo;
	uint32_t id = newID();
	const std::string& identifier = std::get<std::string>(node.paramIdentifier.value);
	env.set(identifier, id);

	TacValue value;
	value.kind = ValueKind::Param;
	value.id = id;
	value.typeInfo = type;
	value.data.emplace<std::string>(identifier);
	pushValue(value);
	
	IROperand param;
	param.kind = ValueKind::Param;
	param.id = id;
	return param;
}

IROperand IRGenerator::visitArgument(ASTArgument& node)
{
	IROperand arg = node.value->accept(*this);
	emitInstruction({ TacOpcode::FuncArg, arg, nullOperand, nullOperand });
	return arg;
}

IROperand IRGenerator::visitParamList(ASTParamList& node)
{
	// need to treat each param as variable declarations for the current scope (the function's scope)
	for (size_t i = 0; i < node.params.size(); i++)
	{
		IROperand paramID = visitParameter(*node.params[i]);
		currentFunction->paramIDs.push_back(paramID.id);
	}

	return nullOperand;
}

IROperand IRGenerator::visitArgList(ASTArgList& node)
{
	for (size_t i = 0; i < node.args.size(); i++)
	{
		visitArgument(*node.args[i]);
	}

	return nullOperand;
}

IROperand IRGenerator::visitArrayAccess(ASTArrayAccess& node)
{
	TypeKind type = implicitCastCtx ? implicitCastCtx->type : node.typeInfo->type;
	IROperand offset = emitArrayIndexCalculation(&node);
	IROperand result = makeTemporaryOperand(type);
	LValue baseLValue = ASTUtils::unwrapGroupExpr(node.arrayExpr)->getLValue();

	// i think this will break as soon as an array is not a directly addressable variable like in a struct maybe idk
	// or if a function returns an array or something
	uint32_t arrID = env.get(baseLValue.symbol->typeInfo->name)->id;
	IROperand arrOperand;
	arrOperand.id = arrID;

	emitInstruction({ TacOpcode::LoadArrElem, result, arrOperand, offset});

	return result;
}

IROperand IRGenerator::visitCast(ASTCast& node)
{
	implicitCastCtx = node.typeInfo;
	IROperand operand = node.expr->accept(*this);
	implicitCastCtx = nullptr;
	return operand;
}

IROperand IRGenerator::visitStructDecl(ASTStructDecl& node)
{
	return nullOperand;
}

IROperand IRGenerator::visitFieldAccess(ASTFieldAccess& node)
{
	pushEvalMode(EvalMode::LValue);
	IROperand structID = node.structExpr->accept(*this);
	popEvalMode();

	TypeEntry* type = typeTable->lookupType(node.structExpr->typeInfo->typeName);
	const StructField& field = type->structFields[std::get<std::string>(node.fieldIdentifier.value)];

	//uint32_t fieldOffsetID = newID();
	//TacValue fieldOffset;
	//fieldOffset.id = fieldOffsetID;
	//fieldOffset.temporaryType = TypeKind::u64;
	//fieldOffset.kind = ValueKind::Constant;
	//fieldOffset.data.emplace<uint64_t>(field.offset);
	//pushValue(fieldOffset);
	IROperand fieldOffset;
	fieldOffset.kind = ValueKind::Constant;
	fieldOffset.type = TypeKind::u64;
	fieldOffset.data.emplace<uint64_t>(field.offset);

	IROperand fieldID = makeTemporaryOperand(field.typeInfo->type);
	emitInstruction({ TacOpcode::FieldAddr, fieldID, structID, fieldOffset });

	if (getEvalMode() == EvalMode::RValue)
	{
		IROperand result = makeTemporaryOperand(node.typeInfo->type);
		emitInstruction({ TacOpcode::Dereference, result, fieldID, nullOperand });
		return result;
	}

	return fieldID;
}