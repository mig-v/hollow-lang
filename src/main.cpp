#include <iostream>
#include <chrono>
#include <iomanip>

#include "lexer.h"
#include "lexer_tests.h"
#include "parser_tests.h"
#include "bytecode_emitter_tests.h"
#include "compiler.h"

int main()
{
	Compiler compiler;

	LexerTests* lexerTests = new LexerTests();
	ParserTests* parserTests = new ParserTests();
	BytecodeEmitterTests* emitterTests = new BytecodeEmitterTests();

	auto start = std::chrono::high_resolution_clock::now();
	lexerTests->runAll();
	parserTests->runAll();
	emitterTests->runAll();

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "time taken: " << duration << " (ms)" << std::endl;
	
	compiler.compile(TEST_PATH"/parser_tests/temp.hollow");



	delete lexerTests;
	delete parserTests;
	delete emitterTests;

	return 0;
}