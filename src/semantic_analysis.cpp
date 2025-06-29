#include "semantic_analysis.h"
#include "debug_utils.h"
#include "semantic_utils.h"

#include <iostream>
#include <limits>

SemanticAnalysis::SemanticAnalysis()
{
	this->diagnosticReporter = nullptr;
	this->typeArena = nullptr;
}

void SemanticAnalysis::printInfo()
{
	env.dumpEnvironment();
}

int SemanticAnalysis::getGlobalVarCount()
{
	SymbolTable& globalScope = env.getGlobalScope();
	return globalScope.getSlotOffset();
}

void SemanticAnalysis::analyze(std::vector<ASTNode*>& ast, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter)
{
	this->diagnosticReporter = diagnosticReporter;
	this->typeArena = typeArena;

	for (ASTNode* node : ast)
		node->accept(*this);
}

void SemanticAnalysis::visitIntLiteral(ASTIntLiteral& node)
{
	// idea is to have int literals default to the smallest container possible that fits the value
	// and assignment / variable declarations can widen the type if needed
	node.typeInfo = typeArena->alloc<TypeInfo>();
	if (node.value <= std::numeric_limits<int8_t>::max())
		node.typeInfo->type = TypeKind::i8;
	else if (node.value <= std::numeric_limits<int16_t>::max())
		node.typeInfo->type = TypeKind::i16;
	else if (node.value <= std::numeric_limits<int32_t>::max())
		node.typeInfo->type = TypeKind::i32;
	else
		node.typeInfo->type = TypeKind::i64;
}

void SemanticAnalysis::visitDoubleLiteral(ASTDoubleLiteral& node)
{
	node.typeInfo = typeArena->alloc<TypeInfo>();
	float castedFloat = static_cast<float>(node.value);
	double castedBack = static_cast<double>(castedFloat);

	if (std::fabs(node.value - castedBack) <= (std::numeric_limits<float>::epsilon() * std::fabs(node.value)))
		node.typeInfo->type = TypeKind::f32;
	else
		node.typeInfo->type = TypeKind::f64;
}

void SemanticAnalysis::visitCharLiteral(ASTCharLiteral& node)
{
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Char;
}

void SemanticAnalysis::visitBoolLiteral(ASTBoolLiteral& node)
{
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Bool;
}

void SemanticAnalysis::visitVarDecl(ASTVarDecl& node)
{
	SymbolTable& table = env.getCurrentScope();
	std::string& identifier = std::get<std::string>(node.varIdentifier.value);
	node.typeInfo = typeArena->alloc<TypeInfo>();

	// error, defining a variable that is already defined
	if (table.isDefined(identifier))
	{
		std::string message = "cannot define variables with the same name: \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::Semantic, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
		return;
	}

	node.typeInfo->name = identifier;
	node.typeInfo->type = SemanticUtils::getTypeFromToken(node.varType);
	
	// if the node has an initialization, make sure the type of this variable is compatible with the type of the initialization expression
	if (node.initialization)
	{
		// now we know what type is declared for the variable, and what type was inferred for the initialization. Now we need to check
		// if they are compatible
		node.initialization->accept(*this);

		//Diagnostic ctx;
		//if (!SemanticUtils::isAssignable(node.typeInfo->type, node.initialization->typeInfo->type, ctx))
		//{
		//	std::string message = "incompatible type for \"" + identifier + "\", expected ";
		//	message += DebugUtils::typeKindToString(node.typeInfo->type);
		//	message += ", but got ";
		//	message += DebugUtils::typeKindToString(node.initialization->typeInfo->type);
		//	reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleType, node.line, node.col);
		//	node.typeInfo->type = TypeKind::Unknown;
		//	return;
		//}

		//if (ctx.level != DiagnosticLevel::None)
		//	reportDiagnostic(ctx.message, ctx.level, ctx.type, node.line, node.col);

		// at this point we know the types of the variable and initializer are compatible, however literals always try to fit into the smallest
		// type they can, so if we have x : i32 = 10, 10 will default to i8, but we can safely widen to match the variable i32 which is what this checks
		//if ((SemanticUtils::isInteger(node.typeInfo->type) && (SemanticUtils::getIntegerWidth(node.initialization->typeInfo->type) < SemanticUtils::getIntegerWidth(node.typeInfo->type)))
		//	|| (SemanticUtils::isFloat(node.typeInfo->type) && (SemanticUtils::getFloatWidth(node.initialization->typeInfo->type) < SemanticUtils::getFloatWidth(node.typeInfo->type))))
		//{
		//	ASTExpr* initializer = node.initialization;
		//	ASTCast* implicitCast = nodeArena->alloc<ASTCast>(initializer);
		//	implicitCast->typeInfo = typeArena->alloc<TypeInfo>();
		//	implicitCast->typeInfo->type = node.typeInfo->type;
		//	node.initialization = implicitCast;
		//}
	}

	node.scope = env.getScopeDepth();
	node.slotIndex = table.declareVar(identifier, node.typeInfo, node.scope);
}

