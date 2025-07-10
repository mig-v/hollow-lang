#pragma once

#include "ast_visitor.h"

// mutates the AST, replacing constant expressions with literal nodes
// E.G. will replace things like 10 + 20, which would be BinaryExpr -> IntLiteral + IntLiteral with just an IntLiteral(30) node
class ConstantFolder : public ASTVisitor
{
public:
	ConstantFolder();

	void visitIntLiteral(ASTIntLiteral& node) override;
	void visitDoubleLiteral(ASTDoubleLiteral& node) override;
	void visitCharLiteral(ASTCharLiteral& node) override;
	void visitBoolLiteral(ASTBoolLiteral& node) override;
	void visitVarDecl(ASTVarDecl& node) override;
	void visitFuncDecl(ASTFuncDecl& node) override;
	void visitIdentifier(ASTIdentifier& node) override;
	void visitExprStmt(ASTExprStmt& node) override;
	void visitAssign(ASTAssign& node) override;
	void visitReturn(ASTReturn& node) override;
	void visitBlock(ASTBlock& node) override;
	void visitForLoop(ASTForLoop& node) override;
	void visitWhileLoop(ASTWhileLoop& node) override;
	void visitIfStatement(ASTIfStatement& node) override;
	void visitLogical(ASTLogical& node) override;
	void visitBinaryExpr(ASTBinaryExpr& node) override;
	void visitUnaryExpr(ASTUnaryExpr& node) override;
	void visitCall(ASTCall& node) override;
	void visitGroupExpr(ASTGroupExpr& node) override;
	void visitPostfix(ASTPostfix& node) override;
	void visitParameter(ASTParameter& node) override;
	void visitArgument(ASTArgument& node) override;
	void visitParamList(ASTParamList& node) override;
	void visitArgList(ASTArgList& node) override;
	void visitArrayAccess(ASTArrayAccess& node) override;
};