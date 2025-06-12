#include "ast_node.h"
#include "ast_printer.h"

//ASTUnsignedLiteral::ASTUnsignedLiteral(TokenType primitiveType, uint64_t value)
//{
//	this->type = primitiveType;
//	this->value = value;
//}
//
//void ASTUnsignedLiteral::accept(ASTPrinter visitor)
//{
//	visitor.visitUnsignedLiteral(*this);
//}
//
//ASTSignedLiteral::ASTSignedLiteral(TokenType primitiveType, int64_t value)
//{
//	this->type = primitiveType;
//	this->value = value;
//}
//
//void ASTSignedLiteral::accept(ASTPrinter visitor)
//{
//	visitor.visitSignedLiteral(*this);
//}

ASTIntLiteral::ASTIntLiteral(uint64_t value)
{
	this->value = value;
}

void ASTIntLiteral::accept(ASTPrinter visitor)
{
	visitor.visitIntLiteral(*this);
}

//ASTFloatLiteral::ASTFloatLiteral(float value)
//{
//	this->value = value;
//}
//
//void ASTFloatLiteral::accept(ASTPrinter visitor)
//{
//	visitor.visitFloatLiteral(*this);
//}

ASTDoubleLiteral::ASTDoubleLiteral(double value)
{
	this->value = value;
}

void ASTDoubleLiteral::accept(ASTPrinter visitor)
{
	visitor.visitDoubleLiteral(*this);
}

ASTCharLiteral::ASTCharLiteral(char value)
{
	this->value = value;
}

void ASTCharLiteral::accept(ASTPrinter visitor)
{
	visitor.visitCharLiteral(*this);
}

ASTBoolLiteral::ASTBoolLiteral(bool value)
{
	this->value = value;
}

void ASTBoolLiteral::accept(ASTPrinter visitor)
{
	visitor.visitBoolLiteral(*this);
}

ASTVarDecl::ASTVarDecl(TokenType varType, Token varIdentifier, ASTNode* initialization)
{
	this->varType = varType;
	this->varIdentifier = varIdentifier;
	this->initialization = initialization;
}

void ASTVarDecl::accept(ASTPrinter visitor)
{
	visitor.visitVarDecl(*this);
}

ASTFuncDecl::ASTFuncDecl(Token funcIdentifier, std::vector<Token>& params, ASTBlock* body)
{
	this->funcIdentifier = funcIdentifier;
	this->params = std::move(params);
	this->body = body;
}

void ASTFuncDecl::accept(ASTPrinter visitor)
{
	visitor.visitFuncDecl(*this);
}

ASTVarAccess::ASTVarAccess(Token identifier)
{
	this->identifier = identifier;
}

void ASTVarAccess::accept(ASTPrinter visitor)
{
	visitor.visitVarAccess(*this);
}

ASTExprStmt::ASTExprStmt(ASTNode* expression)
{
	this->expression = expression;
}

void ASTExprStmt::accept(ASTPrinter visitor)
{
	visitor.visitExprStmt(*this);
}

ASTAssign::ASTAssign(Token identifier, ASTNode* value)
{
	this->identifier = identifier;
	this->value = value;
}

void ASTAssign::accept(ASTPrinter visitor)
{
	visitor.visitAssign(*this);
}

ASTReturn::ASTReturn(ASTNode* returnVal)
{
	this->returnVal = returnVal;
}

void ASTReturn::accept(ASTPrinter visitor)
{
	visitor.visitReturn(*this);
}

void ASTBlock::accept(ASTPrinter visitor)
{
	visitor.visitBlock(*this);
}

ASTForLoop::ASTForLoop(ASTNode* initializer, ASTNode* condition, ASTNode* increment, ASTBlock* body)
{
	this->initializer = initializer;
	this->condition = condition;
	this->increment = increment;
	this->body = body;
}

void ASTForLoop::accept(ASTPrinter visitor)
{
	visitor.visitForLoop(*this);
}

ASTIfStatement::ASTIfStatement(ASTNode* condition, ASTNode* trueBranch, ASTNode* falseBranch)
{
	this->condition = condition;
	this->trueBranch = trueBranch;
	this->falseBranch = falseBranch;
}

void ASTIfStatement::accept(ASTPrinter visitor)
{
	visitor.visitIfStatement(*this);
}

ASTLogical::ASTLogical(ASTNode* lhs, Token logicalOperator, ASTNode* rhs)
{
	this->lhs = lhs;
	this->logicalOperator;
	this->rhs = rhs;
}

void ASTLogical::accept(ASTPrinter visitor)
{
	visitor.visitLogical(*this);
}

ASTBinaryExpr::ASTBinaryExpr(ASTNode* lhs, Token op, ASTNode* rhs)
{
	this->lhs = lhs;
	this->op = op;
	this->rhs = rhs;
}

void ASTBinaryExpr::accept(ASTPrinter visitor)
{
	visitor.visitBinaryExpr(*this);
}

ASTUnaryExpr::ASTUnaryExpr(Token op, ASTNode* expr)
{
	this->op = op;
	this->expr = expr;
}

void ASTUnaryExpr::accept(ASTPrinter visitor)
{
	visitor.visitUnaryExpr(*this);
}

ASTCall::ASTCall(ASTNode* callee, std::vector<ASTNode*>& args)
{
	this->callee = callee;
	this->args = std::move(args);
}

void ASTCall::accept(ASTPrinter visitor)
{
	visitor.visitCall(*this);
}

ASTGroupExpr::ASTGroupExpr(ASTNode* expr)
{
	this->expr = expr;
}

void ASTGroupExpr::accept(ASTPrinter visitor)
{
	visitor.visitGroupExpr(*this);
}