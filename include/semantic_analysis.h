#pragma once

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "environment.h"
#include "memory_arena.h"
#include "diagnostic.h"

class SemanticAnalysis
{
public:
	SemanticAnalysis();
	void analyze(std::vector<ASTNode*>& ast, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter);
	void printInfo();
	int getGlobalVarCount();

	void visitIntLiteral(ASTIntLiteral& node);
	void visitDoubleLiteral(ASTDoubleLiteral& node);
	void visitCharLiteral(ASTCharLiteral& node);
	void visitBoolLiteral(ASTBoolLiteral& node);
	void visitVarDecl(ASTVarDecl& node);
	void visitFuncDecl(ASTFuncDecl& node);
	void visitIdentifier(ASTIdentifier& node);
	void visitExprStmt(ASTExprStmt& node);
	void visitAssign(ASTAssign& node);
	void visitReturn(ASTReturn& node);
	void visitBlock(ASTBlock& node);
	void visitForLoop(ASTForLoop& node);
	void visitWhileLoop(ASTWhileLoop& node);
	void visitIfStatement(ASTIfStatement& node);
	void visitLogical(ASTLogical& node);
	void visitBinaryExpr(ASTBinaryExpr& node);
	void visitUnaryExpr(ASTUnaryExpr& node);
	void visitCall(ASTCall& node);
	void visitGroupExpr(ASTGroupExpr& node);
	void visitPostfix(ASTPostfix& node);
	void visitParameter(ASTParameter& node);
	void visitArgument(ASTArgument& node);
	void visitParamList(ASTParamList& node);
	void visitArgList(ASTArgList& node);
	void visitCast(ASTCast& node);

private:
	Environment env;
	MemoryArena* typeArena;
	DiagnosticReporter* diagnosticReporter;
	std::vector<TypeInfo*> functionCtxStack; // used for nested calls like add(10, getNum()); we need a stack to deal with this
};