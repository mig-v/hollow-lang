#pragma once

#include <array>
#include <vector>
#include <string>
#include <map>
#include <fstream>

#include <token.h>
#include "lexer.h"

struct TestInfo
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
	std::map<std::string, TestInfo> testSuite;
	std::array<std::string, (size_t)TokenType::NumTokens> tokenTypeToStr;
	std::ofstream logFile;
};