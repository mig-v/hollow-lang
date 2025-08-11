#pragma once

#include "parser/ast_node.h"
#include "parser/ast_type.h"

#include "semantics/symbol_table.h"

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
	IROperand accept(IRGenerator& visitor);

	ASTExpr* expression;
};

class ASTVarDecl : public ASTStmt
{
public:
	ASTVarDecl(ASTType* type, Token varIdentifier, ASTExpr* initialization);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	//TokenType varType;
	ASTType* type;
	Token varIdentifier;
	ASTExpr* initialization;

	// type resolution data, populated during semantic analysis
	TypeInfo* typeInfo;
	Symbol* symbol;
};

class ASTBlock : public ASTStmt
{
public:
	ASTBlock();

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	std::vector<ASTStmt*> statements;
	SymbolTable* scope;

	// used during semantic analysis, blocks on their own should create scopes, but blocks for if, while, for, and funcDecl nodes
	// shouldnt. The control statements are responsible for creating the scope
	bool createScope;
};

class ASTFuncDecl : public ASTStmt
{
public:
	ASTFuncDecl(Token funcIdentifier, ASTType* returnType, ASTParamList* params, ASTBlock* body);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	Token funcIdentifier;
	//Token returnType;
	ASTType* returnType;
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
	IROperand accept(IRGenerator& visitor);

	ASTExpr* returnVal;
};

class ASTForLoop : public ASTStmt
{
public:
	ASTForLoop(ASTStmt* initializer, ASTExpr* condition, ASTExpr* increment, ASTBlock* body);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

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
	IROperand accept(IRGenerator& visitor);

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
	IROperand accept(IRGenerator& visitor);

	ASTExpr* condition;
	ASTStmt* trueBranch;
	ASTStmt* falseBranch;
};

class ASTStructDecl : public ASTStmt
{
public:
	ASTStructDecl(ASTStructType* type);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	ASTStructType* type;
	std::vector<ASTVarDecl*> fields;
	//TypeInfo* typeInfo;
};