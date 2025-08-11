#pragma once

#include <unordered_map>
#include <array>

#include "lexer/lexer_state.h"
#include "lexer/token.h"

struct TransitionInfo
{
	LexerState state;
	TokenType  tokenType;
};

class TransitionTable
{
public:
	TransitionTable();
	LexerState getTransition(Token& currentToken, LexerState currentState, char currentChar);
private:
	// Util functions to fill [A-Za-z] or [0-9] for the given 'state' with the given transition states and tokens
	void fillAlphaEntry(LexerState state, LexerState transitionState, TokenType transitionToken);
	void fillDigitEntry(LexerState state, LexerState transitionState, TokenType transitionToken);

	// Fills 'state' with transition states and tokens for ALL valid characters in the grammar (except for whitespace) This is mainly for invalid tokens
	void fillAllEntry(LexerState state, LexerState transitionState, TokenType transitionToken);

	std::array<std::unordered_map<char, TransitionInfo>, (size_t)LexerState::NumLexerStates> transitions;
};