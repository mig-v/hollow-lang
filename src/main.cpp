#include <iostream>
#include <chrono>

#include "lexer.h"
#include "lexer_tests.h"
#include "parser.h"

int main()
{
	Lexer lexer;
	Parser parser;

	auto start = std::chrono::high_resolution_clock::now();

	if (lexer.lexFile(TEST_PATH"/parser_tests/example.hollow"))
	{
		lexer.dumpTokens();
		parser.parse(lexer.getTokens());
		parser.printAST();
		parser.printErrors();
	}

	//LexerTests tests;
	//tests.runAll();

	//auto end = std::chrono::high_resolution_clock::now();

	//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	//std::cout << "time taken: " << duration << " (ms)" << std::endl;

	return 0;
}