void SemanticAnalysis::visitFuncDecl(ASTFuncDecl& node)
{
	SymbolTable& table = env.getCurrentScope();
	std::string& funcIdentifier = std::get<std::string>(node.funcIdentifier.value);
	node.typeInfo = typeArena->alloc<TypeInfo>();

	if (table.isDefined(funcIdentifier))
	{
		std::string message = "cannot define multiple functions with the same name \"" + funcIdentifier +"\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::Semantic, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
		return;
	}

	node.typeInfo->returnType = typeArena->alloc<TypeInfo>();
	node.typeInfo->name = funcIdentifier;
	node.typeInfo->type = TypeKind::Function;
	node.typeInfo->returnType->type = SemanticUtils::getTypeFromToken(node.returnType.type);

	// declare function in outer scope
	table.declareFunc(funcIdentifier, node.typeInfo, env.getScopeDepth());

	// enter inner function scope
	env.pushScope(ScopeKind::Function);
	SymbolTable& funcScope = env.getCurrentScope();

	// set the semantic analysis functionCtx to the current ASTFuncDecl's type information
	//functionCtx = node.typeInfo;
	functionCtxStack.push_back(node.typeInfo);
	visitParamList(*node.params);

	// when visiting the body, ASTBlock should not create a scope since the functionDecl node itself is responsible for creating the scope
	node.body->createScope = false;
	visitBlock(*node.body);
	
	// after visiting param list / body / whatever other functionCtx related information, reset the ctx back to nullptr
	//functionCtx = nullptr;
	functionCtxStack.pop_back();
	env.dumpEnvironment();
	env.popScope();
}

void SemanticAnalysis::visitIdentifier(ASTIdentifier& node)
{
	// try to find the symbol in the environment
	std::string& identifier = std::get<std::string>(node.identifier.value);
	Symbol* symbol = env.findSymbol(identifier);
	
	// if we find the symbol, set this nodes type info to the symbols type info, otherwise set it to unknown
	if (symbol)
	{
		// this line feels like a bug, since symbol tables are deleted after semantic analysis, unless I make them persistent later no
		// but the code breaks if i change it
		node.typeInfo = symbol->typeInfo; 
		node.slotIndex = symbol->slotIndex;
		node.scope = symbol->scope;
	}
	else
	{
		std::string message = "undefined identifier \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, node.line, node.col);
		node.typeInfo = typeArena->alloc<TypeInfo>();
		node.typeInfo->type = TypeKind::Unknown;
	}
}

void SemanticAnalysis::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void SemanticAnalysis::visitAssign(ASTAssign& node)
{
	/*
	get type info for value 
	make sure its compatible with the identifier
	
	assignment operators are:
	=, +=, -=, *=, /=, ^=, &=, |=, <<=, >>=

	*/

	//std::string& identifier = std::get<std::string>(node.identifier.value);
	node.assignee->accept(*this);
	node.typeInfo = node.assignee->typeInfo;
	std::string identifier = node.typeInfo->name;
	Symbol* symbol = env.findSymbol(identifier);

	if (!symbol)
	{
		std::string message = "undefined identifier \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, node.line, node.col);
		node.typeInfo = typeArena->alloc<TypeInfo>();
		node.typeInfo->type = TypeKind::Unknown;
		return;
	}

	node.value->accept(*this);
	node.typeInfo = symbol->typeInfo;
	node.slotIndex = symbol->slotIndex;
}

