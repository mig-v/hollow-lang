#pragma once

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "environment.h"
#include "memory_arena.h"

class SemanticAnalysis
{
public:
	SemanticAnalysis() = default;
	void printInfo();

	void visitIntLiteral(ASTIntLiteral& node);
	void visitDoubleLiteral(ASTDoubleLiteral& node);
	void visitCharLiteral(ASTCharLiteral& node);
	void visitBoolLiteral(ASTBoolLiteral& node);
	void visitVarDecl(ASTVarDecl& node);
	void visitFuncDecl(ASTFuncDecl& node);
	void visitVariable(ASTVariable& node);
	void visitExprStmt(ASTExprStmt& node);
	void visitAssign(ASTAssign& node);
	void visitReturn(ASTReturn& node);
	void visitBlock(ASTBlock& node);
	void visitForLoop(ASTForLoop& node);
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

private:
	TypeKind getTypeFromToken(TokenType token);
	bool fitsInIntegerType(uint64_t value, TypeKind type);
	bool fitsInFloatType(double value, TypeKind type);
	bool isAssignable(TypeKind lhs, TypeKind rhs);
	
	Environment env;
	MemoryArena arena;
	TypeInfo* functionCtx;
};