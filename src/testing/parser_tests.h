#pragma once

#include "parser/parser.h"

#include "lexer/lexer.h"

#include "core/memory_arena.h"
#include "core/diagnostic.h"

#include <vector>
#include <string>
#include <map>
#include <fstream>

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
	MemoryArena arena;
	DiagnosticReporter reporter;
	std::map<std::string, ParserTestInfo> testSuite;
	std::ofstream logFile;
};