void SemanticAnalysis::visitReturn(ASTReturn& node)
{
	// if there's no functionCtx, then we're visiting a return statement outside of a function body which is not allowed
	if (functionCtxStack.size() == 0)
	{
		diagnosticReporter->reportDiagnostic("return statements must be inside function body", DiagnosticLevel::Error, DiagnosticType::ReturnOutsideFunction, ErrorPhase::Semantic, node.line, node.col);
		return;
	}

	// if there's no return val, that means the function returns void, check to make sure the signature returns void
	//if (!node.returnVal)
	//{
	//	if (functionCtx->returnType->type != TypeKind::Void)
	//		reportDiagnostic("missing return statement from function expecting non-void return type", DiagnosticLevel::Error, DiagnosticType::IncompatibleReturnType, node.line, node.col);

	//	return;
	//}

	// if functionCtx is valid, we need to resolve the type of the return value and make sure it matches the function signatures return type
	if (node.returnVal)
		node.returnVal->accept(*this);

	//std::cout << DebugUtils::typeKindToString(functionCtx->returnType->type) << std::endl;
	//std::cout << DebugUtils::typeKindToString(node.returnVal->typeInfo->type) << std::endl;
	//Diagnostic ctx;
	//if (!SemanticUtils::isAssignable(functionCtx->returnType->type, node.returnVal->typeInfo->type, ctx))
	//{
	//	std::string message = "incompatible function return type, expected ";
	//	message += DebugUtils::typeKindToString(functionCtx->returnType->type);
	//	message += " or convertible type, but got ";
	//	message += DebugUtils::typeKindToString(node.returnVal->typeInfo->type);
	//	reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleReturnType, node.line, node.col);
	//	return;
	//}

	//if (ctx.level != DiagnosticLevel::None)
	//	reportDiagnostic(ctx.message, ctx.level, ctx.type, node.line, node.col);
}

void SemanticAnalysis::visitBlock(ASTBlock& node)
{
	if (node.createScope)
		env.pushScope(ScopeKind::Normal);

	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);

	env.dumpEnvironment();
	if (node.createScope)
		env.popScope();
}

void SemanticAnalysis::visitForLoop(ASTForLoop& node)
{
	env.pushScope(ScopeKind::Normal);
	
	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
	{
		node.condition->accept(*this);
		//if (node.condition->typeInfo->type != TypeKind::Bool)
		//	reportDiagnostic("for loop condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, node.line, node.col);
	}

	if (node.increment)
		node.increment->accept(*this);

	node.body->createScope = false;
	visitBlock(*node.body);

	env.dumpEnvironment();
	env.popScope();
}

void SemanticAnalysis::visitWhileLoop(ASTWhileLoop& node)
{
	node.condition->accept(*this);
	//if (node.condition->typeInfo->type != TypeKind::Bool)
	//	reportDiagnostic("while loop condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, node.line, node.col);

	visitBlock(*node.body);
}

void SemanticAnalysis::visitIfStatement(ASTIfStatement& node)
{
	node.condition->accept(*this);
	//if (node.condition->typeInfo->type != TypeKind::Bool)
	//{
	//	reportDiagnostic("if statement condition must be of type bool", DiagnosticLevel::Error, DiagnosticType::IncompatibleType, node.line, node.col);
	//	return;
	//}

	node.trueBranch->accept(*this);
	if (node.falseBranch)
		node.falseBranch->accept(*this);
}

void SemanticAnalysis::visitLogical(ASTLogical& node)
{
	node.lhs->accept(*this);
	node.rhs->accept(*this);

	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Bool;
}

