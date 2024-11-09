#include "transition_table.h"
#include <iostream>

TransitionTable::TransitionTable()
{
	// StartState -> IdentifierKeywordState
	fillAlphaEntry(LexerState::StartState, LexerState::IdentifierKeywordState, TokenType::Identifier);
	transitions[(int)LexerState::StartState]['_'] = { LexerState::IdentifierKeywordState, TokenType::Identifier };
	transitions[(int)LexerState::StartState]['$'] = { LexerState::IdentifierKeywordState, TokenType::Identifier };

	// StartState -> IntLiteralState
	fillDigitEntry(LexerState::StartState, LexerState::IntLiteralState, TokenType::IntLiteral);

	// StartState -> CharLiteralState
	transitions[(int)LexerState::StartState]['\''] = { LexerState::CharLiteralState, TokenType::CharLiteral };

	// StartState -> SemicolonState
	transitions[(int)LexerState::StartState][';'] = { LexerState::SemicolonState, TokenType::Semicolon };

	// StartState -> AssignState
	transitions[(int)LexerState::StartState]['='] = { LexerState::AssignState, TokenType::Assign };

	// StartState -> FloatLiteralState
	transitions[(int)LexerState::StartState]['.'] = { LexerState::FloatLiteralState, TokenType::FloatLiteral };

	// StartState -> ColonState
	transitions[(int)LexerState::StartState][':'] = { LexerState::ColonState, TokenType::Colon };

	// StartState -> Math Operators
	transitions[(int)LexerState::StartState]['+'] = { LexerState::PlusState, TokenType::Plus };
	transitions[(int)LexerState::StartState]['-'] = { LexerState::MinusState, TokenType::Minus };
	transitions[(int)LexerState::StartState]['*'] = { LexerState::AsteriskState, TokenType::Asterisk };
	transitions[(int)LexerState::StartState]['/'] = { LexerState::ForwardSlashState, TokenType::ForwardSlash };

	// StartState -> Open / Close Paren and Curly State
	transitions[(int)LexerState::StartState]['('] = { LexerState::OpenParenState, TokenType::OpenParen };
	transitions[(int)LexerState::StartState][')'] = { LexerState::CloseParenState, TokenType::CloseParen };
	transitions[(int)LexerState::StartState]['{'] = { LexerState::OpenCurlyState, TokenType::OpenCurly };
	transitions[(int)LexerState::StartState]['}'] = { LexerState::CloseCurlyState, TokenType::CloseCurly };

	// StartState -> LessThanState, GreaterThanState, LogicalNotState  (Single Character Comparison Operators)
	transitions[(int)LexerState::StartState]['<'] = { LexerState::LessThanState, TokenType::LessThan };
	transitions[(int)LexerState::StartState]['>'] = { LexerState::GreaterThanState, TokenType::GreaterThan };
	transitions[(int)LexerState::StartState]['!'] = { LexerState::LogicalNotState, TokenType::LogicalNot };

	// Single character operators -> double character operators (< : <=, > : >=, ! : !=)
	transitions[(int)LexerState::LessThanState]['='] = { LexerState::LessThanEqState, TokenType::LessThanEq };
	transitions[(int)LexerState::GreaterThanState]['='] = { LexerState::GreaterThanEqState, TokenType::GreaterThanEq };
	transitions[(int)LexerState::LogicalNotState]['='] = { LexerState::NotEqState, TokenType::NotEq };
	transitions[(int)LexerState::AssignState]['='] = { LexerState::EqualityState, TokenType::Equality };

	// IdentifierKeywordState -> IdentifierKeywordState
	fillAlphaEntry(LexerState::IdentifierKeywordState, LexerState::IdentifierKeywordState, TokenType::Identifier);
	fillDigitEntry(LexerState::IdentifierKeywordState, LexerState::IdentifierKeywordState, TokenType::Identifier);
	transitions[(int)LexerState::IdentifierKeywordState]['_'] = { LexerState::IdentifierKeywordState, TokenType::Identifier };
	transitions[(int)LexerState::IdentifierKeywordState]['$'] = { LexerState::IdentifierKeywordState, TokenType::Identifier };

	// IntLiteralState -> IntLiteralState
	fillDigitEntry(LexerState::IntLiteralState, LexerState::IntLiteralState, TokenType::IntLiteral);

	// IntLiteralState -> FloatLiteralState
	transitions[(int)LexerState::IntLiteralState]['.'] = { LexerState::FloatLiteralState, TokenType::FloatLiteral };

	// FloatLiteralState -> FloatLiteralState
	fillDigitEntry(LexerState::FloatLiteralState, LexerState::FloatLiteralState, TokenType::FloatLiteral);

	// Invalid -> Invalid (The only way to break out of an invalid token is by whitespace)
	fillAllEntry(LexerState::InvalidState, LexerState::InvalidState, TokenType::Invalid);
}

