#pragma once

#include <vector>
#include <string>
#include <map>

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
	void runSpecific(const std::string& filename);

private:
	Lexer lexer;
	std::map<std::string, TestInfo> testSuite;
};