void SemanticAnalysis::visitBinaryExpr(ASTBinaryExpr& node)
{
	// get type information for the two operands
	node.lhs->accept(*this);
	node.rhs->accept(*this);

	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = node.lhs->typeInfo->type;	// unknown type till type checking phase since type depends on operands
	/*if (SemanticUtils::isComparisonOp(node.op.type))
	{
		if (SemanticUtils::tryComparison(*node.lhs->typeInfo, *node.rhs->typeInfo))
			node.typeInfo->type = TypeKind::Bool;
		else
		{
			std::string message = "incompatible types ";
			message += DebugUtils::typeKindToString(node.lhs->typeInfo->type);
			message += " and ";
			message += DebugUtils::typeKindToString(node.rhs->typeInfo->type);
			message += " for comparison operation";
			reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
		}
	}
	else if (SemanticUtils::isArithmeticOp(node.op.type))
	{
		if (SemanticUtils::tryArithmetic(*node.lhs->typeInfo, *node.rhs->typeInfo))
			node.typeInfo->type = node.lhs->typeInfo->type;
		else
		{
			std::string message = "incompatible types ";
			message += DebugUtils::typeKindToString(node.lhs->typeInfo->type);
			message += " and ";
			message += DebugUtils::typeKindToString(node.rhs->typeInfo->type);
			message += " for arithmetic operation";
			reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
		}
	}
	else if (SemanticUtils::isBitwiseOp(node.op.type))
	{
		if (SemanticUtils::tryBitwise(*node.lhs->typeInfo, *node.rhs->typeInfo))
			node.typeInfo->type = node.lhs->typeInfo->type;
		else
		{
			std::string message = "incompatible types ";
			message += DebugUtils::typeKindToString(node.lhs->typeInfo->type);
			message += " and ";
			message += DebugUtils::typeKindToString(node.rhs->typeInfo->type);
			message += " for bitwise operation";
			reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
			node.typeInfo->type = TypeKind::Unknown;
		}
	}*/
}

void SemanticAnalysis::visitUnaryExpr(ASTUnaryExpr& node)
{
	node.expr->accept(*this);
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Unknown;

	// '!' is only defined for bool types
	//if (node.op.type == TokenType::LogicalNot)
	//{
	//	if (node.expr->typeInfo->type != TypeKind::Bool)
	//	{
	//		reportDiagnostic("operator \"!\" can only be applied to bool types", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
	//		node.typeInfo->type = TypeKind::Unknown;
	//		return;
	//	}

	//	node.typeInfo->type = TypeKind::Bool;
	//}

	//// infix '++', '--', '~' is only defined for integer or unsigned integer types
	//else if (node.op.type == TokenType::Increment || node.op.type == TokenType::Decrement || node.op.type == TokenType::BitwiseNot)
	//{
	//	if (!SemanticUtils::isInteger(node.expr->typeInfo->type))
	//	{
	//		std::string message = "operator ";
	//		message += DebugUtils::tokenTypeToString(node.op);
	//		message += "can only be applied to unsigned or signed integers";
	//		reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
	//		node.typeInfo->type = TypeKind::Unknown;
	//		return;
	//	}

	//	node.typeInfo->type = node.expr->typeInfo->type;
	//}

	//else if (node.op.type == TokenType::Minus)
	//{
	//	if (!SemanticUtils::isNumeric(node.expr->typeInfo->type))
	//	{
	//		reportDiagnostic("unary '-' can only be applied to numeric types", DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
	//		node.typeInfo->type = TypeKind::Unknown;
	//		return;
	//	}

	//	node.typeInfo->type = node.expr->typeInfo->type;
	//}
	//else
	//{
	//	reportDiagnostic("unknown unary operator", DiagnosticLevel::Error, DiagnosticType::UnknownOperator, node.line, node.col);
	//	node.typeInfo->type = TypeKind::Unknown;
	//}
}

void SemanticAnalysis::visitCall(ASTCall& node)
{
	// get type info for the callee and make sure it's a function
	node.callee->accept(*this);
	//if (node.callee->typeInfo->type != TypeKind::Function)
	//{
	//	reportDiagnostic("could not resolve function call", DiagnosticLevel::Error, DiagnosticType::UnresolvedFunctionCall, node.line, node.col);
	//	node.typeInfo = typeArena->alloc<TypeInfo>();
	//	node.typeInfo->type = TypeKind::Unknown;
	//	return;
	//}

	// set function ctx, and verify the arguments passed in match the function signature
	//functionCtx = node.callee->typeInfo;
	functionCtxStack.push_back(node.callee->typeInfo);
	visitArgList(*node.args);

	// lastly, the ASTCall node type info should be the return type of the function call
	node.typeInfo = functionCtxStack.back()->returnType;
	functionCtxStack.pop_back();
	//functionCtx = nullptr;
}

