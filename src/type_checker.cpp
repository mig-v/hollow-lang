#include "type_checker.h"
#include "semantic_utils.h"
#include "debug_utils.h"

TypeChecker::TypeChecker()
{
	this->nodeArena = nullptr;
	this->typeArena = nullptr;
	this->diagnosticReporter = nullptr;
	this->typeConversions = nullptr;
}

TypeChecker::~TypeChecker()
{
	delete typeConversions;
}

bool TypeChecker::conversionIsLegal(const ConversionInfo& conversion, ASTNode* nodeCtx)
{
	switch (conversion.type)
	{
		case ConversionType::SameType:
			return true;
		case ConversionType::Widening:
			return true;
		case ConversionType::Narrowing:
			diagnosticReporter->reportDiagnostic(conversion.message, DiagnosticLevel::Warning, DiagnosticType::NarrowingConversion, ErrorPhase::TypeChecker, nodeCtx->line, nodeCtx->col);
			return true;
		case ConversionType::ExplicitOnly:
			diagnosticReporter->reportDiagnostic(conversion.message, DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, nodeCtx->line, nodeCtx->col);
			return false;
		case ConversionType::NotAllowed:
			diagnosticReporter->reportDiagnostic(conversion.message, DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, nodeCtx->line, nodeCtx->col);
			return false;
	}
}

void TypeChecker::typeCheck(std::vector<ASTNode*>& ast, MemoryArena* nodeArena, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter)
{
	this->nodeArena = nodeArena;
	this->typeArena = typeArena;
	this->diagnosticReporter = diagnosticReporter;
	this->typeConversions = new TypeConversions();

	for (ASTNode* node : ast)
		node->accept(*this);
}

bool TypeChecker::typesLegalForComparison(TypeKind lhs, TypeKind rhs)
{
	return ((lhs == rhs) || (SemanticUtils::isNumeric(lhs) && SemanticUtils::isNumeric(rhs)));
}

bool TypeChecker::typesLegalForArithmetic(TypeKind lhs, TypeKind rhs)
{
	return ((lhs == rhs) || (SemanticUtils::isNumeric(lhs) && SemanticUtils::isNumeric(rhs)));
}

bool TypeChecker::typesLegalForBitwise(TypeKind lhs, TypeKind rhs)
{
	// bitwise operations disallowed for any type that is not a signed or unsigned integer
	if (!SemanticUtils::isInteger(lhs) || !SemanticUtils::isInteger(rhs))
		return false;

	// bitwise operations disallowed for opposite signed integers 
	if ((SemanticUtils::isSigned(lhs) && SemanticUtils::isUnsigned(rhs)) || (SemanticUtils::isUnsigned(lhs) && SemanticUtils::isSigned(rhs)))
		return false;
}

LValue TypeChecker::isLValue(ASTExpr* expr)
{
	LValue lValue;
	ASTExpr* unwrappedExpr = unwrapExpr(expr);
	if (ASTIdentifier* var = dynamic_cast<ASTIdentifier*>(unwrappedExpr))
	{
		lValue.kind = LValueKind::Slot;
		lValue.slotIndex = var->slotIndex;
	}

	return lValue;
}

ASTExpr* TypeChecker::unwrapExpr(ASTExpr* expr)
{
	// unwrap expr if expr is a group expr until we reach the underlying expr, that was a lot of expr's
	while (ASTGroupExpr* groupExpr = dynamic_cast<ASTGroupExpr*>(expr))
	{
		expr = groupExpr->expr;
	}

	return expr;
}

