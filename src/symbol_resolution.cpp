#include "symbol_resolution.h"
#include "semantic_utils.h"

SymbolResolution::SymbolResolution()
{
	this->diagnosticReporter = nullptr;
	this->functionCtx = nullptr;

	SymbolTable* globalScope = symbolArena.alloc<SymbolTable>();
	this->env = new Environment(globalScope);
}

SymbolResolution::~SymbolResolution()
{
	delete env;
}

void SymbolResolution::resolve(std::vector<ASTNode*>& ast, DiagnosticReporter* diagnosticReporter)
{
	this->diagnosticReporter = diagnosticReporter;
	
	for (ASTNode* node : ast)
		node->accept(*this);

	env->dumpEnvironment();
}

void SymbolResolution::visitFuncDecl(ASTFuncDecl& node)
{
	SymbolTable* table = env->getCurrentScope();
	std::string& funcIdentifier = std::get<std::string>(node.funcIdentifier.value);
	node.typeInfo = typeArena.alloc<TypeInfo>();

	// make sure no duplicate symbols have been declared with the same name
	if (table->isDefined(funcIdentifier))
	{
		std::string message = "cannot define multiple functions with the same name \"" + funcIdentifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::SymbolResolution, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
		return;
	}

	// fill in type info for the function
	node.typeInfo->returnType = typeArena.alloc<TypeInfo>();
	node.typeInfo->name = funcIdentifier;
	node.typeInfo->type = TypeKind::Function;
	node.typeInfo->returnType->type = SemanticUtils::getTypeFromToken(node.returnType.type);

	// declare function in outer scope
	table->declareFunc(funcIdentifier, node.typeInfo, env->getScopeDepth());

	// push the functions scope, we have to push it before the function body, and disable the function body's ability to create its own scope
	// because we need to contain the function's parameter list in the same scope as the function body
	SymbolTable* funcScope = symbolArena.alloc<SymbolTable>();
	node.scope = funcScope;
	env->pushScope(funcScope, ScopeKind::Function);

	// set the function context before visiting the functions parameter list
	functionCtx = node.typeInfo;
	visitParamList(*node.params);

	node.body->createScope = false;
	visitBlock(*node.body);
	functionCtx = nullptr;

	env->popScope();
}

void SymbolResolution::visitVarDecl(ASTVarDecl& node)
{
	SymbolTable* table = env->getCurrentScope();
	std::string& identifier = std::get<std::string>(node.varIdentifier.value);
	node.typeInfo = typeArena.alloc<TypeInfo>();


	// error, defining a variable that is already defined
	if (table->isDefined(identifier))
	{
		std::string message = "cannot define variables with the same name: \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::SymbolResolution, node.line, node.col);
		node.typeInfo->type = TypeKind::Unknown;
		return;
	}

	node.typeInfo->name = identifier;
	node.typeInfo->type = SemanticUtils::getTypeFromToken(node.varType);

	if (node.initialization)
		node.initialization->accept(*this);

	// get the scope and slot index for the varaiable and add it into the symbol table
	node.scope = env->getScopeDepth();
	node.slotIndex = table->declareVar(identifier, node.typeInfo, node.scope);
}

void SymbolResolution::visitIdentifier(ASTIdentifier& node)
{
	std::string& identifier = std::get<std::string>(node.identifier.value);
	Symbol* symbol = env->findSymbol(identifier);

	if (!symbol)
	{
		std::string message = "undefined identifier \"" + identifier + "\"";
		diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::SymbolResolution, node.line, node.col);
	}
}

