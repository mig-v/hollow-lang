#pragma once

#include <unordered_map>
#include <array>

#include "lexer_state.h"
#include "token.h"


/*

Basic Idea behind this, is to avoid all the nested switch cases / if statements inside the lexer class
and just have hardcoded rules (which is what a grammar essentially is) to determine any state transitions
and when a token is complete, etc.

This will need to be achieved by:
	Being able to pass in the current state, and the current character, and get a new state / be notified if the token is complete

	[
		[StartState, 'a' = {IdentiferKeywordState, IdentifierToken}]
	
	
	
	]

	{ StartState : { all valid characters : TransitionInfo } }

	[StartState, IntLiteralState, ... , EOFState]
	array[LEXER_STATE_COUNT] TransitionLink;

	Where TransitionLink -> {char : TransitionInfo}

*/

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