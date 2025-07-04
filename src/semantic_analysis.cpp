#include "semantic_analysis.h"
#include "debug_utils.h"
#include "semantic_utils.h"

#include <iostream>
#include <limits>

SemanticAnalysis::SemanticAnalysis()
{
	this->diagnosticReporter = nullptr;
	this->typeArena = nullptr;
	this->env = nullptr;
}

void SemanticAnalysis::printInfo()
{
	env->dumpEnvironment();
}

int SemanticAnalysis::getGlobalVarCount()
{
	SymbolTable* globalScope = env->getGlobalScope();
	return globalScope->getSlotOffset();
}

void SemanticAnalysis::checkForMainFunction()
{
	SymbolTable* globalScope = env->getGlobalScope();

	if (!globalScope->isDefined("main"))
	{
		diagnosticReporter->reportDiagnostic("main function not found", DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, 0, 0);
		return;
	}
	
	Symbol* main = globalScope->getSymbol("main");
	if (main->typeInfo->type != TypeKind::Function)
	{
		diagnosticReporter->reportDiagnostic("identifier \"main\" found, but it is not a function", DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, 0, 0);
		return;
	}

	if (main->typeInfo->returnType->type != TypeKind::i32)
	{
		diagnosticReporter->reportDiagnostic("main function must return i32", DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, 0, 0);
		return;
	}
}

void SemanticAnalysis::analyze(std::vector<ASTNode*>& ast, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter, Environment* env)
{
	this->diagnosticReporter = diagnosticReporter;
	this->typeArena = typeArena;
	this->env = env;

	for (ASTNode* node : ast)
		node->accept(*this);

	checkForMainFunction();
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
	// we don't need to lookup the symbol here, since the symbol resolution pass guarantees that this variable is unique
	// if the node has an initialization, visit it and collect type data on it, type data for this var decl node was collected during symbol resolution
	if (node.initialization)
		node.initialization->accept(*this);
}

void SemanticAnalysis::visitFuncDecl(ASTFuncDecl& node)
{
	// enter inner function scope
	env->pushScope(node.scope, ScopeKind::Function);

	// set the semantic analysis functionCtx to the current ASTFuncDecl's type information
	functionCtxStack.push_back(node.typeInfo);
	visitParamList(*node.params);

	// when visiting the body, ASTBlock should not create a scope since the functionDecl node itself is responsible for creating the scope
	node.body->createScope = false;
	visitBlock(*node.body);
	
	// after visiting param list / body / whatever other functionCtx related information, reset the ctx back to nullptr
	functionCtxStack.pop_back();
	//env->dumpEnvironment();
	env->popScope();
}

void SemanticAnalysis::visitIdentifier(ASTIdentifier& node)
{
	// variable symbols are guaranteed to be valid because of symbol resolution, but function identifiers are not.
	// this is because out of order function calls are allowed, so function calls are not checked during symbol resolution
	// so we need to check the symbol here
	std::string& identifier = std::get<std::string>(node.identifier.value);
	Symbol* symbol = env->findSymbol(identifier);

	if (!symbol)
	{
		// if the symbol is not found, set type to Unknown (This will only occur when the user tries to call an undefined function)
		std::string message = "undefined identifier \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::Semantic, node.line, node.col);
		node.typeInfo = typeArena->alloc<TypeInfo>();
		node.typeInfo->type = TypeKind::Unknown;
		return;
	} 

	node.typeInfo = symbol->typeInfo;
	node.slotIndex = symbol->slotIndex;
	node.scope = symbol->scope;
}

void SemanticAnalysis::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void SemanticAnalysis::visitAssign(ASTAssign& node)
{
	node.assignee->accept(*this);
	node.typeInfo = node.assignee->typeInfo;
	std::string identifier = node.typeInfo->name;
	Symbol* symbol = env->findSymbol(identifier);

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
	node.scope = symbol->scope;
}

