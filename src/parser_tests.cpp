#include "parser_tests.h"
#include "ast_printer.h"

#include <iostream>

ParserTests::ParserTests()
{
	testSuite[TEST_PATH"/parser_tests/literals_test.hollow"].testname = "literals_test.hollow";
	testSuite[TEST_PATH"/parser_tests/literals_test.hollow"].expectedAst =
	{
		new ASTExprStmt(new ASTIntLiteral(100)), new ASTExprStmt(new ASTDoubleLiteral(100.0)), new ASTExprStmt(new ASTBoolLiteral(true)),
		new ASTExprStmt(new ASTBoolLiteral(false)), new ASTExprStmt(new ASTCharLiteral('a')), new ASTExprStmt(new ASTCharLiteral('\n')),
		new ASTExprStmt(new ASTCharLiteral(';')), new ASTExprStmt(new ASTCharLiteral('\\')), new ASTExprStmt(new ASTCharLiteral('\''))
	};
}

ParserTests::~ParserTests()
{
	// clean up all of the expected AST vectors since we're storing raw ASTNode* in them
	for (auto const& [key, val] : testSuite)
	{
		std::vector<ASTNode*> ast = testSuite[key].expectedAst;
		for (ASTNode* node : ast)
		{
			
		}
	}
}

void ParserTests::runAll()
{
	logFile.open(TEST_PATH"/parser_tests/parser_log.txt");

	std::cout << "===== Running Parser Test Suite =====\n";
	for (auto const& [key, val] : testSuite)
		runSpecific(key);

	logFile.close();
}

void ParserTests::runSpecific(const std::string & filename)
{
	lexer.reset();
	lexer.lexFile(filename);
	parser.parse(lexer.getTokens());

	const std::vector<ASTNode*> ast = parser.getAst();
	const std::vector<ASTNode*>& expectedAst = testSuite[filename].expectedAst;
	size_t expectedNodeCount = expectedAst.size();
	bool passed = true;

	for (size_t i = 0; i < expectedNodeCount; i++)
	{
		if (!(*ast[i] == *expectedAst[i]))
		{
			ASTPrinter visitor;
			passed = false;

			std::cout << "Got: ";
			ast[i]->accept(visitor);

			std::cout << "Expected: ";
			expectedAst[i]->accept(visitor);
		}
	}

	if (passed)
	{
		std::cout << testSuite[filename].testname << " ... PASSED" << std::endl;

		if (logFile.is_open())
			logFile << testSuite[filename].testname << " ... PASSED" << std::endl << std::endl;
	}
	else
	{
		std::cout << testSuite[filename].testname << " ... FAILED" << std::endl;

		if (logFile.is_open())
			logFile << testSuite[filename].testname << " ... FAILED" << std::endl << std::endl;
	}

}