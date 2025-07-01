#pragma once

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"

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



	//void visitFuncDecl(ASTFuncDecl& node) override;
	//void visitVarDecl(ASTVarDecl& node) override;
	//void visitIdentifier(ASTIdentifier& node) override;  // identifier references MUST be resolved when they're encountered, this is unlike functions
	//void visitBlock(ASTBlock& node) override;
	//void visitForLoop(ASTForLoop& node) override;
	//void visitParamList(ASTParamList& node) override;
	//void visitParameter(ASTParameter& node) override;
	//void visitWhileLoop(ASTWhileLoop& node) override;
	//void visitIfStatement(ASTIfStatement& node) override;
	//void visitCall(ASTCall& node) override;
	//void visitBinaryExpr(ASTBinaryExpr& node) override;
	//void visitExprStmt(ASTExprStmt& node) override;
	//void visitPostfix(ASTPostfix& node) override;
	//void visitUnaryExpr(ASTUnaryExpr& node) override;
	//void visitGroupExpr(ASTGroupExpr& node) override;
	//void visitArgList(ASTArgList& node) override;
	//void visitArgument(ASTArgument& node) override;
};