void TypeChecker::unifyBinaryOperands(ASTBinaryExpr& node, bool isComparisonOp)
{
	// need to find a common unified type (for now im just finding the largest bit width type)
	int lhsWidth = (SemanticUtils::isInteger(node.lhs->typeInfo->type)) ? SemanticUtils::getIntegerWidth(node.lhs->typeInfo->type) : SemanticUtils::getFloatWidth(node.lhs->typeInfo->type);
	int rhsWidth = (SemanticUtils::isInteger(node.rhs->typeInfo->type)) ? SemanticUtils::getIntegerWidth(node.rhs->typeInfo->type) : SemanticUtils::getFloatWidth(node.rhs->typeInfo->type);

	TypeKind commonType;
	ConversionInfo conversion;
	ASTExpr** castee;

	if (lhsWidth >= rhsWidth)
	{
		commonType = node.lhs->typeInfo->type;
		conversion = typeConversions->getConversion(node.rhs->typeInfo->type, commonType);
		castee = &node.rhs;
	}
	else
	{
		commonType = node.rhs->typeInfo->type;
		conversion = typeConversions->getConversion(node.lhs->typeInfo->type, commonType);
		castee = &node.lhs;
	}

	// if the conversion is illegal report it and return
	if (!conversionIsLegal(conversion, &node))
	{
		std::string message = "incompatible types ";
		message += DebugUtils::typeKindToString(node.lhs->typeInfo->type);
		message += " and ";
		message += DebugUtils::typeKindToString(node.rhs->typeInfo->type);
		message += " for binary operation";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
		return;
	}

	// comparison operations are the only time the binary expressions type is explicitly set to bool
	if (isComparisonOp)
		node.typeInfo->type = TypeKind::Bool;
	else
		node.typeInfo->type = commonType;

	// if conversion is between same types, no implicit cast needed, just set the binary expr nodes type info and return
	if (conversion.type == ConversionType::SameType)
		return;

	// otherwise, the operands are compatible, we just need to insert an implicit cast to unify their types
	ASTCast* implicitCast = nodeArena->alloc<ASTCast>(*castee);
	implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
	implicitCast->typeInfo->type = commonType;
	*castee = implicitCast;
}



void TypeChecker::visitIntLiteral(ASTIntLiteral& node)
{
	// nothing the typechecker needs to do
}

void TypeChecker::visitDoubleLiteral(ASTDoubleLiteral& node) 
{
	// nothing the typechecker needs to do
}

void TypeChecker::visitCharLiteral(ASTCharLiteral& node)
{
	// nothing the typechecker needs to do
}

void TypeChecker::visitBoolLiteral(ASTBoolLiteral& node)
{
	// nothing the typechecker needs to do
}

void TypeChecker::visitVarDecl(ASTVarDecl& node)
{
	if (!node.initialization)
		return;

	node.initialization->accept(*this);
	std::string& identifier = std::get<std::string>(node.varIdentifier.value);
	ConversionInfo conversion = typeConversions->getConversion(node.initialization->typeInfo->type, node.typeInfo->type);

	// if the types of initializer and var decl are the same or illegal, return since there's no more work to be done
	if (conversion.type == ConversionType::SameType || !conversionIsLegal(conversion, &node))
		return;

	// otherwise, an implicit cast is required
	ASTExpr* initializer = node.initialization;
	ASTCast* implicitCast = nodeArena->alloc<ASTCast>(initializer);
	implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
	implicitCast->typeInfo->type = node.typeInfo->type;
	node.initialization = implicitCast;
}

void TypeChecker::visitFuncDecl(ASTFuncDecl& node)
{
	//functionCtx = node.typeInfo;
	functionCtxStack.push_back(node.typeInfo);
	node.params->accept(*this);
	node.body->accept(*this);
	functionCtxStack.pop_back();
	//functionCtx = nullptr;
}

void TypeChecker::visitIdentifier(ASTIdentifier& node)
{
	// nothing to do i think, type info already set during semantic analysis
}

void TypeChecker::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void TypeChecker::visitAssign(ASTAssign& node)
{
	node.value->accept(*this);
	node.assignee->accept(*this);

	if (node.op.type == TokenType::Assign
		|| (SemanticUtils::isArithmeticAssignment(node.op.type) && (SemanticUtils::isNumeric(node.typeInfo->type) && SemanticUtils::isNumeric(node.assignee->typeInfo->type)))
		|| (SemanticUtils::isBitwiseAssignment(node.op.type) && (SemanticUtils::isInteger(node.typeInfo->type) && SemanticUtils::isNumeric(node.assignee->typeInfo->type))))
	{
		ConversionInfo conversion = typeConversions->getConversion(node.value->typeInfo->type, node.assignee->typeInfo->type);

		if (conversion.type == ConversionType::SameType || !conversionIsLegal(conversion, &node))
			return;

		// otherwise, an implicit cast is required
		ASTExpr* expr = node.value;
		ASTCast* implicitCast = nodeArena->alloc<ASTCast>(expr);
		implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
		implicitCast->typeInfo->type = node.assignee->typeInfo->type;
		node.value = implicitCast;
	}
}

