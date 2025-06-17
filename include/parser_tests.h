#pragma once

#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "parser.h"
#include "lexer.h"

struct ParserTestInfo
{
	std::string testname;
	std::vector<ASTNode*> expectedAst;
};

class ParserTests
{
public:
	ParserTests();
	~ParserTests();
	void runAll();

private:
	void runSpecific(const std::string& filename);

	Lexer lexer;
	Parser parser;
	std::map<std::string, ParserTestInfo> testSuite;
	std::ofstream logFile;
};