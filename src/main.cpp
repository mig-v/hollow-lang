#include "lexer/lexer.h"

#include "testing/lexer_tests.h"
#include "testing/parser_tests.h"

#include "archive/bytecode_emitter_tests.h"

#include "core/compiler.h"

#include <iostream>
#include <chrono>
#include <iomanip>

struct Point {};

int main()
{
	Compiler compiler;

	LexerTests* lexerTests = new LexerTests();
	ParserTests* parserTests = new ParserTests();

	auto start = std::chrono::high_resolution_clock::now();
	lexerTests->runAll();
	parserTests->runAll();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "time taken: " << duration << " (ms)" << std::endl;
	
	compiler.compile(TEST_PATH"/parser_tests/temp.hollow");

	delete lexerTests;
	delete parserTests;

	return 0;
}