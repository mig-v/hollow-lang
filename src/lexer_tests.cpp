#include <iostream>

#include "lexer_tests.h"

LexerTests::LexerTests()
{
	testSuite[LEXER_TESTS_PATH"/keywords_test.hollow"].testname = "keywords_test.hollow";
	testSuite[LEXER_TESTS_PATH"/keywords_test.hollow"].expectedTokens =
	{ TokenType::i8Keyword, TokenType::Semicolon, TokenType::i16Keyword, TokenType::Semicolon,
	TokenType::i32Keyword, TokenType::Semicolon, TokenType::i64Keyword, TokenType::Semicolon,
	TokenType::u8Keyword, TokenType::Semicolon, TokenType::u16Keyword, TokenType::Semicolon,
	TokenType::u32Keyword, TokenType::Semicolon, TokenType::u64Keyword, TokenType::Semicolon,
	TokenType::f32Keyword, TokenType::Semicolon, TokenType::f64Keyword, TokenType::Semicolon,
	TokenType::returnKeyword, TokenType::Semicolon, TokenType::forKeyword, TokenType::Semicolon,
	TokenType::ifKeyword, TokenType::Semicolon, TokenType::whileKeyword, TokenType::Semicolon,
	TokenType::typeKeyword, TokenType::Semicolon, TokenType::funcKeyword, TokenType::Semicolon,
	TokenType::breakKeyword, TokenType::Semicolon, TokenType::boolKeyword, TokenType::Semicolon, 
	TokenType::trueKeyword, TokenType::Semicolon, TokenType::falseKeyword, TokenType::Semicolon,
	TokenType::charKeyword, TokenType::Semicolon, TokenType::Eof };
	
	testSuite[LEXER_TESTS_PATH"/literals_test.hollow"].testname = "literals_test.hollow";
	testSuite[LEXER_TESTS_PATH"/literals_test.hollow"].expectedTokens =
	{ TokenType::i32Keyword, TokenType::Identifier, TokenType::Assign, TokenType::IntLiteral,
	TokenType::Semicolon, TokenType::f32Keyword, TokenType::Identifier, TokenType::Assign,
	TokenType::FloatLiteral, TokenType::Semicolon, TokenType::f64Keyword, TokenType::Identifier,
	TokenType::Assign, TokenType::FloatLiteral, TokenType::Semicolon, TokenType::Eof };

	testSuite[LEXER_TESTS_PATH"/operators_test.hollow"].testname = "operators_test.hollow";
	testSuite[LEXER_TESTS_PATH"/operators_test.hollow"].expectedTokens =
	{ TokenType::GreaterThan, TokenType::Semicolon, TokenType::GreaterThanEq, TokenType::Semicolon,
	TokenType::LessThan, TokenType::Semicolon, TokenType::LessThanEq, TokenType::Semicolon,
	TokenType::Assign, TokenType::Semicolon, TokenType::Equality, TokenType::Semicolon,
	TokenType::LogicalNot, TokenType::Semicolon, TokenType::NotEq, TokenType::Semicolon,
	TokenType::OpenParen, TokenType::Semicolon, TokenType::CloseParen, TokenType::Semicolon,
	TokenType::OpenCurly, TokenType::Semicolon, TokenType::CloseCurly, TokenType::Semicolon,
	TokenType::Colon, TokenType::Semicolon, TokenType::Plus, TokenType::Semicolon, 
	TokenType::Minus, TokenType::Semicolon, TokenType::Asterisk, TokenType::Semicolon,
	TokenType::ForwardSlash, TokenType::Semicolon, TokenType::Semicolon, TokenType::Eof };

	testSuite[LEXER_TESTS_PATH"/function_test.hollow"].testname = "function_test.hollow";
	testSuite[LEXER_TESTS_PATH"/function_test.hollow"].expectedTokens =
	{ TokenType::funcKeyword, TokenType::Identifier, TokenType::OpenParen, TokenType::CloseParen,
	TokenType::Colon, TokenType::i32Keyword, TokenType::OpenCurly, TokenType::i32Keyword,
	TokenType::Identifier, TokenType::Semicolon, TokenType::returnKeyword, TokenType::Identifier,
	TokenType::Semicolon, TokenType::CloseCurly, TokenType::Eof };
}

void LexerTests::runAll()
{	
	for (auto const& [key, val] : testSuite)
		runSpecific(key);
}

void LexerTests::runSpecific(const std::string& filename)
{
	Token* token = nullptr;
	std::vector<TokenType>& expected = testSuite[filename].expectedTokens;
	size_t expectedTokenCount = expected.size();
	bool passed = true;

	lexer.reset();
	lexer.lexFile(filename);

	for (size_t i = 0; i < expectedTokenCount; i++)
	{
		token = lexer.getToken();
		if (!token || token->type != expected[i])
		{
			if (token)
				std::cout << "expected " << (int)token->type << " got " << (int)expected[i] << " at index: " << i << std::endl;
			else
				std::cout << "token was null, indicating more tokens than expected" << std::endl;

			passed = false;
			break;
		}
	}

	if (passed) std::cout << testSuite[filename].testname << " ... PASSED" << std::endl;
	else		std::cout << testSuite[filename].testname << " ... FAILED" << std::endl;
}