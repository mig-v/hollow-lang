#pragma once

#include <vector>

#include "ast_node.h"
#include "token_stream.h"
#include "error_report.h"

class Parser
{
public:
	Parser() = default;

	void parse(std::vector<Token>& tokens);
	void printAST();
	void printErrors();

private:
	ASTNode* declaration();
	ASTNode* varDecl();
	ASTNode* funcDecl();
	ASTNode* statement();
	ASTNode* expressionStatement();

	ASTNode* expression();
	ASTNode* assignment();

	// ...

	ASTNode* primary();

	void assertCurrent(TokenType type, const std::string& errorMsg);	// assert the top token matches 'type', otherwise report a syntax error
	void assertCurrentIsType(const std::string& errorMsg);
	void throwError(const std::string& errorMsg);
	void synchronize();

	TokenStream tokenStream;
	std::vector<ASTNode*> ast;
	std::vector<ErrorReport> parseErrors;

	bool inFuncDecl;
};