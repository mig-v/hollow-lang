#include "parser.h"
#include "ast_printer.h"

#include <iostream>

void Parser::printAST()
{
	ASTPrinter printer;

	for (ASTNode* node : ast)
		node->accept(printer);
}

void Parser::printErrors()
{
	for (const ErrorReport& error : parseErrors)
		std::cout << "line " << error.errorToken.lineNum << " " << error.errorMessage << std::endl;
}

void Parser::reset()
{
	inFuncDecl = false;
	ast.clear();
}

void Parser::parse(std::vector<Token>& tokens)
{
	reset();
	tokenStream.init(tokens);

	while (tokenStream.tokensInStream() && !tokenStream.peek(TokenType::Eof))
	{
		ASTStmt* node = declaration();

		// declaration returns a nullptr if an error is found when parsing, so we need to check to make sure a valid AST node was returned
		// before pushing it into the AST
		if (node)
			ast.emplace_back(node);
	}
}

ASTStmt* Parser::declaration()
{
	try
	{
		// check to see if a function declaration is being made
		if (tokenStream.peek(TokenType::funcKeyword))
			return funcDecl();

		// If we see something like [x :] then we know we have a variable declaration
		if (tokenStream.peek(TokenType::Identifier) && tokenStream.lookAhead(TokenType::Colon))
			return varDecl();

		// if no other declarations are being made, we can return statement() which will recursively apply productions until one is found
		// or an error is found
		return statement();
	}
	catch (...)
	{
		synchronize();
		return nullptr;
	}
}

ASTStmt* Parser::varDecl()
{
	Token identifier = tokenStream.consume();	// get the identifier
	tokenStream.consume();						// consume the ":" token
	Token varType = tokenStream.consume();		// get the type of the variable
	
	ASTExpr* varInitialization = nullptr;
	if (tokenStream.peek(TokenType::Assign))
	{
		tokenStream.consume();	// consume the "=" token
		varInitialization = expression();
		if (!varInitialization)
			throwError("Expect valid initialization for variable");
	}

	assertCurrent(TokenType::Semicolon, "Expect ';' after variable declaration");
	return arena.alloc<ASTVarDecl>(varType.type, identifier, varInitialization);
}

ASTStmt* Parser::funcDecl()
{
	if (inFuncDecl)
	{
		inFuncDecl = false;
		throwError("Nested functions are not allowed");
	}

	// indicate that we are currently in a function declaration and consume the "func" keyword
	inFuncDecl = true;
	tokenStream.consume();

	// make sure a name for the function is provided, then get it by getting the previously consumed token from the token stream
	assertCurrent(TokenType::Identifier, "Expect function name in declaration");
	Token functionIdentifier = tokenStream.prev();
	assertCurrent(TokenType::OpenParen, "Expect \"(\" after function name");
	ASTParamList* params = parseFuncParams();

	assertCurrent(TokenType::CloseParen, "Expect \")\" after function parameters");
	assertCurrent(TokenType::Colon, "Expect \":\" to indicate function return type");
	assertCurrentIsType("Expect return type in function declaration");
	Token returnType = tokenStream.prev();

	ASTBlock* body = block();

	inFuncDecl = false;
	return arena.alloc<ASTFuncDecl>(functionIdentifier, returnType, params, body);
}

ASTStmt* Parser::statement()
{
	// TODO: add forStmt, ifStmt ... , block
	if (tokenStream.peek(TokenType::ifKeyword))		return ifStatement();
	if (tokenStream.peek(TokenType::forKeyword))    return forStatement();
	if (tokenStream.peek(TokenType::returnKeyword)) return returnStatement();
	if (tokenStream.peek(TokenType::OpenCurly))     return block();


	return expressionStatement();
}