void TransitionTable::fillAlphaEntry(LexerState state, LexerState transitionState, TokenType transitionToken)
{
	transitions[(int)state]['a'] = { transitionState, transitionToken };
	transitions[(int)state]['b'] = { transitionState, transitionToken };
	transitions[(int)state]['c'] = { transitionState, transitionToken };
	transitions[(int)state]['d'] = { transitionState, transitionToken };
	transitions[(int)state]['e'] = { transitionState, transitionToken };
	transitions[(int)state]['f'] = { transitionState, transitionToken };
	transitions[(int)state]['g'] = { transitionState, transitionToken };
	transitions[(int)state]['h'] = { transitionState, transitionToken };
	transitions[(int)state]['i'] = { transitionState, transitionToken };
	transitions[(int)state]['j'] = { transitionState, transitionToken };
	transitions[(int)state]['k'] = { transitionState, transitionToken };
	transitions[(int)state]['l'] = { transitionState, transitionToken };
	transitions[(int)state]['m'] = { transitionState, transitionToken };
	transitions[(int)state]['n'] = { transitionState, transitionToken };
	transitions[(int)state]['o'] = { transitionState, transitionToken };
	transitions[(int)state]['p'] = { transitionState, transitionToken };
	transitions[(int)state]['q'] = { transitionState, transitionToken };
	transitions[(int)state]['r'] = { transitionState, transitionToken };
	transitions[(int)state]['s'] = { transitionState, transitionToken };
	transitions[(int)state]['t'] = { transitionState, transitionToken };
	transitions[(int)state]['u'] = { transitionState, transitionToken };
	transitions[(int)state]['v'] = { transitionState, transitionToken };
	transitions[(int)state]['w'] = { transitionState, transitionToken };
	transitions[(int)state]['x'] = { transitionState, transitionToken };
	transitions[(int)state]['y'] = { transitionState, transitionToken };
	transitions[(int)state]['z'] = { transitionState, transitionToken };

	transitions[(int)state]['A'] = { transitionState, transitionToken };
	transitions[(int)state]['B'] = { transitionState, transitionToken };
	transitions[(int)state]['C'] = { transitionState, transitionToken };
	transitions[(int)state]['D'] = { transitionState, transitionToken };
	transitions[(int)state]['E'] = { transitionState, transitionToken };
	transitions[(int)state]['F'] = { transitionState, transitionToken };
	transitions[(int)state]['G'] = { transitionState, transitionToken };
	transitions[(int)state]['H'] = { transitionState, transitionToken };
	transitions[(int)state]['I'] = { transitionState, transitionToken };
	transitions[(int)state]['J'] = { transitionState, transitionToken };
	transitions[(int)state]['K'] = { transitionState, transitionToken };
	transitions[(int)state]['L'] = { transitionState, transitionToken };
	transitions[(int)state]['M'] = { transitionState, transitionToken };
	transitions[(int)state]['N'] = { transitionState, transitionToken };
	transitions[(int)state]['O'] = { transitionState, transitionToken };
	transitions[(int)state]['P'] = { transitionState, transitionToken };
	transitions[(int)state]['Q'] = { transitionState, transitionToken };
	transitions[(int)state]['R'] = { transitionState, transitionToken };
	transitions[(int)state]['S'] = { transitionState, transitionToken };
	transitions[(int)state]['T'] = { transitionState, transitionToken };
	transitions[(int)state]['U'] = { transitionState, transitionToken };
	transitions[(int)state]['V'] = { transitionState, transitionToken };
	transitions[(int)state]['W'] = { transitionState, transitionToken };
	transitions[(int)state]['X'] = { transitionState, transitionToken };
	transitions[(int)state]['Y'] = { transitionState, transitionToken };
	transitions[(int)state]['Z'] = { transitionState, transitionToken };
}

