#pragma once

#include <stdint.h>
#include <vector>

#include "token.h"

// forward declare any visitor classes needed
class ASTPrinter;

class ASTNode
{
public:
	virtual void accept(ASTPrinter visitor) = 0;
};


class ASTIntLiteral : public ASTNode
{
public:
	ASTIntLiteral(uint64_t value);

	void accept(ASTPrinter visitor);

	uint64_t value;
};

class ASTDoubleLiteral : public ASTNode  // f64
{
public:
	ASTDoubleLiteral(double value);

	void accept(ASTPrinter visitor);

	double value;
};

class ASTCharLiteral : public ASTNode
{
public:
	ASTCharLiteral(char value);

	void accept(ASTPrinter visitor);

	char value;
};

class ASTBoolLiteral : public ASTNode
{
public:
	ASTBoolLiteral(bool value);

	void accept(ASTPrinter visitor);

	bool value;
};

class ASTVarDecl : public ASTNode
{
public:
	ASTVarDecl(TokenType varType, Token varIdentifier, ASTNode* initialization);

	void accept(ASTPrinter visitor);

	TokenType varType;
	Token varIdentifier;
	ASTNode* initialization;
};

class ASTFuncDecl : public ASTNode
{
public:
	ASTFuncDecl(Token funcIdentifier, std::vector<Token>& params, std::vector<ASTNode*>& body);

	void accept(ASTPrinter visitor);

	Token funcIdentifier;
	std::vector<Token> params;
	std::vector<ASTNode*> body;
};

class ASTVarAccess : public ASTNode
{
public:
	ASTVarAccess(Token identifier);

	void accept(ASTPrinter visitor);

	Token identifier;
};

class ASTExprStmt : public ASTNode
{
public:
	ASTExprStmt(ASTNode* expression);

	void accept(ASTPrinter visitor);

	ASTNode* expression;
};

class ASTAssign : public ASTNode
{
public:
	ASTAssign(Token identifier, ASTNode* value);

	void accept(ASTPrinter visitor);

	Token identifier;
	ASTNode* value;
};