void TypeChecker::visitReturn(ASTReturn& node)
{
	if (functionCtxStack.size() == 0)
		return;

	// if there's no return val, that means the function returns void, check to make sure the signature returns void
	if (!node.returnVal)
	{
		if (functionCtxStack.back()->returnType->type != TypeKind::Void)
			diagnosticReporter->reportDiagnostic("missing return statement from function expecting non-void return type", DiagnosticLevel::Error, DiagnosticType::IncompatibleReturnType, ErrorPhase::TypeChecker, node.line, node.col);

		return;
	}

	node.returnVal->accept(*this);
	ConversionInfo conversion = typeConversions->getConversion(node.returnVal->typeInfo->type, functionCtxStack.back()->returnType->type);

	// if the types of return type and function declarred return type are the same or illegal, return since there's no more work to be done
	if (conversion.type == ConversionType::SameType || !conversionIsLegal(conversion, &node))
		return;

	// otherwise, an implicit cast is required
	ASTExpr* expr = node.returnVal;
	ASTCast* implicitCast = nodeArena->alloc<ASTCast>(expr);
	implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
	implicitCast->typeInfo->type = functionCtxStack.back()->returnType->type;
	node.returnVal = implicitCast;
}

void TypeChecker::visitBlock(ASTBlock& node)
{
	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);
}

