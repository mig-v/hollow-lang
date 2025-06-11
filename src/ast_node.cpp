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

ASTFuncDecl::ASTFuncDecl(Token funcIdentifier, std::vector<Token>& params, std::vector<ASTNode*>& body)
{
	this->funcIdentifier = funcIdentifier;
	this->params = std::move(params);
	this->body = std::move(body);
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