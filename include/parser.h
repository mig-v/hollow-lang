#pragma once

#include <vector>

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "token_stream.h"
#include "error_report.h"
#include "memory_arena.h"

class Parser
{
public:
	Parser() = default;

	void parse(std::vector<Token>& tokens);
	void reset();
	void printAST();
	void printErrors();

	inline const std::vector<ASTNode*>& getAst() const { return ast; }

private:
	ASTStmt* declaration();
	ASTStmt* varDecl();
	ASTStmt* funcDecl();
	ASTStmt* statement();
	ASTStmt* forStatement();
	ASTStmt* ifStatement();
	ASTBlock* block();
	ASTStmt* returnStatement();
	ASTStmt* expressionStatement();

	ASTExpr* expression();
	ASTExpr* assignment();
	ASTExpr* logicalOr();
	ASTExpr* logicalAnd();
	ASTExpr* bitwiseOr();
	ASTExpr* bitwiseXor();
	ASTExpr* bitwiseAnd();
	ASTExpr* equality();
	ASTExpr* comparison();
	ASTExpr* bitwiseShift();
	ASTExpr* term();
	ASTExpr* factor();
	ASTExpr* unary();
	ASTExpr* postfix();
	ASTExpr* primary();

	ASTExpr* parseFuncArgs(ASTNode* callee);
	ASTParamList* parseFuncParams();

	void assertCurrent(TokenType type, const std::string& errorMsg);	// assert the top token matches 'type', otherwise report a syntax error
	void assertCurrentIsType(const std::string& errorMsg);
	void throwError(const std::string& errorMsg);
	void synchronize();

	MemoryArena arena;
	TokenStream tokenStream;

	std::vector<ASTNode*> ast;
	std::vector<ErrorReport> parseErrors;

	bool inFuncDecl;
};