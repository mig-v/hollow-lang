#include "token_stream.h"

void TokenStream::init(std::vector<Token>& tokens)
{
	this->tokens = std::move(tokens);
	this->current = 0;

	this->nullToken.lineNum = 1;
	this->nullToken.type = TokenType::Eof;
	this->nullToken.column = 1;
}

Token TokenStream::consume()
{
	if (!tokensInStream())
		return nullToken;

	Token ret = tokens[current];
	current++;
	return ret;
}

Token TokenStream::get()
{
	if (!tokensInStream())
		return nullToken;
	
	return tokens[current];
}

Token TokenStream::prev()
{
	if (current == 0)
		return nullToken;

	return tokens[current - 1];
}

bool TokenStream::tokensInStream()
{
	return current < tokens.size();
}

bool TokenStream::peek(TokenType type)
{
	if (current >= tokens.size())
		return false;

	return tokens[current].type == type;
}

bool TokenStream::lookAhead(TokenType type)
{
	if ((current + 1) >= tokens.size())
		return false;

	return tokens[current + 1].type == type;
}

// used when we need to match the top of the token stream with multiple valid token options
// E.G. when checking if a primitive type is at the top of the token stream, we would pass in all primitive types
// peek should be used when only needing to check for a single token type
bool TokenStream::matchToken(std::initializer_list<TokenType> list)
{
	if (current >= tokens.size())
		return false;

	for (TokenType type : list)
	{
		if (tokens[current].type == type)
			return true;
	}

	return false;
}

bool TokenStream::topIsPrimitive()
{
	switch (get().type)
	{
		case TokenType::i8Keyword:
		case TokenType::i16Keyword:
		case TokenType::i32Keyword:
		case TokenType::i64Keyword:
		case TokenType::u8Keyword:
		case TokenType::u16Keyword:
		case TokenType::u32Keyword:
		case TokenType::u64Keyword:
		case TokenType::f32Keyword:
		case TokenType::f64Keyword:
		case TokenType::charKeyword:
		case TokenType::boolKeyword:
			return true;
		default:
			return false;
	}
}

bool TokenStream::topIsAssignment()
{
	switch (get().type)
	{
		case TokenType::Assign:
		case TokenType::PlusEquals:
		case TokenType::MinusEquals:
		case TokenType::TimesEquals:
		case TokenType::DividedEquals:
		case TokenType::BitwiseAndEquals:
		case TokenType::BitwiseOrEquals:
		case TokenType::BitwiseXorEquals:
		case TokenType::BitwiseLeftShiftEquals:
		case TokenType::BitwiseRightShiftEquals:
			return true;
		default:
			return false;
	}
}