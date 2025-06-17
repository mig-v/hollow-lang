#pragma once

#include "ast_node.h"

class ASTExpr : public ASTNode
{
public:
	TypeInfo* typeInfo;
};

class ASTIntLiteral : public ASTExpr
{
public:
	ASTIntLiteral(uint64_t value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	uint64_t value;
};

class ASTDoubleLiteral : public ASTExpr  // f64
{
public:
	ASTDoubleLiteral(double value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	double value;
};

class ASTCharLiteral : public ASTExpr
{
public:
	ASTCharLiteral(char value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	char value;
};

class ASTBoolLiteral : public ASTExpr
{
public:
	ASTBoolLiteral(bool value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	bool value;
};

class ASTVariable : public ASTExpr
{
public:
	ASTVariable(Token identifier);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	Token identifier;
};

class ASTAssign : public ASTExpr
{
public:
	ASTAssign(Token identifier, Token op, ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	Token identifier;
	Token op;
	ASTExpr* value;
};

class ASTLogical : public ASTExpr
{
public:
	ASTLogical(ASTExpr* lhs, Token logicalOperator, ASTExpr* rhs);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTExpr* lhs;
	Token logicalOperator;
	ASTExpr* rhs;
};

class ASTBinaryExpr : public ASTExpr
{
public:
	ASTBinaryExpr(ASTExpr* lhs, Token op, ASTExpr* rhs);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTExpr* lhs;
	Token op;
	ASTExpr* rhs;
};

class ASTUnaryExpr : public ASTExpr
{
public:
	ASTUnaryExpr(Token op, ASTExpr* expr);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	Token op;
	ASTExpr* expr;
};

class ASTCall : public ASTExpr
{
public:
	ASTCall(ASTNode* callee, ASTArgList* args);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTNode* callee;
	ASTArgList* args;
};

class ASTGroupExpr : public ASTExpr
{
public:
	ASTGroupExpr(ASTExpr* expr);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTExpr* expr;
};

class ASTPostfix : public ASTExpr
{
public:
	ASTPostfix(ASTExpr* expr, Token op);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTExpr* expr;
	Token op;
};

class ASTArgument : public ASTExpr
{
public:
	ASTArgument(ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	ASTExpr* value;
};