#pragma once

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"

class ASTPrinter
{
public:
	void visitIntLiteral(ASTIntLiteral& node, uint32_t depth);
	void visitDoubleLiteral(ASTDoubleLiteral& node, uint32_t depth);
	void visitCharLiteral(ASTCharLiteral& node, uint32_t depth);
	void visitBoolLiteral(ASTBoolLiteral& node, uint32_t depth);
	void visitVarDecl(ASTVarDecl& node, uint32_t depth);
	void visitFuncDecl(ASTFuncDecl& node, uint32_t depth);
	void visitIdentifier(ASTIdentifier& node, uint32_t depth);
	void visitExprStmt(ASTExprStmt& node, uint32_t depth);
	void visitAssign(ASTAssign& node, uint32_t depth);
	void visitReturn(ASTReturn& node, uint32_t depth);
	void visitBlock(ASTBlock& node, uint32_t depth);
	void visitForLoop(ASTForLoop& node, uint32_t depth);
	void visitWhileLoop(ASTWhileLoop& node, uint32_t depth);
	void visitIfStatement(ASTIfStatement& node, uint32_t depth);
	void visitLogical(ASTLogical& node, uint32_t depth);
	void visitBinaryExpr(ASTBinaryExpr& node, uint32_t depth);
	void visitUnaryExpr(ASTUnaryExpr& node, uint32_t depth);
	void visitCall(ASTCall& node, uint32_t depth);
	void visitGroupExpr(ASTGroupExpr& node, uint32_t depth);
	void visitPostfix(ASTPostfix& node, uint32_t depth);
	void visitParameter(ASTParameter& node, uint32_t depth);
	void visitArgument(ASTArgument& node, uint32_t depth);
	void visitParamList(ASTParamList& node, uint32_t depth);
	void visitArgList(ASTArgList& node, uint32_t depth);
	void visitCast(ASTCast& node, uint32_t depth);

	void printNull(uint32_t depth, const std::string& message);
	void printTypeInfo(uint32_t depth, TypeInfo* info);
};