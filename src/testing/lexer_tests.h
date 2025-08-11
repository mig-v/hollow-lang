#pragma once

#include "lexer/token.h"
#include "lexer/lexer.h"

#include <array>
#include <vector>
#include <string>
#include <map>
#include <fstream>

struct LexerTestInfo
{
	std::string testname;
	std::vector<TokenType> expectedTokens;
};

class LexerTests
{
public:
	LexerTests();
	void runAll();

private:
	void runSpecific(const std::string& filename);

	Lexer lexer;
	std::map<std::string, LexerTestInfo> testSuite;
	std::array<std::string, (size_t)TokenType::NumTokens> tokenTypeToStr;
	std::ofstream logFile;
};