void SemanticAnalysis::visitGroupExpr(ASTGroupExpr& node)
{
	node.expr->accept(*this);
	node.typeInfo = node.expr->typeInfo;
}

void SemanticAnalysis::visitPostfix(ASTPostfix& node)
{
	node.expr->accept(*this);
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Unknown;

	//if (node.op.type == TokenType::Increment || node.op.type == TokenType::Decrement)
	//{
	//	if (!SemanticUtils::isInteger(node.expr->typeInfo->type))
	//	{
	//		std::string message = "postix ";
	//		message += DebugUtils::tokenTypeToString(node.op);
	//		message += " can only be applied to unsigned or signed integers";
	//		reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleOperands, node.line, node.col);
	//		node.typeInfo->type = TypeKind::Unknown;
	//		return;
	//	}

	//	node.typeInfo->type = node.expr->typeInfo->type;
	//}
	//else
	//{
	//	reportDiagnostic("unknown postfix operator", DiagnosticLevel::Error, DiagnosticType::UnknownOperator, node.line, node.col);
	//	node.typeInfo->type = TypeKind::Unknown;
	//}
}

void SemanticAnalysis::visitParameter(ASTParameter& node)
{
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->name = std::get<std::string>(node.paramIdentifier.value);
	node.typeInfo->type = SemanticUtils::getTypeFromToken(node.paramType.type);
}

void SemanticAnalysis::visitArgument(ASTArgument& node)
{
	node.value->accept(*this);
	node.typeInfo = node.value->typeInfo;
}

void SemanticAnalysis::visitParamList(ASTParamList& node)
{
	// if functionCtx is a valid pointer, we know this parameter list belongs to a function declaration
	if (functionCtxStack.size() > 0)
	{
		SymbolTable& table = env.getCurrentScope();
		for (ASTParameter* param : node.params)
		{
			visitParameter(*param);
			std::string& paramIdentifier = param->typeInfo->name;
			
			if (table.isDefined(paramIdentifier))
			{
				std::string message = "duplicate parameter name \"" + paramIdentifier + "\" in function \"" + functionCtxStack.back()->name + "\"()";
				diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::Semantic, node.line, node.col);
			}

			table.declareVar(paramIdentifier, param->typeInfo, env.getScopeDepth());
			functionCtxStack.back()->paramTypes.emplace_back(param->typeInfo);
		}
	}
}

void SemanticAnalysis::visitArgList(ASTArgList& node)
{
	// if functionCtx is set, make sure ArgList size matches the expected parameter count as the function signature
	// we also need to make sure the type of each arg corresponds to the type of each expected parameter
	if (functionCtxStack.size() > 0)
	{
		//if (node.args.size() != functionCtx->paramTypes.size())
		//{
		//	std::string message = "mismatched function argument count, expected ";
		//	message += functionCtx->paramTypes.size();
		//	message += ", but got ";
		//	message += node.args.size();
		//	reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleArgCount, node.line, node.col);
		//	return;
		//}
		
		for (size_t i = 0; i < node.args.size(); i++)
		{
			//Diagnostic ctx;
			node.args[i]->accept(*this);
			//if (!SemanticUtils::isAssignable(functionCtx->paramTypes[i]->type, node.args[i]->typeInfo->type, ctx))
			//{
			//	std::string message = "expected argument type ";
			//	message += DebugUtils::typeKindToString(functionCtx->paramTypes[i]->type);
			//	message += ", but got type ";
			//	message += DebugUtils::typeKindToString(node.args[i]->typeInfo->type);
			//	reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleType, node.args[i]->line, node.args[i]->col);
			//	continue;
			//}

			//if (ctx.level != DiagnosticLevel::None)
			//	reportDiagnostic(ctx.message, ctx.level, ctx.type, node.args[i]->line, node.args[i]->col);

			// set the args type to the expected parameter type if it's compatible with the declared parameter type
			node.args[i]->typeInfo->type = functionCtxStack.back()->paramTypes[i]->type;
		}
	}
}

void SemanticAnalysis::visitCast(ASTCast& node)
{
	// should be empty i think, casts are only placed in the AST for implicit casts right now, when explicit casts are added
	// i think some code should go here
}