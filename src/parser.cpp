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

void Parser::parse(std::vector<Token>& tokens)
{
	inFuncDecl = false;
	tokenStream.init(tokens);

	while (tokenStream.tokensInStream() && !tokenStream.peek(TokenType::Eof))
	{
		ASTNode* node = declaration();

		// declaration returns a nullptr if an error is found when parsing, so we need to check to make sure a valid AST node was returned
		// before pushing it into the AST
		if (node)
			ast.emplace_back(node);
	}
}

ASTNode* Parser::declaration()
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

ASTNode* Parser::varDecl()
{
	Token identifier = tokenStream.consume();	// get the identifier
	tokenStream.consume();						// consume the ":" token
	Token varType = tokenStream.consume();		// get the type of the variable
	
	ASTNode* varInitialization = nullptr;
	if (tokenStream.peek(TokenType::Assign))
	{
		tokenStream.consume();	// consume the "=" token
		varInitialization = expression();
		if (!varInitialization)
			throwError("Expect valid initialization for variable");
	}

	assertCurrent(TokenType::Semicolon, "Expect ';' after variable declaration");
	return new ASTVarDecl(varType.type, identifier, varInitialization);
}

ASTNode* Parser::funcDecl()
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

	std::vector<Token> params;
	if (!tokenStream.peek(TokenType::CloseParen))
	{
		assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
		Token paramIdentifier = tokenStream.prev();
		
		// this will also need to check for user defined types when that is added
		assertCurrentIsType("Expect type for function arguments");
		params.emplace_back(paramIdentifier);

		// same logic as above, just need to consume an additional comma token between parameters
		while (tokenStream.peek(TokenType::Comma))
		{
			tokenStream.consume();
			assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
			Token paramIdentifier = tokenStream.prev();
			assertCurrentIsType("Expect type for function arguments");
			params.emplace_back(paramIdentifier);
		}
	}

	assertCurrent(TokenType::CloseParen, "Expect \")\" after function parameters");
	assertCurrent(TokenType::Colon, "Expect \":\" to indicate function return type");
	assertCurrentIsType("Expect return type in function declaration");

	ASTBlock* body = block();

	inFuncDecl = false;
	return new ASTFuncDecl(functionIdentifier, params, body);
}

ASTNode* Parser::statement()
{
	// TODO: add forStmt, ifStmt ... , block
	if (tokenStream.peek(TokenType::ifKeyword))		return ifStatement();
	if (tokenStream.peek(TokenType::forKeyword))    return forStatement();
	if (tokenStream.peek(TokenType::returnKeyword)) return returnStatement();


	return expressionStatement();
}

ASTNode* Parser::forStatement()
{
	// consume the "for" keyword
	tokenStream.consume();
	assertCurrent(TokenType::OpenParen, "Expect \"(\" at start of for loop");

	// the initializer is null by default when just a ";" is provided
	// also need to do other checks for declarations or a simple expression -> [i : u8] or [i = 10] respectively
	ASTNode* initializer = nullptr;
	if (tokenStream.peek(TokenType::Semicolon))
		tokenStream.consume();
	else if (tokenStream.peek(TokenType::Identifier) && tokenStream.lookAhead(TokenType::Colon))
		initializer = varDecl();
	else
		initializer = expressionStatement();

	ASTNode* condition = nullptr;
	if (!tokenStream.peek(TokenType::Semicolon))
		condition = expression();

	assertCurrent(TokenType::Semicolon, "Expect \";\" after for loop condition");

	ASTNode* increment = nullptr;
	if (!tokenStream.peek(TokenType::CloseParen))
		increment = expression();

	assertCurrent(TokenType::CloseParen, "Expect \")\" at end of for loop");
	ASTBlock* body = block();

	return new ASTForLoop(initializer, condition, increment, body);
}

