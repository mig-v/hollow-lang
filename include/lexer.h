#pragma once

#include <vector>
#include <sstream>
#include <unordered_map>

#include "token.h"
#include "lexer_state.h"
#include "transition_table.h"

class Lexer
{
public:
	Lexer();

	bool lexFile(const std::string& filename);
	void dumpTokens();
	void printToken(const Token& token);
	void reset();

	// Returns the next token in the token stream and advances the tokenIndex pointer
	Token* getToken();

	// Returns the next token in the token stream but does NOT advance the tokenIndex pointer
	Token* peek();
private:
	char getChar();
	void ungetChar();

	// returns true when a token is complete. This means the most recently read character is a part of the NEXT token, so we need to put it back
	// onto the string stream
	bool consumeChar(char current);	
	void emitToken();
	void identifierOrKeyword();

	Token currentToken;
	LexerState state;
	TransitionTable transitionTable;
	size_t tokenIndex;	// Always points to the next valid token to process
	uint32_t lineNum;

	std::vector<Token> tokens;
	std::stringstream fileContents;
	std::string currentLiteral;
	std::unordered_map<std::string, TokenType> keywords;
};