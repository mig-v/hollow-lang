#include <iostream>

#include "lexer_tests.h"

LexerTests::LexerTests()
{
	tokenTypeToStr[(int)TokenType::i8Keyword] = "<i8Keyword>";
	tokenTypeToStr[(int)TokenType::i16Keyword] = "<i16Keyword>";
	tokenTypeToStr[(int)TokenType::i32Keyword] = "<i32Keyword>";
	tokenTypeToStr[(int)TokenType::i64Keyword] = "<i64Keyword>";
	tokenTypeToStr[(int)TokenType::u8Keyword] = "<u8Keyword>";
	tokenTypeToStr[(int)TokenType::u16Keyword] = "<u16Keyword>";
	tokenTypeToStr[(int)TokenType::u32Keyword] = "<u32Keyword>";
	tokenTypeToStr[(int)TokenType::u64Keyword] = "<u64Keyword>";
	tokenTypeToStr[(int)TokenType::f32Keyword] = "<f32Keyword>";
	tokenTypeToStr[(int)TokenType::f64Keyword] = "<f64Keyword>";
	tokenTypeToStr[(int)TokenType::boolKeyword] = "<boolKeyword>";
	tokenTypeToStr[(int)TokenType::trueKeyword] = "<trueKeyword>";
	tokenTypeToStr[(int)TokenType::falseKeyword] = "<falseKeyword>";
	tokenTypeToStr[(int)TokenType::charKeyword] = "<charKeyword>";
	tokenTypeToStr[(int)TokenType::typeKeyword] = "<typeKeyword>";
	tokenTypeToStr[(int)TokenType::funcKeyword] = "<funcKeyword>";
	tokenTypeToStr[(int)TokenType::returnKeyword] = "<returnKeyword>";
	tokenTypeToStr[(int)TokenType::breakKeyword] = "<breakKeyword>";
	tokenTypeToStr[(int)TokenType::ifKeyword] = "<ifKeyword>";
	tokenTypeToStr[(int)TokenType::forKeyword] = "<forKeyword>";
	tokenTypeToStr[(int)TokenType::whileKeyword] = "<whileKeyword>";
	tokenTypeToStr[(int)TokenType::IntLiteral] = "<IntLiteral>";
	tokenTypeToStr[(int)TokenType::FloatLiteral] = "<FloatLiteral>";
	tokenTypeToStr[(int)TokenType::CharLiteral] = "<CharLiteral>";
	tokenTypeToStr[(int)TokenType::Identifier] = "<Identifier>";
	tokenTypeToStr[(int)TokenType::Assign] = "<Assign>";
	tokenTypeToStr[(int)TokenType::OpenParen] = "<OpenParen>";
	tokenTypeToStr[(int)TokenType::CloseParen] = "<CloseParen>";
	tokenTypeToStr[(int)TokenType::OpenCurly] = "<OpenCurly>";
	tokenTypeToStr[(int)TokenType::CloseCurly] = "<CloseCurly>";
	tokenTypeToStr[(int)TokenType::Semicolon] = "<Semicolon>";
	tokenTypeToStr[(int)TokenType::Colon] = "<Colon>";
	tokenTypeToStr[(int)TokenType::LessThan] = "<LessThan>";
	tokenTypeToStr[(int)TokenType::LessThanEq] = "<LessThanEq>";
	tokenTypeToStr[(int)TokenType::GreaterThan] = "<GreaterThan>";
	tokenTypeToStr[(int)TokenType::GreaterThanEq] = "<GreaterThanEq>";
	tokenTypeToStr[(int)TokenType::LogicalNot] = "<LogicalNot>";
	tokenTypeToStr[(int)TokenType::NotEq] = "<NotEq>";
	tokenTypeToStr[(int)TokenType::Equality] = "<Equality>";
	tokenTypeToStr[(int)TokenType::Plus] = "<Plus>";
	tokenTypeToStr[(int)TokenType::Minus] = "<Minus>";
	tokenTypeToStr[(int)TokenType::Asterisk] = "<Asterisk>";
	tokenTypeToStr[(int)TokenType::ForwardSlash] = "<ForwardSlash>";
	tokenTypeToStr[(int)TokenType::Invalid] = "<Invalid>";
	tokenTypeToStr[(int)TokenType::Eof] = "<EOF>";

	testSuite[TEST_PATH"/lexer_tests/keywords_test.hollow"].testname = "keywords_test.hollow";
	testSuite[TEST_PATH"/lexer_tests/keywords_test.hollow"].expectedTokens =
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
	
	testSuite[TEST_PATH"/lexer_tests/literals_test.hollow"].testname = "literals_test.hollow";
	testSuite[TEST_PATH"/lexer_tests/literals_test.hollow"].expectedTokens =
	{ TokenType::i32Keyword, TokenType::Identifier, TokenType::Assign, TokenType::IntLiteral,
	TokenType::Semicolon, TokenType::f32Keyword, TokenType::Identifier, TokenType::Assign,
	TokenType::FloatLiteral, TokenType::Semicolon, TokenType::f64Keyword, TokenType::Identifier,
	TokenType::Assign, TokenType::FloatLiteral, TokenType::Semicolon, TokenType::charKeyword,
	TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral, TokenType::Semicolon,
	TokenType::charKeyword, TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral,
	TokenType::Semicolon, TokenType::charKeyword, TokenType::Identifier, TokenType::Assign,
	TokenType::CharLiteral, TokenType::Semicolon, TokenType::charKeyword, TokenType::Identifier,
	TokenType::Assign, TokenType::CharLiteral, TokenType::Semicolon, TokenType::charKeyword,
	TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral, TokenType::Semicolon,
	TokenType::charKeyword, TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral,
	TokenType::Semicolon, TokenType::charKeyword, TokenType::Identifier, TokenType::Assign,
	TokenType::CharLiteral, TokenType::Semicolon, TokenType::charKeyword, TokenType::Identifier,
	TokenType::Assign, TokenType::CharLiteral, TokenType::Semicolon, TokenType::charKeyword,
	TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral, TokenType::Semicolon,
	TokenType::charKeyword, TokenType::Identifier, TokenType::Assign, TokenType::CharLiteral,
	TokenType::Semicolon, TokenType::charKeyword, TokenType::Identifier, TokenType::Assign,
	TokenType::Invalid, TokenType::charKeyword, TokenType::Identifier,
	TokenType::Assign, TokenType::Invalid, TokenType::Semicolon, TokenType::charKeyword,
	TokenType::Identifier, TokenType::Assign, TokenType::Invalid, TokenType::Semicolon, TokenType::Eof };

	testSuite[TEST_PATH"/lexer_tests/operators_test.hollow"].testname = "operators_test.hollow";
	testSuite[TEST_PATH"/lexer_tests/operators_test.hollow"].expectedTokens =
	{ TokenType::GreaterThan, TokenType::Semicolon, TokenType::GreaterThanEq, TokenType::Semicolon,
	TokenType::LessThan, TokenType::Semicolon, TokenType::LessThanEq, TokenType::Semicolon,
	TokenType::Assign, TokenType::Semicolon, TokenType::Equality, TokenType::Semicolon,
	TokenType::LogicalNot, TokenType::Semicolon, TokenType::NotEq, TokenType::Semicolon,
	TokenType::OpenParen, TokenType::Semicolon, TokenType::CloseParen, TokenType::Semicolon,
	TokenType::OpenCurly, TokenType::Semicolon, TokenType::CloseCurly, TokenType::Semicolon,
	TokenType::Colon, TokenType::Semicolon, TokenType::Plus, TokenType::Semicolon, 
	TokenType::Minus, TokenType::Semicolon, TokenType::Asterisk, TokenType::Semicolon,
	TokenType::ForwardSlash, TokenType::Semicolon, TokenType::Semicolon, TokenType::Eof };

	testSuite[TEST_PATH"/lexer_tests/function_test.hollow"].testname = "function_test.hollow";
	testSuite[TEST_PATH"/lexer_tests/function_test.hollow"].expectedTokens =
	{ TokenType::funcKeyword, TokenType::Identifier, TokenType::OpenParen, TokenType::CloseParen,
	TokenType::Colon, TokenType::i32Keyword, TokenType::OpenCurly, TokenType::i32Keyword,
	TokenType::Identifier, TokenType::Semicolon, TokenType::returnKeyword, TokenType::Identifier,
	TokenType::Semicolon, TokenType::CloseCurly, TokenType::Eof };

	testSuite[TEST_PATH"/lexer_tests/control_flow_test.hollow"].testname = "control_flow_test.hollow";
	testSuite[TEST_PATH"/lexer_tests/control_flow_test.hollow"].expectedTokens =
	{ TokenType::forKeyword, TokenType::OpenParen, TokenType::i32Keyword, TokenType::Identifier,
	TokenType::Assign, TokenType::IntLiteral, TokenType::Semicolon, TokenType::Identifier,
	TokenType::LessThan, TokenType::IntLiteral, TokenType::Semicolon, TokenType::Identifier,
	TokenType::Assign, TokenType::Identifier, TokenType::Plus, TokenType::IntLiteral,
	TokenType::CloseParen, TokenType::OpenCurly, TokenType::ifKeyword, TokenType::OpenParen,
	TokenType::Identifier, TokenType::Equality, TokenType::IntLiteral, TokenType::CloseParen,
	TokenType::breakKeyword, TokenType::Semicolon, TokenType::elseKeyword, TokenType::continueKeyword,
	TokenType::Semicolon, TokenType::CloseCurly, TokenType::i32Keyword, TokenType::Identifier,
	TokenType::Assign, TokenType::IntLiteral, TokenType::Semicolon, TokenType::i32Keyword,
	TokenType::Identifier, TokenType::Assign, TokenType::IntLiteral, TokenType::Semicolon,
	TokenType::whileKeyword, TokenType::OpenParen, TokenType::Identifier, TokenType::LessThan,
	TokenType::Identifier, TokenType::CloseParen, TokenType::OpenCurly, TokenType::Identifier,
	TokenType::Assign, TokenType::Identifier, TokenType::Plus, TokenType::IntLiteral,
	TokenType::Semicolon, TokenType::ifKeyword, TokenType::OpenParen, TokenType::Identifier,
	TokenType::Asterisk, TokenType::IntLiteral, TokenType::Equality, TokenType::Identifier,
	TokenType::ForwardSlash, TokenType::IntLiteral, TokenType::CloseParen, TokenType::breakKeyword,
	TokenType::Semicolon, TokenType::CloseCurly, TokenType::Eof };
}

void LexerTests::runAll()
{	
	logFile.open(TEST_PATH"/lexer_tests/lexer_log.txt");

	std::cout << "===== Running Lexer Test Suite =====\n";
	for (auto const& [key, val] : testSuite)
		runSpecific(key);

	logFile.close();
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
			passed = false;

			if (logFile.is_open())
			{
				if (token) 
					logFile << "(line " << token->lineNum << ") expected " << tokenTypeToStr[(int)token->type] << ", got " << tokenTypeToStr[(int)expected[i]] << std::endl;
				else
					logFile << "token stream ended, extra token: " << tokenTypeToStr[(int)expected[i]] << std::endl;
			}
		}
	}

	if (passed)
	{
		std::cout << testSuite[filename].testname << " ... PASSED" << std::endl;

		if (logFile.is_open())
			logFile << testSuite[filename].testname << " ... PASSED" << std::endl << std::endl;
	}
	else
	{
		std::cout << testSuite[filename].testname << " ... FAILED" << std::endl;

		if (logFile.is_open())
			logFile << testSuite[filename].testname << " ... FAILED" << std::endl << std::endl;
	}
}