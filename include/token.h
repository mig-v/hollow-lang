#pragma once

#include <variant>
#include <string>

enum class TokenType
{
	None,

	i8Keyword,
	i16Keyword,
	i32Keyword,
	i64Keyword,

	u8Keyword,
	u16Keyword,
	u32Keyword,
	u64Keyword,

	f32Keyword,
	f64Keyword,

	// No BoolLiteral needed, as trueKeyword and falseKeyword serve the same purpose and these are the only two possibilities for a bool
	boolKeyword,
	trueKeyword,
	falseKeyword,

	charKeyword,

	typeKeyword,
	funcKeyword,
	returnKeyword,
	breakKeyword,

	ifKeyword,
	forKeyword,
	whileKeyword,

	IntLiteral,
	FloatLiteral,
	CharLiteral,

	Identifier,

	Assign,

	OpenParen,
	CloseParen,
	OpenCurly,
	CloseCurly,

	Semicolon,
	Colon,

	LessThan,
	GreaterThan,
	LessThanEq,
	GreaterThanEq,
	LogicalNot,
	NotEq,
	Equality,

	Plus,
	Minus,
	Asterisk,
	ForwardSlash,

	Invalid,
	Eof
};

struct Token
{
	TokenType type;
	std::variant<char, uint64_t, double, std::string> value;
	/*
	
	What are possible values of a token?
	char, int, float, string for identifiers
	All other values can be deduced from their token types such as all keywords, and any operators / paren / curly / brackets
	
	*/
};