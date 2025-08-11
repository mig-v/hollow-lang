#pragma once

#include "lexer/token.h"
#include "lexer/lexer_state.h"
#include "lexer/transition_table.h"

#include <vector>
#include <sstream>
#include <unordered_map>

class Lexer
{
public:
	Lexer();

	bool lexFile(const std::string& filename);

	// Mainly for debugging purposes, a simple dump of the lexers token stream
	void dumpTokens();

	// Prints the type of token passed in as well as its value if applicable
	void printToken(const Token& token);

	// Resets the lexers internal state. reset() must be called before lexing another file if a file has been previously lexed
	void reset();

	// Returns the next token in the token stream and advances the tokenIndex pointer
	Token* getToken();

	// Returns the next token in the token stream but does NOT advance the tokenIndex pointer
	Token* peek();

	// returns the lexers token stream
	inline std::vector<Token>& getTokens() { return tokens; }
private:
	char getChar();
	void ungetChar();

	// returns true when a token is complete. This means the most recently read character is a part of the NEXT token, so we need to put it back
	// onto the string stream
	bool consumeChar(char current);	

	// Outputs a token to the lexers token stream
	void emitToken();

	// Checks if the currentLiteral is an identifier or keyword
	void identifierOrKeyword();

	// Checks if the currentLiteral is a valid escape sequence char, I.E. '\n', '\t', '\\', etc.
	bool validEscapeSequence();

	Token currentToken;
	LexerState state;
	TransitionTable transitionTable;
	size_t tokenIndex;	// Always points to the next valid token to process
	uint32_t lineNum;
	uint32_t column;
	uint32_t tokenStartCol;

	std::vector<Token> tokens;
	std::stringstream fileContents;
	std::string currentLiteral;
	std::unordered_map<std::string, TokenType> keywords;
	std::unordered_map<std::string, char> validEscapeChars;
};