void SemanticAnalysis::visitReturn(ASTReturn& node)
{
	// if there's no functionCtx, then we're visiting a return statement outside of a function body which is not allowed
	if (functionCtxStack.size() == 0)
	{
		diagnosticReporter->reportDiagnostic("return statements must be inside function body", DiagnosticLevel::Error, DiagnosticType::ReturnOutsideFunction, ErrorPhase::Semantic, node.line, node.col);
		return;
	}

	// if functionCtx is valid, we need to resolve the type of the return value
	if (node.returnVal)
		node.returnVal->accept(*this);
}

void SemanticAnalysis::visitBlock(ASTBlock& node)
{
	if (node.createScope)
		env->pushScope(node.scope, ScopeKind::Normal);

	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);

	//env->dumpEnvironment();
	if (node.createScope)
		env->popScope();
}

void SemanticAnalysis::visitForLoop(ASTForLoop& node)
{
	env->pushScope(node.scope, ScopeKind::Normal);
	
	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
		node.condition->accept(*this);

	if (node.increment)
		node.increment->accept(*this);

	node.body->createScope = false;
	visitBlock(*node.body);

	//env->dumpEnvironment();
	env->popScope();
}

void SemanticAnalysis::visitWhileLoop(ASTWhileLoop& node)
{
	node.condition->accept(*this);
	visitBlock(*node.body);
}

void SemanticAnalysis::visitIfStatement(ASTIfStatement& node)
{
	// condition and true branch are required to be in an if statement
	node.condition->accept(*this);
	node.trueBranch->accept(*this);

	// check if there is a false branch, since they're not required
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

	// default to the lhs type, the type checker will resolve the type of this node and the two operands
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = node.lhs->typeInfo->type;
}

void SemanticAnalysis::visitUnaryExpr(ASTUnaryExpr& node)
{
	node.expr->accept(*this);
	node.typeInfo = typeArena->alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Unknown;
}

void SemanticAnalysis::visitCall(ASTCall& node)
{
	// get type info for the callee and make sure it's a function
	node.callee->accept(*this);

	// make sure the type isnt unknown, undefined functions will set the callee's type info to Unknown to indicate an error
	if (node.callee->typeInfo->type == TypeKind::Unknown)
		return;

	// set function ctx, and verify the arguments passed in match the function signature
	functionCtxStack.push_back(node.callee->typeInfo);
	visitArgList(*node.args);

	// lastly, the ASTCall node type info should be the return type of the function call
	node.typeInfo = functionCtxStack.back()->returnType;
	functionCtxStack.pop_back();
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
}

void SemanticAnalysis::visitParameter(ASTParameter& node)
{
	
}

void SemanticAnalysis::visitArgument(ASTArgument& node)
{
	node.value->accept(*this);
	node.typeInfo = node.value->typeInfo;
}

void SemanticAnalysis::visitParamList(ASTParamList& node)
{
	
}

void SemanticAnalysis::visitArgList(ASTArgList& node)
{
	// if functionCtx is set, make sure ArgList size matches the expected parameter count as the function signature
	// we also need to make sure the type of each arg corresponds to the type of each expected parameter
	if (functionCtxStack.size() > 0)
	{
		if (!functionCtxStack.back())
			std::cout << "BACK OF FUNCTION CTX IS NULLPTR\n";
		std::cout << "visiting arg list, paramTypes.size = " << functionCtxStack.back()->paramTypes.size();
		if (node.args.size() != functionCtxStack.back()->paramTypes.size())
		{
			std::string message = "mismatched function argument count, expected ";
			message += std::to_string(functionCtxStack.back()->paramTypes.size());
			message += ", but got ";
			message += std::to_string(node.args.size());
			diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::IncompatibleArgCount, ErrorPhase::Semantic, node.line, node.col);
			return;
		}
		
		for (size_t i = 0; i < node.args.size(); i++)
		{
			// get type info for the arg
			node.args[i]->accept(*this);

			// i dont think this line should be here, we should leave it to the type checker to make sure the type is convertible or matches exactly
			// not directly set it to the parameters type
			//node.args[i]->typeInfo->type = functionCtxStack.back()->paramTypes[i]->type;
		}
	}
}