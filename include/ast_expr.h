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
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	uint64_t value;
};

class ASTDoubleLiteral : public ASTExpr  // f64
{
public:
	ASTDoubleLiteral(double value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	double value;
};

class ASTCharLiteral : public ASTExpr
{
public:
	ASTCharLiteral(char value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	char value;
};

class ASTBoolLiteral : public ASTExpr
{
public:
	ASTBoolLiteral(bool value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	bool value;
};

class ASTIdentifier : public ASTExpr
{
public:
	ASTIdentifier(Token identifier);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	Token identifier;
	int scope;
	int slotIndex;
};

class ASTAssign : public ASTExpr
{
public:
	ASTAssign(ASTExpr* assignee, Token op, ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* assignee;
	Token op;
	ASTExpr* value;

	int slotIndex;
};

class ASTLogical : public ASTExpr
{
public:
	ASTLogical(ASTExpr* lhs, Token logicalOperator, ASTExpr* rhs);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* lhs;
	Token logicalOperator;
	ASTExpr* rhs;
};

class ASTBinaryExpr : public ASTExpr
{
public:
	ASTBinaryExpr(ASTExpr* lhs, Token op, ASTExpr* rhs);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* lhs;
	Token op;
	ASTExpr* rhs;
};

class ASTUnaryExpr : public ASTExpr
{
public:
	ASTUnaryExpr(Token op, ASTExpr* expr);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	Token op;
	ASTExpr* expr;
	int slotIndex;	// only applicable when the UnaryExpr is something like ++x or --x
};

class ASTCall : public ASTExpr
{
public:
	ASTCall(ASTExpr* callee, ASTArgList* args);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* callee;
	ASTArgList* args;
};

class ASTGroupExpr : public ASTExpr
{
public:
	ASTGroupExpr(ASTExpr* expr);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* expr;
};

class ASTPostfix : public ASTExpr
{
public:
	ASTPostfix(ASTExpr* expr, Token op);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* expr;
	Token op;

	// will need to add more when array access is a thing, and pointers, structs, etc. 
	// This field can prob just become an LValue struct from l_value.h
	int slotIndex;
};

class ASTArgument : public ASTExpr
{
public:
	ASTArgument(ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* value;
};

class ASTCast : public ASTExpr
{
public:
	ASTCast(ASTExpr* expr);
	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* expr;
};