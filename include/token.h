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
	elseKeyword,
	continueKeyword,

	IntLiteral,
	FloatLiteral,
	CharLiteral,

	Identifier,

	Assign,
	Comment,

	OpenParen,
	CloseParen,
	OpenCurly,
	CloseCurly,
	OpenBracket,
	CloseBracket,

	Semicolon,
	Colon,
	Comma,

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

	LogicalOr,
	LogicalAnd,

	BitwiseOr,
	BitwiseAnd,
	BitwiseXor,
	BitwiseNot,
	BitwiseLeftShift,
	BitwiseRightShift,
	BitwiseOrEquals,
	BitwiseAndEquals,
	BitwiseXorEquals,
	BitwiseLeftShiftEquals,
	BitwiseRightShiftEquals,

	Increment,
	Decrement,
	PlusEquals,
	MinusEquals,
	TimesEquals,
	DividedEquals,
	DotOperator,

	// Note: This token type is never emitted, it is only here to make lexing escaped chars easier and is later converted to CharLiteral
	EscapedCharLiteral,	

	Invalid,
	Eof,
	NumTokens
};

struct Token
{
	TokenType type;
	std::variant<char, uint64_t, double, std::string> value;
	uint32_t lineNum;
	uint32_t column;
	/*
	
	What are possible values of a token?
	char, int, float, string for identifiers
	All other values can be deduced from their token types such as all keywords, and any operators / paren / curly / brackets
	
	*/
};