void SymbolResolution::visitParamList(ASTParamList& node)
{
	if (!functionCtx)
	{
		diagnosticReporter->reportDiagnostic("cannot have parameter list outside of function declaration", DiagnosticLevel::Error, DiagnosticType::UndefinedIdentifier, ErrorPhase::SymbolResolution, node.line, node.col);
		return;
	}

	SymbolTable* table = env->getCurrentScope();
	int scopeDepth = env->getScopeDepth();
	for (ASTParameter* param : node.params)
	{
		visitParameter(*param);
		std::string& identifier = param->typeInfo->name;

		// check if the parameter identifier is already used in the function declaration
		if (table->isDefined(identifier))
		{
			std::string message = "duplicate parameter name \"" + identifier + "\" in function \"" + functionCtx->name + "\"()";
			diagnosticReporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::DuplicateIdentifier, ErrorPhase::SymbolResolution, node.line, node.col);
		}
		else
		{
			table->declareVar(identifier, param->typeInfo, scopeDepth);
			functionCtx->paramTypes.emplace_back(param->typeInfo);
		}
	}
}

void SymbolResolution::visitParameter(ASTParameter& node)
{
	node.typeInfo = typeArena.alloc<TypeInfo>();
	node.typeInfo->name = std::get<std::string>(node.paramIdentifier.value);
	node.typeInfo->type = SemanticUtils::getTypeFromToken(node.paramType.type);
}

void SymbolResolution::visitWhileLoop(ASTWhileLoop& node)
{
	// even though a while loop isnt a declaration, we still need to visit any node that may contain symbol references / declarations
	// and a while loop can contain identifiers in its condition like while (x > 0), and var declarations in its body
	node.condition->accept(*this);
	node.body->accept(*this);
}

void SymbolResolution::visitIfStatement(ASTIfStatement& node)
{
	node.condition->accept(*this);
	node.trueBranch->accept(*this);

	if (node.falseBranch)
		node.falseBranch->accept(*this);
}

void SymbolResolution::visitCall(ASTCall& node)
{
	// dont visit the callee, since we're not verifying the function exists during symbol resolution because I want to allow
	// the ability to call functions that haven't been declared yet in the source file, so we will check for that in semantic analysis
	node.args->accept(*this);
}

void SymbolResolution::visitBinaryExpr(ASTBinaryExpr& node)
{
	node.lhs->accept(*this);
	node.rhs->accept(*this);
}

void SymbolResolution::visitExprStmt(ASTExprStmt& node)
{
	node.expression->accept(*this);
}

void SymbolResolution::visitPostfix(ASTPostfix& node)
{
	node.expr->accept(*this);
}

void SymbolResolution::visitUnaryExpr(ASTUnaryExpr& node)
{
	node.expr->accept(*this);
}

void SymbolResolution::visitGroupExpr(ASTGroupExpr& node)
{
	node.expr->accept(*this);
}

void SymbolResolution::visitArgList(ASTArgList& node)
{
	for (ASTArgument* arg : node.args)
		arg->accept(*this);
}

void SymbolResolution::visitArgument(ASTArgument& node)
{
	node.value->accept(*this);
}

void SymbolResolution::visitAssign(ASTAssign& node)
{
	node.assignee->accept(*this);
	node.value->accept(*this);
}

void SymbolResolution::visitReturn(ASTReturn& node)
{
	node.returnVal->accept(*this);
}

void SymbolResolution::visitLogical(ASTLogical& node)
{
	node.lhs->accept(*this);
	node.rhs->accept(*this);
}

void SymbolResolution::visitBlock(ASTBlock& node)
{
	if (node.createScope)
	{
		SymbolTable* scope = symbolArena.alloc<SymbolTable>();
		env->pushScope(scope, ScopeKind::Normal);
		node.scope = scope;
	}

	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);

	env->dumpEnvironment();
	if (node.createScope)
		env->popScope();
}

void SymbolResolution::visitForLoop(ASTForLoop& node)
{
	SymbolTable* scope = symbolArena.alloc<SymbolTable>();
	env->pushScope(scope, ScopeKind::Normal);
	node.scope = scope;

	// visit the initializer in case it's a var decl like i : i32 = 0; if it's an assign expression, it'll be a nop
	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
		node.condition->accept(*this);

	if (node.increment)
		node.increment->accept(*this);

	node.body->createScope = false;
	visitBlock(*node.body);

	env->dumpEnvironment();
	env->popScope();
}