ASTNode* Parser::ifStatement()
{
	// consume the "if" keyword
	tokenStream.consume();

	// make sure the condition is contained within parenthesis, then get the expression for the condition and the statement for the true branch
	// we also set the false branch to null by default since it isn't required to have one in an if statement
	assertCurrent(TokenType::OpenParen, "Expect \"(\" for if statement condition");
	ASTNode* condition = expression();
	assertCurrent(TokenType::CloseParen, "Expect \")\" for if statement condition");
	ASTNode* trueBranch = statement();
	ASTNode* falseBranch = nullptr;

	// check to see if the if statement has an else clause
	if (tokenStream.peek(TokenType::elseKeyword))
	{
		// consume the "else" keyword then get the statement for the false branch
		tokenStream.consume();
		falseBranch = statement();
	}

	return new ASTIfStatement(condition, trueBranch, falseBranch);
}

ASTBlock* Parser::block()
{
	assertCurrent(TokenType::OpenCurly, "Expect \"{\" at start of block");

	ASTBlock* block = new ASTBlock();
	while (!tokenStream.peek(TokenType::CloseCurly))
	{
		ASTNode* statement = declaration();

		if (statement)
			block->statements.emplace_back(statement);
	}

	assertCurrent(TokenType::CloseCurly, "Expect \"}\" at end of block");
	return block;
}

ASTNode* Parser::returnStatement()
{
	// consume the "return" keyword
	tokenStream.consume();

	// initially have the return value be null, this is because some functions may return void which returns no value
	// only get the return value if there's not a semicolon directly after the return keyword
	ASTNode* returnVal = nullptr;
	if (!tokenStream.peek(TokenType::Semicolon))
		returnVal = expression();

	assertCurrent(TokenType::Semicolon, "Expect \";\" after return value");
	return new ASTReturn(returnVal);
}

ASTNode* Parser::expressionStatement()
{
	ASTNode* expr = expression();
	assertCurrent(TokenType::Semicolon, "Expect \";\" at end of expression");
	return new ASTExprStmt(expr);
}

ASTNode* Parser::expression()
{
	return assignment();
}

ASTNode* Parser::assignment()
{
	// this will need to be changed once more productions are added. instead of calling primary, call the production at the top of the chain first
	ASTNode* expression = primary();

	if (tokenStream.peek(TokenType::Assign))
	{
		Token equal = tokenStream.consume();
		ASTNode* value = assignment();
		ASTVarAccess* variable = dynamic_cast<ASTVarAccess*>(expression);

		// if the dynamic cast succeeds, the LHS expanded to a variable and we know we're doing an assignment [x = (expression);]
		if (variable)
			return new ASTAssign(variable->identifier, value);

		throwError("LHS of assignment needs to be a variable");
	}

	return expression;
}

ASTNode* Parser::logicalOr()
{
	ASTNode* lhs = logicalAnd();

	while (tokenStream.peek(TokenType::LogicalOr))
	{
		Token logicalOperator = tokenStream.consume();
		ASTNode* rhs = logicalAnd();
		lhs = new ASTLogical(lhs, logicalOperator, rhs);
	}

	return lhs;
}

ASTNode* Parser::logicalAnd()
{
	ASTNode* lhs = equality();

	while (tokenStream.peek(TokenType::LogicalAnd))
	{
		Token logicalOperator = tokenStream.consume();
		ASTNode* rhs = equality();
		lhs = new ASTLogical(lhs, logicalOperator, rhs);
	}

	return lhs;
}

ASTNode* Parser::equality()
{
	ASTNode* lhs = comparison();

	while (tokenStream.matchToken({ TokenType::NotEq, TokenType::Equality }))
	{
		Token op = tokenStream.consume();
		ASTNode* rhs = comparison();
		lhs = new ASTBinaryExpr(lhs, op, rhs);
	}

	return lhs;
}

