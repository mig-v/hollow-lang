#pragma once

#include "ast_node.h"
#include "symbol_table.h"

class ASTExpr;
class ASTType;

class ASTStmt : public ASTNode
{
public:

};

class ASTExprStmt : public ASTStmt
{
public:
	ASTExprStmt(ASTExpr* expression);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* expression;
};

class ASTVarDecl : public ASTStmt
{
public:
	ASTVarDecl(ASTType* type, Token varIdentifier, ASTExpr* initialization);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	//TokenType varType;
	ASTType* type;
	Token varIdentifier;
	ASTExpr* initialization;

	// type resolution data, populated during semantic analysis
	TypeInfo* typeInfo;
	Symbol* symbol;
	int scope;
	int slotIndex;
};

class ASTBlock : public ASTStmt
{
public:
	ASTBlock();

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	std::vector<ASTStmt*> statements;
	SymbolTable* scope;

	// used during semantic analysis, blocks on their own should create scopes, but blocks for if, while, for, and funcDecl nodes
	// shouldnt. The control statements are responsible for creating the scope
	bool createScope;
};

class ASTFuncDecl : public ASTStmt
{
public:
	ASTFuncDecl(Token funcIdentifier, Token returnType, ASTParamList* params, ASTBlock* body);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	Token funcIdentifier;
	Token returnType;
	ASTBlock* body;
	ASTParamList* params;
	TypeInfo* typeInfo;
	SymbolTable* scope;
};

class ASTReturn : public ASTStmt
{
public:
	ASTReturn(ASTExpr* returnVal);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* returnVal;
};

class ASTForLoop : public ASTStmt
{
public:
	ASTForLoop(ASTStmt* initializer, ASTExpr* condition, ASTExpr* increment, ASTBlock* body);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTStmt* initializer;
	ASTExpr* condition;
	ASTExpr* increment;
	ASTBlock* body;
	SymbolTable* scope;
};

class ASTWhileLoop : public ASTStmt
{
public:
	ASTWhileLoop(ASTExpr* condition, ASTBlock* body);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* condition;
	ASTBlock* body;
};

class ASTIfStatement : public ASTStmt
{
public:
	ASTIfStatement(ASTExpr* condition, ASTStmt* trueBranch, ASTStmt* falseBranch);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);

	ASTExpr* condition;
	ASTStmt* trueBranch;
	ASTStmt* falseBranch;
};