#pragma once

#include "ast_node.h"

class ASTPrinter
{
public:
	//void visitUnsignedLiteral(ASTUnsignedLiteral& node);
	//void visitSignedLiteral(ASTSignedLiteral& node);
	void visitIntLiteral(ASTIntLiteral& node);
	//void visitFloatLiteral(ASTFloatLiteral& node);
	void visitDoubleLiteral(ASTDoubleLiteral& node);
	void visitCharLiteral(ASTCharLiteral& node);
	void visitBoolLiteral(ASTBoolLiteral& node);
	void visitVarDecl(ASTVarDecl& node);
	void visitFuncDecl(ASTFuncDecl& node);
	void visitVarAccess(ASTVarAccess& node);
	void visitExprStmt(ASTExprStmt& node);
	void visitAssign(ASTAssign& node);
};