ASTStmt* Parser::forStatement()
{
	// consume the "for" keyword
	tokenStream.consume();
	assertCurrent(TokenType::OpenParen, "Expect \"(\" at start of for loop");

	// the initializer is null by default when just a ";" is provided
	// also need to do other checks for declarations or a simple expression -> [i : u8] or [i = 10] respectively
	ASTStmt* initializer = nullptr;
	if (tokenStream.peek(TokenType::Semicolon))
		tokenStream.consume();
	else if (tokenStream.peek(TokenType::Identifier) && tokenStream.lookAhead(TokenType::Colon))
		initializer = varDecl();
	else
		initializer = expressionStatement();

	ASTExpr* condition = nullptr;
	if (!tokenStream.peek(TokenType::Semicolon))
		condition = expression();

	assertCurrent(TokenType::Semicolon, "Expect \";\" after for loop condition");

	ASTExpr* increment = nullptr;
	if (!tokenStream.peek(TokenType::CloseParen))
		increment = expression();

	assertCurrent(TokenType::CloseParen, "Expect \")\" at end of for loop");
	ASTBlock* body = block();

	return arena.alloc<ASTForLoop>(initializer, condition, increment, body);
}

ASTStmt* Parser::ifStatement()
{
	// consume the "if" keyword
	tokenStream.consume();

	// make sure the condition is contained within parenthesis, then get the expression for the condition and the statement for the true branch
	// we also set the false branch to null by default since it isn't required to have one in an if statement
	assertCurrent(TokenType::OpenParen, "Expect \"(\" for if statement condition");
	ASTExpr* condition = expression();
	assertCurrent(TokenType::CloseParen, "Expect \")\" for if statement condition");
	ASTStmt* trueBranch = statement();
	ASTStmt* falseBranch = nullptr;

	// check to see if the if statement has an else clause
	if (tokenStream.peek(TokenType::elseKeyword))
	{
		// consume the "else" keyword then get the statement for the false branch
		tokenStream.consume();
		falseBranch = statement();
	}

	return arena.alloc<ASTIfStatement>(condition, trueBranch, falseBranch);
}

ASTBlock* Parser::block()
{
	assertCurrent(TokenType::OpenCurly, "Expect \"{\" at start of block");

	ASTBlock* block = arena.alloc<ASTBlock>();
	while (!tokenStream.peek(TokenType::CloseCurly))
	{
		ASTStmt* statement = declaration();

		if (statement)
			block->statements.emplace_back(statement);
	}

	assertCurrent(TokenType::CloseCurly, "Expect \"}\" at end of block");
	return block;
}

ASTStmt* Parser::returnStatement()
{
	// consume the "return" keyword
	tokenStream.consume();

	// initially have the return value be null, this is because some functions may return void which returns no value
	// only get the return value if there's not a semicolon directly after the return keyword
	ASTExpr* returnVal = nullptr;
	if (!tokenStream.peek(TokenType::Semicolon))
		returnVal = expression();

	assertCurrent(TokenType::Semicolon, "Expect \";\" after return value");
	return arena.alloc<ASTReturn>(returnVal);
}

ASTStmt* Parser::expressionStatement()
{
	ASTExpr* expr = expression();
	std::cout << "About to assert on semicolon in expressionStatement(), top = " << (int)tokenStream.get().type << std::endl;
	assertCurrent(TokenType::Semicolon, "Expect \";\" at end of expression");
	return arena.alloc<ASTExprStmt>(expr);
}

ASTExpr* Parser::expression()
{
	return assignment();
}

ASTExpr* Parser::assignment()
{
	ASTExpr* expression = logicalOr();

	if (tokenStream.topIsAssignment())
	{
		Token assignmentOperator = tokenStream.consume();
		ASTExpr* value = assignment();
		ASTVariable* variable = dynamic_cast<ASTVariable*>(expression);

		// if the dynamic cast succeeds, the LHS expanded to a variable and we know we're doing an assignment [x = (expression);]
		if (variable)
			return arena.alloc<ASTAssign>(variable->identifier, assignmentOperator, value);

		throwError("LHS of assignment needs to be a variable");
	}

	return expression;
}

