#include <iostream>
#include <chrono>

#include "lexer.h"
#include "lexer_tests.h"

int main()
{
	std::cout << "Hello hollow-lang!" << std::endl;

	Lexer lexer;

	auto start = std::chrono::high_resolution_clock::now();

	if (lexer.lexFile(LEXER_TESTS_PATH"/operators_test.hollow"))
	{
		lexer.dumpTokens();
	}

	LexerTests tests;
	tests.runAll();

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::cout << "time taken: " << duration << " (ms)" << std::endl;

	return 0;
}