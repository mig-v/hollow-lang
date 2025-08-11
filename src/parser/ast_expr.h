#pragma once

#include "parser/ast_node.h"

#include "core/l_value.h"

struct Symbol;

class ASTExpr : public ASTNode
{
public:
	TypeInfo* typeInfo;

	// those booleans below should also become virtual methods
	virtual bool isLValue() const {  return false; }
	virtual LValue getLValue() const { return LValue{}; }
	virtual std::string getLValueIdentifier() const { return "default l value identifier"; }

	// defeats the purpose of inheritance + dispatch calls, but I need to know what types of nodes some expressions are
	// while doing things, so these are going here to avoid dynamic casts
	bool shortCircuitable;
	bool isGroupExpr;
};

class ASTIntLiteral : public ASTExpr
{
public:
	ASTIntLiteral(uint64_t value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	uint64_t value;
};

class ASTDoubleLiteral : public ASTExpr  // f64
{
public:
	ASTDoubleLiteral(double value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	double value;
};

class ASTCharLiteral : public ASTExpr
{
public:
	ASTCharLiteral(char value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	char value;
};

class ASTBoolLiteral : public ASTExpr
{
public:
	ASTBoolLiteral(bool value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	bool value;
};

class ASTIdentifier : public ASTExpr
{
public:
	ASTIdentifier(Token identifier);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	bool isLValue() const override {  return true; }
	std::string getLValueIdentifier() const override { return std::get<std::string>(identifier.value); }
	LValue getLValue() const override;

	Token identifier;
	Symbol* symbol;
};

class ASTAssign : public ASTExpr
{
public:
	ASTAssign(ASTExpr* assignee, Token op, ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	ASTExpr* assignee;
	Token op;
	ASTExpr* value;

	Symbol* symbol;
};

class ASTLogical : public ASTExpr
{
public:
	ASTLogical(ASTExpr* lhs, Token logicalOperator, ASTExpr* rhs);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

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
	IROperand accept(IRGenerator& visitor);

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
	IROperand accept(IRGenerator& visitor);

	std::string getLValueIdentifier() const override { return expr->getLValueIdentifier(); }
	bool isLValue() const override;

	Token op;
	ASTExpr* expr;
};

class ASTCall : public ASTExpr
{
public:
	ASTCall(ASTExpr* callee, ASTArgList* args);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

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
	IROperand accept(IRGenerator& visitor);

	ASTExpr* expr;
};

class ASTPostfix : public ASTExpr
{
public:
	ASTPostfix(ASTExpr* expr, Token op);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	ASTExpr* expr;
	Token op;
};

class ASTArgument : public ASTExpr
{
public:
	ASTArgument(ASTExpr* value);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	ASTExpr* value;
};

class ASTCast : public ASTExpr
{
public:
	ASTCast(ASTExpr* expr);
	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	ASTExpr* expr;
};

class ASTArrayAccess : public ASTExpr
{
public:
	ASTArrayAccess(ASTExpr* arrayExpr, ASTExpr* indexExpr);
	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	bool isLValue() const override { return true; }
	std::string getLValueIdentifier() const { return arrayExpr->getLValueIdentifier(); }
	LValue getLValue() const override;

	ASTExpr* arrayExpr;	// the expr that results in an array E.G arr, foo(), vec.arr, etc.
	ASTExpr* indexExpr; // the actual index expression, [1], [x], [foo()], etc
};

class ASTFieldAccess : public ASTExpr
{
public:
	ASTFieldAccess(ASTExpr* structExpr, Token fieldIdentifier);
	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	bool isLValue() const override { return true; }
	std::string getLValueIdentifier() const { return structExpr->getLValueIdentifier(); }

	ASTExpr* structExpr;
	Token fieldIdentifier;
};