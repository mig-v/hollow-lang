#include <iostream>
#include <chrono>

#include "lexer.h"
#include "lexer_tests.h"
#include "parser.h"
#include "parser_tests.h"
#include "semantic_analysis.h"

int main()
{
	Lexer lexer;
	Parser parser;

	LexerTests* lexerTests = new LexerTests();
	ParserTests* parserTests = new ParserTests();

	auto start = std::chrono::high_resolution_clock::now();
	lexerTests->runAll();
	parserTests->runAll();

	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::cout << "time taken: " << duration << " (ms)" << std::endl;

	if (lexer.lexFile(TEST_PATH"/parser_tests/temp.hollow"))
	{
		lexer.dumpTokens();
		parser.parse(lexer.getTokens());

		//SemanticAnalysis semanticAnalysis;
		//const std::vector<ASTNode*> ast = parser.getAst();

		//for (ASTNode* node : ast)
		//{
		//	node->accept(semanticAnalysis);
		//}

		//semanticAnalysis.printInfo();
		parser.printAST();
		parser.printErrors();
	}

	delete lexerTests;
	delete parserTests;

	return 0;
}