void TransitionTable::fillDigitEntry(LexerState state, LexerState transitionState, TokenType transitionToken)
{
	transitions[(int)state]['0'] = { transitionState, transitionToken };
	transitions[(int)state]['1'] = { transitionState, transitionToken };
	transitions[(int)state]['2'] = { transitionState, transitionToken };
	transitions[(int)state]['3'] = { transitionState, transitionToken };
	transitions[(int)state]['4'] = { transitionState, transitionToken };
	transitions[(int)state]['5'] = { transitionState, transitionToken };
	transitions[(int)state]['6'] = { transitionState, transitionToken };
	transitions[(int)state]['7'] = { transitionState, transitionToken };
	transitions[(int)state]['8'] = { transitionState, transitionToken };
	transitions[(int)state]['9'] = { transitionState, transitionToken };
}

void TransitionTable::fillAllEntry(LexerState state, LexerState transitionState, TokenType transitionToken)
{
	fillAlphaEntry(state, transitionState, transitionToken);
	fillDigitEntry(state, transitionState, transitionToken);
	transitions[(int)state]['!'] = { transitionState, transitionToken };
	transitions[(int)state]['@'] = { transitionState, transitionToken };
	transitions[(int)state]['#'] = { transitionState, transitionToken };
	transitions[(int)state]['$'] = { transitionState, transitionToken };
	transitions[(int)state]['%'] = { transitionState, transitionToken };
	transitions[(int)state]['^'] = { transitionState, transitionToken };
	transitions[(int)state]['&'] = { transitionState, transitionToken };
	transitions[(int)state]['*'] = { transitionState, transitionToken };
	transitions[(int)state]['('] = { transitionState, transitionToken };
	transitions[(int)state][')'] = { transitionState, transitionToken };
	transitions[(int)state]['_'] = { transitionState, transitionToken };
	transitions[(int)state]['-'] = { transitionState, transitionToken };
	transitions[(int)state]['+'] = { transitionState, transitionToken };
	transitions[(int)state]['='] = { transitionState, transitionToken };
	transitions[(int)state]['['] = { transitionState, transitionToken };
	transitions[(int)state][']'] = { transitionState, transitionToken };
	transitions[(int)state]['{'] = { transitionState, transitionToken };
	transitions[(int)state]['}'] = { transitionState, transitionToken };
	transitions[(int)state][';'] = { transitionState, transitionToken };
	transitions[(int)state][':'] = { transitionState, transitionToken };
	transitions[(int)state]['\''] = { transitionState, transitionToken };
	transitions[(int)state]['\"'] = { transitionState, transitionToken };
	transitions[(int)state][','] = { transitionState, transitionToken };
	transitions[(int)state]['.'] = { transitionState, transitionToken };
	transitions[(int)state]['/'] = { transitionState, transitionToken };
	transitions[(int)state]['<'] = { transitionState, transitionToken };
	transitions[(int)state]['>'] = { transitionState, transitionToken };
	transitions[(int)state]['?'] = { transitionState, transitionToken };
	transitions[(int)state]['|'] = { transitionState, transitionToken };
	transitions[(int)state]['\\'] = { transitionState, transitionToken };
}

LexerState TransitionTable::getTransition(Token& currentToken, LexerState currentState, char currentChar)
{
	// Check to see if we find a valid transition, if we do, return the transition info found
	if (transitions[(int)currentState].find(currentChar) != transitions[(int)currentState].end())
	{
		TransitionInfo info = transitions[(int)currentState][currentChar];
		currentToken.type = info.tokenType;
		return info.state;
	}

	// If we don't find a valid transition and the current state is NOT the start state, then the token is complete.
	else if (currentState != LexerState::StartState)
	{
		return LexerState::AcceptState;
	}

	// Now, if there's no valid transition, and we are in the start state, then we have an invalid character
	else
	{
		currentToken.type = TokenType::Invalid;
		return LexerState::InvalidState;
	}
}