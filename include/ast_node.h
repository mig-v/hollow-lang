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

class ASTBlock : public ASTNode
{
public:
	ASTBlock() = default;

	void accept(ASTPrinter visitor);

	std::vector<ASTNode*> statements;
};

class ASTFuncDecl : public ASTNode
{
public:
	ASTFuncDecl(Token funcIdentifier, std::vector<Token>& params, ASTBlock* body);

	void accept(ASTPrinter visitor);

	Token funcIdentifier;
	ASTBlock* body;
	std::vector<Token> params;
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

class ASTReturn : public ASTNode
{
public:
	ASTReturn(ASTNode* returnVal);

	void accept(ASTPrinter visitor);

	ASTNode* returnVal;
};

class ASTForLoop : public ASTNode
{
public:
	ASTForLoop(ASTNode* initializer, ASTNode* condition, ASTNode* increment, ASTBlock* body);

	void accept(ASTPrinter visitor);
	
	ASTNode* initializer;
	ASTNode* condition;
	ASTNode* increment;
	ASTBlock* body;
};

class ASTIfStatement : public ASTNode
{
public:
	ASTIfStatement(ASTNode* condition, ASTNode* trueBranch, ASTNode* falseBranch);

	void accept(ASTPrinter visitor);

	ASTNode* condition;
	ASTNode* trueBranch;
	ASTNode* falseBranch;
};

class ASTLogical : public ASTNode
{
public:
	ASTLogical(ASTNode* lhs, Token logicalOperator, ASTNode* rhs);

	void accept(ASTPrinter visitor);

	ASTNode* lhs;
	Token logicalOperator;
	ASTNode* rhs;
};

class ASTBinaryExpr : public ASTNode
{
public:
	ASTBinaryExpr(ASTNode* lhs, Token op, ASTNode* rhs);

	void accept(ASTPrinter visitor);

	ASTNode* lhs;
	Token op;
	ASTNode* rhs;
};

class ASTUnaryExpr : public ASTNode
{
public:
	ASTUnaryExpr(Token op, ASTNode* expr);

	void accept(ASTPrinter visitor);

	Token op;
	ASTNode* expr;
};

class ASTCall : public ASTNode
{
public:
	ASTCall(ASTNode* callee, std::vector<ASTNode*>& args);

	void accept(ASTPrinter visitor);

	ASTNode* callee;
	std::vector<ASTNode*> args;
};

class ASTGroupExpr : public ASTNode
{
public:
	ASTGroupExpr(ASTNode* expr);

	void accept(ASTPrinter visitor);

	ASTNode* expr;
};