ASTNode* Parser::comparison()
{
	ASTNode* lhs = term();
	
	while (tokenStream.matchToken({ TokenType::GreaterThan, TokenType::GreaterThanEq, TokenType::LessThan, TokenType::LessThanEq }))
	{
		Token op = tokenStream.consume();
		ASTNode* rhs = term();
		lhs = new ASTBinaryExpr(lhs, op, rhs);
	}

	return lhs;
}

ASTNode* Parser::term()
{
	ASTNode* lhs = factor();

	while (tokenStream.matchToken({ TokenType::Minus, TokenType::Plus }))
	{
		Token op = tokenStream.consume();
		ASTNode* rhs = factor();
		lhs = new ASTBinaryExpr(lhs, op, rhs);
	}

	return lhs;
}

ASTNode* Parser::factor()
{
	ASTNode* lhs = unary();

	while (tokenStream.matchToken({ TokenType::ForwardSlash, TokenType::Asterisk }))
	{
		Token op = tokenStream.consume();
		ASTNode* rhs = unary();
		lhs = new ASTBinaryExpr(lhs, op, rhs);
	}

	return lhs;
}

ASTNode* Parser::unary()
{
	// if the top token is "-" or "!", consume it and recursively call unary again. This allows expressions like !!(expression), -(expression)
	// and even !foo() or -foo() via the call() function at the bottom. call() will either result in a function call, or a simple primary() call
	if (tokenStream.matchToken({ TokenType::Minus, TokenType::LogicalNot }))
	{
		Token op = tokenStream.consume();
		ASTNode* expr = unary();
		return new ASTUnaryExpr(op, expr);
	}

	return call();
}

ASTNode* Parser::call()
{
	// parse the primary production rule
	ASTNode* expr = primary();

	// this call() production rule can produce a function call if we find an open paren directly after our primary rule
	// otherwise, if no open paren is found, the primary we produced is returned since the while loop will do nothing
	while (true)
	{
		if (tokenStream.peek(TokenType::OpenParen))
			expr = parseFuncArgs(expr);
		else
			break;
	}

	return expr;
}

ASTNode* Parser::parseFuncArgs(ASTNode* callee)
{
	// consume the "(" token
	tokenStream.consume();
	std::vector<ASTNode*> args;

	// if the next token after "(" is ")", then the function is being called with no arguments
	if (tokenStream.peek(TokenType::CloseParen))
		return new ASTCall(callee, args);

	// otherwise, parse all arguments
	args.emplace_back(expression());
	while (tokenStream.peek(TokenType::Comma))
	{
		tokenStream.consume();
		args.emplace_back(expression());
	}

	assertCurrent(TokenType::CloseParen, "Expect \")\" after function arguments");
	return new ASTCall(callee, args);
}

ASTNode* Parser::primary()
{
	std::cout << "in primary\n";
	if (tokenStream.peek(TokenType::IntLiteral)) return new ASTIntLiteral(std::get<uint64_t>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::FloatLiteral)) return new ASTDoubleLiteral(std::get<double>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::CharLiteral)) return new ASTCharLiteral(std::get<char>(tokenStream.consume().value));
	if (tokenStream.peek(TokenType::Identifier)) return new ASTVarAccess(tokenStream.consume());

	if (tokenStream.peek(TokenType::trueKeyword))
	{
		tokenStream.consume();
		return new ASTBoolLiteral(true);
	}

	if (tokenStream.peek(TokenType::falseKeyword))
	{
		tokenStream.consume();
		return new ASTBoolLiteral(false);
	}

	if (tokenStream.peek(TokenType::OpenParen))
	{
		tokenStream.consume();
		ASTNode* expr = expression();
		assertCurrent(TokenType::CloseParen, "Expect \")\" to close expression");
		return new ASTGroupExpr(expr);
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

	// sometimes there will be a closing curly brace if the line that had an error was right before a block ended, so just check for that
	if (tokenStream.peek(TokenType::CloseCurly))
		tokenStream.consume();
}