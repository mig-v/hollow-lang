#pragma once

#include "lexer/token.h"

#include <vector>
#include <initializer_list>

class TokenStream
{
public:
	TokenStream() = default;

	void init(std::vector<Token>& tokens);

	Token consume();			// consume the top token and advance the stream pointer
	Token get();				// gets the top token but does NOT advance the stream pointer
	Token prev();				// gets the previously consumed token [current - 1]. does NOT modify the stream pointer

	bool peek(TokenType type);	// returns true if 'type' matches the top of the token stream, false otherwise
	bool lookAhead(TokenType type);	// returns true if 'type' matches the token 1 ahead of the current token. Looks ahead by 1 token
	bool tokensInStream();		// returns true if there are still tokens in the stream
	bool matchToken(std::initializer_list<TokenType> list);	// returns true if the top token matches any of the token types in 'list', false otherwise
	bool topIsPrimitive();		// returns true if the top token is a primitive type, [u8 ... u64, i8 ... i64, f32, f64, char, bool]
	bool topIsAssignment();		// returns true if the top token is an assignment operator, [=, +=, -=, *=, /=, &=, |=, ^=, <<=, >>=]
private:
	std::vector<Token> tokens;
	size_t current;
	Token nullToken;
};