ASTExpr* Parser::logicalOr()
{
	ASTExpr* lhs = logicalAnd();

	while (tokenStream.peek(TokenType::LogicalOr))
	{
		Token logicalOperator = tokenStream.consume();
		ASTExpr* rhs = logicalAnd();
		lhs = arena.alloc<ASTLogical>(lhs, logicalOperator, rhs);
	}

	return lhs;
}

ASTExpr* Parser::logicalAnd()
{
	ASTExpr* lhs = bitwiseOr();

	while (tokenStream.peek(TokenType::LogicalAnd))
	{
		Token logicalOperator = tokenStream.consume();
		ASTExpr* rhs = bitwiseOr();
		lhs = arena.alloc<ASTLogical>(lhs, logicalOperator, rhs);
	}

	return lhs;
}

ASTExpr* Parser::bitwiseOr()
{
	ASTExpr* lhs = bitwiseXor();

	while (tokenStream.peek(TokenType::BitwiseOr))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseXor();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::bitwiseXor()
{
	ASTExpr* lhs = bitwiseAnd();

	while (tokenStream.peek(TokenType::BitwiseXor))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseAnd();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::bitwiseAnd()
{
	ASTExpr* lhs = equality();

	while (tokenStream.peek(TokenType::BitwiseAnd))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = equality();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::equality()
{
	ASTExpr* lhs = comparison();

	while (tokenStream.matchToken({ TokenType::NotEq, TokenType::Equality }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = comparison();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::comparison()
{
	ASTExpr* lhs = bitwiseShift();
	
	while (tokenStream.matchToken({ TokenType::GreaterThan, TokenType::GreaterThanEq, TokenType::LessThan, TokenType::LessThanEq }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseShift();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::bitwiseShift()
{
	ASTExpr* lhs = term();

	while (tokenStream.matchToken({ TokenType::BitwiseLeftShift, TokenType::BitwiseRightShift }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = term();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::term()
{
	ASTExpr* lhs = factor();

	while (tokenStream.matchToken({ TokenType::Minus, TokenType::Plus }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = factor();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::factor()
{
	ASTExpr* lhs = unary();

	while (tokenStream.matchToken({ TokenType::ForwardSlash, TokenType::Asterisk }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = unary();
		lhs = arena.alloc<ASTBinaryExpr>(lhs, op, rhs);
	}

	return lhs;
}

ASTExpr* Parser::unary()
{
	// if the top token is "-" or "!", consume it and recursively call unary again. This allows expressions like !!(expression), -(expression)
	// and even !foo() or -foo() via the call() function at the bottom. call() will either result in a function call, or a simple primary() call
	if (tokenStream.matchToken({ TokenType::Minus, TokenType::LogicalNot, TokenType::BitwiseNot, TokenType::Increment, TokenType::Decrement }))
	{
		Token op = tokenStream.consume();
		ASTExpr* expr = unary();
		return arena.alloc<ASTUnaryExpr>(op, expr);
	}

	return postfix();
}

ASTExpr* Parser::postfix()
{
	// parse the primary production rule
	ASTExpr* expr = primary();

	// this postfix() production rule can produce a function call if we find an open paren directly after our primary rule
	while (true)
	{
		if (tokenStream.peek(TokenType::OpenParen))
		{
			expr = parseFuncArgs(expr);
		}
		else if (tokenStream.matchToken({ TokenType::Increment, TokenType::Decrement }))
		{
			Token op = tokenStream.consume();
			expr = arena.alloc<ASTPostfix>(expr, op);
		}
		else
			break;
	}
	
	return expr;
}

ASTExpr* Parser::parseFuncArgs(ASTNode* callee)
{
	// consume the "(" token
	tokenStream.consume();
	ASTArgList* args = arena.alloc<ASTArgList>();

	// if the next token after "(" is ")", then the function is being called with no arguments
	if (tokenStream.peek(TokenType::CloseParen))
		return arena.alloc<ASTCall>(callee, args);

	// otherwise, parse all arguments
	args->args.emplace_back(arena.alloc<ASTArgument>(expression()));
	while (tokenStream.peek(TokenType::Comma))
	{
		tokenStream.consume();
		args->args.emplace_back(arena.alloc<ASTArgument>(expression()));
	}

	assertCurrent(TokenType::CloseParen, "Expect \")\" after function arguments");
	return arena.alloc<ASTCall>(callee, args);
}

ASTParamList* Parser::parseFuncParams()
{
	ASTParamList* node = arena.alloc<ASTParamList>();
	if (!tokenStream.peek(TokenType::CloseParen))
	{
		assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
		Token paramIdentifier = tokenStream.prev();

		// this will also need to check for user defined types when that is added
		assertCurrent(TokenType::Colon, "Parameters need to be in the form of identifier : type");
		assertCurrentIsType("Expect type for function arguments");
		Token paramType = tokenStream.prev();

		node->params.emplace_back(arena.alloc<ASTParameter>(paramIdentifier, paramType));

		// same logic as above, just need to consume an additional comma token between parameters
		while (tokenStream.peek(TokenType::Comma))
		{
			tokenStream.consume();
			assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
			Token paramIdentifier = tokenStream.prev();

			assertCurrent(TokenType::Colon, "Function parameters need to be in the form of identifier : type");
			assertCurrentIsType("Expect type for function arguments");
			Token paramType = tokenStream.prev();

			node->params.emplace_back(arena.alloc<ASTParameter>(paramIdentifier, paramType));
		}
	}

	return node;
}

ASTExpr* Parser::primary()
{
	
	if (tokenStream.peek(TokenType::IntLiteral))   return arena.alloc<ASTIntLiteral>(std::get<uint64_t>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::FloatLiteral)) return arena.alloc<ASTDoubleLiteral>(std::get<double>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::CharLiteral))  return arena.alloc<ASTCharLiteral>(std::get<char>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::Identifier))   return arena.alloc<ASTVariable>(tokenStream.consume());

	if (tokenStream.peek(TokenType::trueKeyword))
	{
		tokenStream.consume();
		return arena.alloc<ASTBoolLiteral>(true);
	}

	if (tokenStream.peek(TokenType::falseKeyword))
	{
		tokenStream.consume();
		return arena.alloc<ASTBoolLiteral>(false);
	}

	if (tokenStream.peek(TokenType::OpenParen))
	{
		tokenStream.consume();
		ASTExpr* expr = expression();
		assertCurrent(TokenType::CloseParen, "Expect \")\" to close expression");
		return arena.alloc<ASTGroupExpr>(expr);
	}
}

void Parser::assertCurrent(TokenType type, const std::string& errorMsg)
{
	if (tokenStream.peek(type))
		tokenStream.consume();
	else
		throwError(errorMsg);
}

void Parser::assertCurrentIsType(const std::string& errorMsg)
{
	if (tokenStream.topIsPrimitive())
		tokenStream.consume();
	else
		throwError(errorMsg);
}

void Parser::throwError(const std::string& errorMsg)
{
	// if there are still tokens in the stream, then get the top token as the error token, if not then get the previously consumed token
	// as the next best thing to represent the error token
	Token errorToken = (tokenStream.tokensInStream()) ? tokenStream.get() : tokenStream.prev();
	ErrorReport error;
	error.errorType = ErrorType::SyntaxError;
	error.errorToken = errorToken;
	error.errorMessage = errorMsg;
	parseErrors.emplace_back(error);
	throw std::exception();
}

void Parser::synchronize()
{
	// since the compiler is in an error state, we need to consume all tokens till we find a semicolon then continue parsing
	while (tokenStream.tokensInStream() && !tokenStream.peek(TokenType::Semicolon))
	{
		tokenStream.consume();
	}

	// consume the ";" token
	if (tokenStream.tokensInStream())
		tokenStream.consume();

	// sometimes there will be a closing curly brace if the line that had an error was right before a block ending, so just check for that
	if (tokenStream.peek(TokenType::CloseCurly))
		tokenStream.consume();
}