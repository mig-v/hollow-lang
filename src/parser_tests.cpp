#include "parser_tests.h"
#include "ast_printer.h"
#include "ast_type.h"

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

	Token add;
	add.type = TokenType::Plus;
	Token sub;
	sub.type = TokenType::Minus;
	Token mul;
	mul.type = TokenType::Asterisk;
	Token div;
	div.type = TokenType::ForwardSlash;
	Token bitAnd;
	bitAnd.type = TokenType::BitwiseAnd;
	Token bitOr;
	bitOr.type = TokenType::BitwiseOr;
	Token bitXor;
	bitXor.type = TokenType::BitwiseXor;
	Token equality;
	equality.type = TokenType::Equality;
	Token lt;
	lt.type = TokenType::LessThan;
	Token gt;
	gt.type = TokenType::GreaterThan;
	Token notEq;
	notEq.type = TokenType::NotEq;
	Token logicalAnd;
	logicalAnd.type = TokenType::LogicalAnd;
	Token logicalOr;
	logicalOr.type = TokenType::LogicalOr;
	Token logicalNot;
	logicalNot.type = TokenType::LogicalNot;
	Token bitNot;
	bitNot.type = TokenType::BitwiseNot;
	Token decrement;
	decrement.type = TokenType::Decrement;

	testSuite[TEST_PATH"/parser_tests/binary_expressions_test.hollow"].testname = "binary_expressions_test.hollow";
	testSuite[TEST_PATH"/parser_tests/binary_expressions_test.hollow"].expectedAst =
	{
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(10), add, new ASTIntLiteral(20))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(10), add, new ASTBinaryExpr(new ASTIntLiteral(20), mul, new ASTIntLiteral(30)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(10), mul, new ASTIntLiteral(20)), add, new ASTIntLiteral(30))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(10), sub, new ASTBinaryExpr(new ASTIntLiteral(20), div, new ASTIntLiteral(30)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(10), div, new ASTIntLiteral(5)), sub, new ASTIntLiteral(30))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(10), bitAnd, new ASTIntLiteral(3)), bitOr, new ASTBinaryExpr(new ASTIntLiteral(5), bitXor, new ASTIntLiteral(2)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(2), add, new ASTBinaryExpr(new ASTIntLiteral(3), mul, new ASTIntLiteral(4))), equality, new ASTIntLiteral(14))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), sub, new ASTIntLiteral(2)), sub, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(8), div, new ASTIntLiteral(4)), div, new ASTIntLiteral(2))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(10), bitAnd, new ASTBinaryExpr(new ASTIntLiteral(3), add, new ASTIntLiteral(2)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(8), bitOr, new ASTBinaryExpr(new ASTIntLiteral(4), mul, new ASTIntLiteral(2)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(6), bitXor, new ASTBinaryExpr(new ASTIntLiteral(2), add, new ASTIntLiteral(1)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2)), equality, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(4), mul, new ASTIntLiteral(2)), lt, new ASTIntLiteral(10))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(6), div, new ASTIntLiteral(3)), notEq, new ASTIntLiteral(2))),
		new ASTExprStmt(new ASTLogical(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2)), logicalAnd, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTLogical(new ASTBinaryExpr(new ASTIntLiteral(4), mul, new ASTIntLiteral(2)), logicalOr, new ASTIntLiteral(0))),
		new ASTExprStmt(new ASTLogical(new ASTBinaryExpr(new ASTIntLiteral(1), equality, new ASTIntLiteral(1)), logicalOr, new ASTIntLiteral(0))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2))), mul, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(4), sub, new ASTIntLiteral(1))), mul, new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(2), add, new ASTIntLiteral(2))))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(5), add, new ASTIntLiteral(3))), bitAnd, new ASTIntLiteral(7))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), bitAnd, new ASTIntLiteral(2)), equality, new ASTIntLiteral(0))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2)), bitOr, new ASTBinaryExpr(new ASTIntLiteral(4), bitXor, new ASTIntLiteral(3)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), bitAnd, new ASTIntLiteral(2)), bitAnd, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), bitOr, new ASTIntLiteral(2)), bitOr, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), bitXor, new ASTIntLiteral(2)), bitXor, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIntLiteral(1), equality, new ASTBinaryExpr(new ASTIntLiteral(2), lt, new ASTIntLiteral(3)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), lt, new ASTIntLiteral(2)), equality, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTLogical(new ASTIntLiteral(1), logicalOr, new ASTLogical(new ASTIntLiteral(2), logicalAnd, new ASTIntLiteral(3)))),
		new ASTExprStmt(new ASTLogical(new ASTLogical(new ASTIntLiteral(1), logicalAnd, new ASTIntLiteral(2)), logicalOr, new ASTIntLiteral(3))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTBinaryExpr(new ASTBinaryExpr(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2)), add, new ASTIntLiteral(3)), add, new ASTIntLiteral(4)), add, new ASTIntLiteral(5)), add, new ASTIntLiteral(6)), add, new ASTIntLiteral(7))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2))), mul, new ASTIntLiteral(3))), sub, new ASTIntLiteral(4))), div, new ASTIntLiteral(5)))
	};

	testSuite[TEST_PATH"/parser_tests/unary_expressions_test.hollow"].testname = "unary_expressions_test.hollow";
	testSuite[TEST_PATH"/parser_tests/unary_expressions_test.hollow"].expectedAst =
	{
		new ASTExprStmt(new ASTUnaryExpr(sub, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTUnaryExpr(sub, new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2))))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(sub, new ASTIntLiteral(1)), add, new ASTIntLiteral(2))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(sub, new ASTIntLiteral(1)), add, new ASTUnaryExpr(sub, new ASTIntLiteral(2)))),
		new ASTExprStmt(new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(true))),
		new ASTExprStmt(new ASTUnaryExpr(logicalNot, new ASTGroupExpr(new ASTBoolLiteral(false)))),
		new ASTExprStmt(new ASTLogical(new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(true)), logicalAnd, new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(false)))),
		new ASTExprStmt(new ASTUnaryExpr(logicalNot, new ASTGroupExpr(new ASTLogical(new ASTBoolLiteral(true), logicalAnd, new ASTBoolLiteral(false))))),
		new ASTExprStmt(new ASTUnaryExpr(bitNot, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTUnaryExpr(bitNot, new ASTGroupExpr(new ASTIntLiteral(1)))),
		new ASTExprStmt(new ASTUnaryExpr(bitNot, new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(1), add, new ASTIntLiteral(2))))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(bitNot, new ASTIntLiteral(1)), add, new ASTUnaryExpr(bitNot, new ASTIntLiteral(2)))),
		new ASTExprStmt(new ASTUnaryExpr(decrement, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTUnaryExpr(logicalNot, new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(true)))),
		new ASTExprStmt(new ASTUnaryExpr(bitNot, new ASTUnaryExpr(sub, new ASTIntLiteral(1)))),
		new ASTExprStmt(new ASTUnaryExpr(sub, new ASTUnaryExpr(bitNot, new ASTIntLiteral(1)))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(sub, new ASTIntLiteral(1)), mul, new ASTIntLiteral(2))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(bitNot, new ASTIntLiteral(1)), bitAnd, new ASTIntLiteral(2))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(true)), equality, new ASTBoolLiteral(false)))
	};

	Token x;
	x.value = "x";
	Token y;
	y.value = "y";
	Token flag;
	flag.value = "flag";
	Token z;
	z.value = "z";
	Token flag2;
	flag2.value = "flag2";
	Token assign;
	assign.type = TokenType::Assign;
	Token plusEq;
	Token minusEq;
	Token timesEq;
	Token divEq;
	Token andEq;
	Token orEq;
	Token xorEq;
	Token leftShiftEq;
	Token rightShiftEq;
	Token increment;
	plusEq.type = TokenType::PlusEquals;
	minusEq.type = TokenType::MinusEquals;
	timesEq.type = TokenType::TimesEquals;
	divEq.type = TokenType::DividedEquals;
	andEq.type = TokenType::BitwiseAndEquals;
	orEq.type = TokenType::BitwiseOrEquals;
	xorEq.type = TokenType::BitwiseXorEquals;
	leftShiftEq.type = TokenType::BitwiseLeftShiftEquals;
	rightShiftEq.type = TokenType::BitwiseRightShiftEquals;
	increment.type = TokenType::Increment;
	
	testSuite[TEST_PATH"/parser_tests/variables_test.hollow"].testname = "variables_test.hollow";
	testSuite[TEST_PATH"/parser_tests/variables_test.hollow"].expectedAst =
	{
		new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), x, nullptr),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::f32Keyword), y, new ASTDoubleLiteral(1.0)),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::boolKeyword), flag, new ASTBoolLiteral(true)),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), z, new ASTIntLiteral(1)),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::boolKeyword), flag2, new ASTBoolLiteral(false)),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), plusEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), minusEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), timesEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), divEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), andEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), orEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), xorEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), leftShiftEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), rightShiftEq, new ASTIntLiteral(1))),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTBinaryExpr(new ASTIdentifier(y), add, new ASTIntLiteral(1)))),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), x, new ASTBinaryExpr(new ASTIntLiteral(10), add, new ASTIntLiteral(20))),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), y, new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(5), mul, new ASTIntLiteral(3))), sub, new ASTIntLiteral(1))),
		new ASTVarDecl(new ASTPrimitiveType(TokenType::boolKeyword), z, new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(false))),
		new ASTExprStmt(new ASTIdentifier(x)),
		new ASTExprStmt(new ASTBinaryExpr(new ASTIdentifier(x), add, new ASTIntLiteral(5))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTBinaryExpr(new ASTIdentifier(x), mul, new ASTIdentifier(y)), add, new ASTIdentifier(z))),
		new ASTExprStmt(new ASTGroupExpr(new ASTIdentifier(x))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTGroupExpr(new ASTBinaryExpr(new ASTIdentifier(x), add, new ASTIdentifier(y))), mul, new ASTIdentifier(z))),
		new ASTExprStmt(new ASTBinaryExpr(new ASTUnaryExpr(bitNot, new ASTGroupExpr(new ASTBinaryExpr(new ASTIdentifier(x), add, new ASTIdentifier(y)))), add, new ASTUnaryExpr(bitNot, new ASTGroupExpr(new ASTBinaryExpr(new ASTIdentifier(y), add, new ASTIdentifier(z)))))),
		new ASTExprStmt(new ASTUnaryExpr(logicalNot, new ASTGroupExpr(new ASTBinaryExpr(new ASTIdentifier(x), add, new ASTIdentifier(y))))),
		new ASTExprStmt(new ASTPostfix(new ASTIdentifier(x), increment)),
		new ASTExprStmt(new ASTUnaryExpr(decrement, new ASTIdentifier(x)))
	};

	ASTBlock* trueBranch = new ASTBlock();
	trueBranch->statements.emplace_back(new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTIntLiteral(10))));
	trueBranch->statements.emplace_back(new ASTExprStmt(new ASTAssign(new ASTIdentifier(y), assign, new ASTIdentifier(x))));

	ASTBlock* falseBranch = new ASTBlock();
	falseBranch->statements.emplace_back(new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTUnaryExpr(bitNot, new ASTIdentifier(y)))));
	falseBranch->statements.emplace_back(new ASTExprStmt(new ASTAssign(new ASTIdentifier(z), assign, new ASTIdentifier(x))));

	ASTBlock* empty = new ASTBlock();
	ASTBlock* nestedIf = new ASTBlock();
	nestedIf->statements.emplace_back(new ASTIfStatement(new ASTIdentifier(y), trueBranch, falseBranch));
	ASTBlock* infiniteFor = new ASTBlock();
	infiniteFor->statements.emplace_back(new ASTForLoop(nullptr, nullptr, nullptr, empty));

	testSuite[TEST_PATH"/parser_tests/control_flow_test.hollow"].testname = "control_flow_test.hollow";
	testSuite[TEST_PATH"/parser_tests/control_flow_test.hollow"].expectedAst =
	{
		new ASTIfStatement(new ASTIdentifier(x), new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), plusEq, new ASTIntLiteral(10))), new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), plusEq, new ASTIntLiteral(5)))),
		new ASTIfStatement(new ASTBoolLiteral(true), trueBranch, falseBranch),
		new ASTIfStatement(new ASTLogical(new ASTBoolLiteral(true), logicalOr, new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(false))), new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTUnaryExpr(bitNot, new ASTGroupExpr(new ASTBinaryExpr(new ASTIdentifier(y), mul, new ASTIdentifier(z)))))), nullptr),
		new ASTIfStatement(new ASTBoolLiteral(true), empty, empty),
		new ASTIfStatement(new ASTIdentifier(x), nestedIf, falseBranch),
		new ASTIfStatement(new ASTBoolLiteral(true), trueBranch, new ASTIfStatement(new ASTUnaryExpr(logicalNot, new ASTBoolLiteral(false)), trueBranch, trueBranch)),
		new ASTForLoop(new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), x, new ASTIntLiteral(0)), new ASTBinaryExpr(new ASTIdentifier(x), lt, new ASTIntLiteral(10)), new ASTPostfix(new ASTIdentifier(x), increment), empty),
		new ASTForLoop(nullptr, nullptr, nullptr, empty),
		new ASTForLoop(nullptr, nullptr, nullptr, infiniteFor),
		new ASTWhileLoop(new ASTBinaryExpr(new ASTIdentifier(x), lt, new ASTIntLiteral(10)), trueBranch)
	};

	Token foo;
	foo.value = "foo";
	Token returnType;
	returnType.type = TokenType::i32Keyword;
	ASTPrimitiveType* retType = new ASTPrimitiveType(TokenType::i32Keyword);

	ASTParamList* noParams = new ASTParamList();
	ASTBlock* fooBody = new ASTBlock();
	fooBody->statements.emplace_back(new ASTVarDecl(new ASTPrimitiveType(TokenType::i32Keyword), x, new ASTIntLiteral(10)));
	fooBody->statements.emplace_back(new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), timesEq, new ASTIntLiteral(10))));
	fooBody->statements.emplace_back(new ASTReturn(new ASTIdentifier(x)));

	Token bar;
	bar.value = "bar";
	ASTParamList* barParams = new ASTParamList();
	barParams->params.emplace_back(new ASTParameter(x, retType));
	barParams->params.emplace_back(new ASTParameter(y, retType));
	ASTBlock* barBody = new ASTBlock();
	barBody->statements.emplace_back(new ASTIfStatement(new ASTBinaryExpr(new ASTIdentifier(x), gt, new ASTIdentifier(y)), new ASTReturn(new ASTIdentifier(x)), new ASTReturn(new ASTIdentifier(y))));
	
	ASTArgList* noArgs = new ASTArgList();
	ASTArgList* args1 = new ASTArgList();
	ASTArgList* args2 = new ASTArgList();
	ASTArgList* args3 = new ASTArgList();
	ASTArgList* args4 = new ASTArgList();

	args1->args.emplace_back(new ASTArgument(new ASTIdentifier(x)));
	args1->args.emplace_back(new ASTArgument(new ASTIdentifier(y)));

	args2->args.emplace_back(new ASTArgument(new ASTIntLiteral(10)));
	args2->args.emplace_back(new ASTArgument(new ASTBinaryExpr(new ASTIntLiteral(20), add, new ASTIntLiteral(10))));

	args3->args.emplace_back(new ASTArgument(new ASTUnaryExpr(bitNot, new ASTIdentifier(x))));
	args3->args.emplace_back(new ASTArgument(new ASTUnaryExpr(sub, new ASTGroupExpr(new ASTBinaryExpr(new ASTIntLiteral(10), mul, new ASTIntLiteral(2))))));

	args4->args.emplace_back(new ASTArgument(new ASTCall(new ASTIdentifier(foo), noArgs)));
	args4->args.emplace_back(new ASTArgument(new ASTIntLiteral(1)));

	testSuite[TEST_PATH"/parser_tests/functions_test.hollow"].testname = "functions_test.hollow";
	testSuite[TEST_PATH"/parser_tests/functions_test.hollow"].expectedAst =
	{
		new ASTFuncDecl(foo, returnType, noParams, fooBody),
		new ASTFuncDecl(bar, returnType, barParams, barBody),
		new ASTExprStmt(new ASTCall(new ASTIdentifier(foo), noArgs)),
		new ASTExprStmt(new ASTCall(new ASTIdentifier(bar), args1)),
		new ASTExprStmt(new ASTCall(new ASTIdentifier(bar), args2)),
		new ASTExprStmt(new ASTCall(new ASTIdentifier(bar), args3)),
		new ASTExprStmt(new ASTCall(new ASTIdentifier(bar), args4)),
		new ASTExprStmt(new ASTAssign(new ASTIdentifier(x), assign, new ASTBinaryExpr(new ASTCall(new ASTIdentifier(bar), noArgs), mul, new ASTIntLiteral(2))))
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

	std::cout << std::endl;

	logFile.close();
}

void ParserTests::runSpecific(const std::string & filename)
{
	lexer.reset();
	lexer.lexFile(filename);
	parser.parse(lexer.getTokens(), &arena, &reporter);

	const std::vector<ASTNode*> ast = parser.getAst();
	const std::vector<ASTNode*>& expectedAst = testSuite[filename].expectedAst;
	size_t actualNodeCount = ast.size();
	bool passed = true;

	for (size_t i = 0; i < actualNodeCount; i++)
	{
		if (!(*ast[i] == *expectedAst[i]))
		{
			ASTPrinter visitor;
			passed = false;

			std::cout << "Got: ";
			ast[i]->accept(visitor, 0);

			std::cout << "Expected: ";
			expectedAst[i]->accept(visitor, 0);
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