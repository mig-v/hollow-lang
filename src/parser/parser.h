#pragma once

#include "parser/ast_node.h"
#include "parser/ast_expr.h"
#include "parser/ast_stmt.h"
#include "parser/token_stream.h"

#include "core/diagnostic.h"
#include "core/memory_arena.h"

#include <vector>

class Parser
{
public:
	Parser();

	void parse(std::vector<Token>& tokens, MemoryArena* nodeArena, DiagnosticReporter* diagnosticReporter);
	void reset();
	void printAST();

	inline std::vector<ASTNode*>& getAst() { return ast; }

private:
	ASTStmt* topLevelDeclaration();
	ASTStmt* funcLevelDeclaration();
	ASTStmt* structDecl();
	ASTStmt* varDecl();
	ASTStmt* funcDecl();
	ASTStmt* statement();
	ASTStmt* forStatement();
	ASTStmt* ifStatement();
	ASTStmt* whileStatement();
	ASTBlock* block();
	ASTStmt* returnStatement();
	ASTStmt* expressionStatement();

	ASTExpr* expression();
	ASTExpr* assignment();
	ASTExpr* logicalOr();
	ASTExpr* logicalAnd();
	ASTExpr* equality();
	ASTExpr* bitwiseOr();
	ASTExpr* bitwiseXor();
	ASTExpr* bitwiseAnd();
	ASTExpr* comparison();
	ASTExpr* bitwiseShift();
	ASTExpr* term();
	ASTExpr* factor();
	ASTExpr* unary();
	ASTExpr* postfix();
	ASTExpr* primary();

	ASTExpr* parseFuncArgs(ASTExpr* callee);
	ASTParamList* parseFuncParams();
	ASTType* parseASTType();
	void     parseFieldDecls(std::vector<ASTVarDecl*>& fields);

	void assertCurrent(TokenType type, const std::string& errorMsg);	// assert the top token matches 'type', otherwise report a syntax error
	void assertCurrentIsType(const std::string& errorMsg);
	void throwError(const std::string& errorMsg);
	void synchronize();

	MemoryArena* arena;
	TokenStream tokenStream;
	DiagnosticReporter* diagnosticReporter;

	std::vector<ASTNode*> ast;

	bool inFuncDecl;
};