void TypeChecker::visitForLoop(ASTForLoop& node)
{
	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
	{
		node.condition->accept(*this);
		if (node.condition->typeInfo->type != TypeKind::Bool)
			diagnosticReporter->reportDiagnostic("for loop condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, node.line, node.col);
	}

	if (node.increment)
		node.increment->accept(*this);

	node.body->accept(*this);
}

void TypeChecker::visitWhileLoop(ASTWhileLoop& node)
{
	node.condition->accept(*this);
	
	if (node.condition->typeInfo->type != TypeKind::Bool)
		diagnosticReporter->reportDiagnostic("while loop condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, node.line, node.col);

	node.body->accept(*this);
}

void TypeChecker::visitIfStatement(ASTIfStatement& node)
{
	node.condition->accept(*this);
	
	if (node.condition->typeInfo->type != TypeKind::Bool)
		diagnosticReporter->reportDiagnostic("if statement condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, node.line, node.col);

	node.trueBranch->accept(*this);

	if (node.falseBranch)
		node.falseBranch->accept(*this);
}

void TypeChecker::visitLogical(ASTLogical& node)
{
	node.lhs->accept(*this);
	node.rhs->accept(*this);

	if (node.lhs->typeInfo->type != TypeKind::Bool || node.rhs->typeInfo->type != TypeKind::Bool)
		diagnosticReporter->reportDiagnostic("logical expression operands must both be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
}

void TypeChecker::visitBinaryExpr(ASTBinaryExpr& node)
{
	// both operands have their types set during semantic analysis so I dont think i need to visit them
	// check for the operator, then check to make sure the types are defined for that operation
	// find a common unified type for both operands, e.g the widest type
	// so i8 + i32 -> i32 is common type, u16 + i64 -> i64 is common type
	// insert implicit cast node for operands if needed
	// set the binaryExpr type info to the common type

	node.lhs->accept(*this);
	node.rhs->accept(*this);

	if (SemanticUtils::isComparisonOp(node.op.type) && typesLegalForComparison(node.lhs->typeInfo->type, node.rhs->typeInfo->type))
		unifyBinaryOperands(node, true);
	else if (SemanticUtils::isArithmeticOp(node.op.type) && typesLegalForArithmetic(node.lhs->typeInfo->type, node.rhs->typeInfo->type))
		unifyBinaryOperands(node, false);
	else if (SemanticUtils::isBitwiseOp(node.op.type) && typesLegalForBitwise(node.lhs->typeInfo->type, node.rhs->typeInfo->type))
		unifyBinaryOperands(node, false);
}

void TypeChecker::visitUnaryExpr(ASTUnaryExpr& node)
{
	node.expr->accept(*this);
	
	// '!' is only defined for bool types
	if (node.op.type == TokenType::LogicalNot)
	{
		if (node.expr->typeInfo->type != TypeKind::Bool)
		{
			diagnosticReporter->reportDiagnostic("operator \"!\" can only be applied to bool types", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}

		node.typeInfo->type = TypeKind::Bool;
	}

	//infix '++', '--', '~' is only defined for integer or unsigned integer types
	else if (node.op.type == TokenType::Increment || node.op.type == TokenType::Decrement)
	{
		LValue lValue = isLValue(node.expr);
		if (lValue.kind == LValueKind::Invalid)
		{
			diagnosticReporter->reportDiagnostic("postfix operand must be modifiable l-value", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}
		else if (!SemanticUtils::isInteger(node.expr->typeInfo->type))
		{
			std::string message = "postfix ";
			message += DebugUtils::tokenTypeToString(node.op);
			message += " can only be applied to unsigned or signed integers";
			diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}

		if (lValue.kind == LValueKind::Slot)
			node.slotIndex = lValue.slotIndex;

		node.typeInfo->type = node.expr->typeInfo->type;
	}

	else if (node.op.type == TokenType::BitwiseNot)
	{
		if (!SemanticUtils::isInteger(node.expr->typeInfo->type))
		{
			std::string message = "operator ";
			message += DebugUtils::tokenTypeToString(node.op);
			message += " can only be applied to unsigned or signed integers";
			diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}

		node.typeInfo->type = node.expr->typeInfo->type;
	}

	else if (node.op.type == TokenType::Minus)
	{
		if (!SemanticUtils::isNumeric(node.expr->typeInfo->type))
		{
			diagnosticReporter->reportDiagnostic("unary '-' can only be applied to numeric types", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}

		node.typeInfo->type = node.expr->typeInfo->type;
	}
	else
	{
		diagnosticReporter->reportDiagnostic("unknown unary operator", DiagnosticLevel::Error, DiagnosticType::UnknownOperator, ErrorPhase::TypeChecker, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
	}
}

void TypeChecker::visitCall(ASTCall& node)
{
	node.callee->accept(*this);

	if (node.callee->typeInfo->type != TypeKind::Function)
	{
		diagnosticReporter->reportDiagnostic("expression is not callable, must be function type", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, ErrorPhase::TypeChecker, node.line, node.col);
		return;
	}

	//functionCtx = node.callee->typeInfo;
	functionCtxStack.push_back(node.callee->typeInfo);
	node.args->accept(*this);
	functionCtxStack.pop_back();
	//functionCtx = nullptr;
}

void TypeChecker::visitGroupExpr(ASTGroupExpr& node)
{
	node.expr->accept(*this);
}

void TypeChecker::visitPostfix(ASTPostfix& node)
{
	node.expr->accept(*this);

	if (node.op.type == TokenType::Increment || node.op.type == TokenType::Decrement)
	{
		LValue lValue = isLValue(node.expr);
		if (lValue.kind == LValueKind::Invalid)
		{
			diagnosticReporter->reportDiagnostic("postfix operand must be modifiable l-value", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}
		else if (!SemanticUtils::isInteger(node.expr->typeInfo->type))
		{
			std::string message = "postfix ";
			message += DebugUtils::tokenTypeToString(node.op);
			message += " can only be applied to unsigned or signed integers";
			diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, ErrorPhase::TypeChecker, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
			return;
		}

		if (lValue.kind == LValueKind::Slot)
			node.slotIndex = lValue.slotIndex;
		
		node.typeInfo->type = node.expr->typeInfo->type;
	}
	else
	{
		diagnosticReporter->reportDiagnostic("unknown postfix operator", DiagnosticLevel::Error, DiagnosticType::UnknownOperator, ErrorPhase::TypeChecker, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
	}
}

void TypeChecker::visitParameter(ASTParameter& node)
{
	// dont need to do much in here yet
}

void TypeChecker::visitArgument(ASTArgument& node)
{
	node.value->accept(*this);
}

void TypeChecker::visitParamList(ASTParamList& node)
{
	// dont need to do much in here yet
}

void TypeChecker::visitArgList(ASTArgList& node) 
{
	if (functionCtxStack.size() == 0)
		return;
	
	if (node.args.size() != functionCtxStack.back()->paramTypes.size())
	{
		std::string message = "mismatched function argument count, expected ";
		message += std::to_string(functionCtxStack.back()->paramTypes.size());
		message += " but got ";
		message += std::to_string(node.args.size());
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleArgCount, ErrorPhase::TypeChecker, node.line, node.col);
		return;
	}

	for (size_t i = 0; i < node.args.size(); i++)
	{
		node.args[i]->accept(*this);
		
		ConversionInfo conversion = typeConversions->getConversion(node.args[i]->typeInfo->type, functionCtxStack.back()->paramTypes[i]->type);

		// if the arg is convertible or the same type to the expected param type, continue to next iteration
		if (conversion.type == ConversionType::SameType || !conversionIsLegal(conversion, &node))
			continue;

		// otherwise, an implicit cast is required
		ASTExpr* expr = node.args[i]->value;
		ASTCast* implicitCast = nodeArena->alloc<ASTCast>(expr);
		implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
		implicitCast->typeInfo->type = functionCtxStack.back()->paramTypes[i]->type;
		node.args[i]->value = implicitCast;
	}
}

void TypeChecker::visitCast(ASTCast& node) 
{

}
