#pragma once

#include "parser/ast_node.h"
#include "parser/ast_expr.h"
#include "parser/ast_stmt.h"

class ASTVisitor
{
public:
	virtual ~ASTVisitor() = default;

	virtual void visitIntLiteral(ASTIntLiteral& node) {}
	virtual void visitDoubleLiteral(ASTDoubleLiteral& node) {}
	virtual void visitCharLiteral(ASTCharLiteral& node) {}
	virtual void visitBoolLiteral(ASTBoolLiteral& node) {}
	virtual void visitVarDecl(ASTVarDecl& node) {}
	virtual void visitFuncDecl(ASTFuncDecl& node) {}
	virtual void visitIdentifier(ASTIdentifier& node) {}
	virtual void visitExprStmt(ASTExprStmt& node) {}
	virtual void visitAssign(ASTAssign& node) {}
	virtual void visitReturn(ASTReturn& node) {}
	virtual void visitBlock(ASTBlock& node) {}
	virtual void visitForLoop(ASTForLoop& node) {}
	virtual void visitWhileLoop(ASTWhileLoop& node) {}
	virtual void visitIfStatement(ASTIfStatement& node) {}
	virtual void visitLogical(ASTLogical& node) {}
	virtual void visitBinaryExpr(ASTBinaryExpr& node) {}
	virtual void visitUnaryExpr(ASTUnaryExpr& node) {}
	virtual void visitCall(ASTCall& node) {}
	virtual void visitGroupExpr(ASTGroupExpr& node) {}
	virtual void visitPostfix(ASTPostfix& node) {}
	virtual void visitParameter(ASTParameter& node) {}
	virtual void visitArgument(ASTArgument& node) {}
	virtual void visitParamList(ASTParamList& node) {}
	virtual void visitArgList(ASTArgList& node) {}
	virtual void visitCast(ASTCast& node) {}
	virtual void visitArrayAccess(ASTArrayAccess& node) {}
	virtual void visitStructDecl(ASTStructDecl& node) {}
	virtual void visitFieldAccess(ASTFieldAccess& node) {}
};