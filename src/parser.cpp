#include "parser.h"
#include "ast_printer.h"
#include "debug_utils.h"

#include <iostream>

Parser::Parser()
{
	this->arena = nullptr;
	this->diagnosticReporter = nullptr;
}

void Parser::printAST()
{
	ASTPrinter printer;

	for (ASTNode* node : ast)
		node->accept(printer, 0);
}

void Parser::reset()
{
	inFuncDecl = false;
	ast.clear();
}

void Parser::parse(std::vector<Token>& tokens, MemoryArena* nodeArena, DiagnosticReporter* diagnosticReporter)
{
	reset();
	tokenStream.init(tokens);
	
	this->arena = nodeArena;
	this->diagnosticReporter = diagnosticReporter;

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
	Token start = tokenStream.get();
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
	ASTVarDecl* node = arena->alloc<ASTVarDecl>(varType.type, identifier, varInitialization);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTStmt* Parser::funcDecl()
{
	if (inFuncDecl)
	{
		inFuncDecl = false;
		throwError("Nested functions are not allowed");
	}

	// indicate that we are currently in a function declaration and consume the "func" keyword
	Token start = tokenStream.get();
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
	ASTFuncDecl* node = arena->alloc<ASTFuncDecl>(functionIdentifier, returnType, params, body);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTStmt* Parser::statement()
{
	// TODO: add forStmt, ifStmt ... , block
	if (tokenStream.peek(TokenType::ifKeyword))		return ifStatement();
	if (tokenStream.peek(TokenType::forKeyword))    return forStatement();
	if (tokenStream.peek(TokenType::returnKeyword)) return returnStatement();
	if (tokenStream.peek(TokenType::whileKeyword))  return whileStatement();
	if (tokenStream.peek(TokenType::OpenCurly))     return block();


	return expressionStatement();
}

ASTStmt* Parser::forStatement()
{
	// consume the "for" keyword
	Token start = tokenStream.get();
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

	ASTForLoop* node = arena->alloc<ASTForLoop>(initializer, condition, increment, body);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTStmt* Parser::whileStatement()
{
	// consume the "while" keyword
	Token start = tokenStream.get();
	tokenStream.consume();

	// make sure there are opening / closing parentheses tokens, then parse the expression for the while loop condition
	assertCurrent(TokenType::OpenParen, "Expect \"(\" at start of while loop condition");
	ASTExpr* condition = expression();
	assertCurrent(TokenType::CloseParen, "Expect \")\" at end of while loop condition");

	ASTBlock* body = block();

	ASTWhileLoop* node = arena->alloc<ASTWhileLoop>(condition, body);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTStmt* Parser::ifStatement()
{
	// consume the "if" keyword
	Token start = tokenStream.get();
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

	ASTIfStatement* node = arena->alloc<ASTIfStatement>(condition, trueBranch, falseBranch);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTBlock* Parser::block()
{
	assertCurrent(TokenType::OpenCurly, "Expect \"{\" at start of block");
	Token start = tokenStream.prev();

	ASTBlock* block = arena->alloc<ASTBlock>();
	block->line = start.lineNum;
	block->col = start.column;
	while (tokenStream.tokensInStream() && !tokenStream.peek(TokenType::CloseCurly))
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
	Token start = tokenStream.get();
	tokenStream.consume();

	// initially have the return value be null, this is because some functions may return void which returns no value
	// only get the return value if there's not a semicolon directly after the return keyword
	ASTExpr* returnVal = nullptr;
	if (!tokenStream.peek(TokenType::Semicolon))
		returnVal = expression();

	assertCurrent(TokenType::Semicolon, "Expect \";\" after return value");
	
	ASTReturn* node = arena->alloc<ASTReturn>(returnVal);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTStmt* Parser::expressionStatement()
{
	Token start = tokenStream.get();
	ASTExpr* expr = expression();
	assertCurrent(TokenType::Semicolon, "Expect \";\" at end of expression");

	ASTExprStmt* node = arena->alloc<ASTExprStmt>(expr);
	node->line = start.lineNum;
	node->col = start.column;
	return node;
}

ASTExpr* Parser::expression()
{
	return assignment();
}

ASTExpr* Parser::assignment()
{
	Token start = tokenStream.get();
	ASTExpr* expression = logicalOr();

	if (tokenStream.topIsAssignment())
	{
		Token assignmentOperator = tokenStream.consume();
		ASTExpr* value = assignment();
		ASTIdentifier* assignee = dynamic_cast<ASTIdentifier*>(expression);
		
		// if the dynamic cast succeeds, the LHS expanded to a variable and we know we're doing an assignment [x = (expression);]
		if (assignee)
		{
			ASTAssign* node = arena->alloc<ASTAssign>(assignee, assignmentOperator, value);
			node->line = start.lineNum;
			node->col = start.column;
			return node;
		}

		throwError("LHS of assignment needs to be a variable");
	}

	return expression;
}

ASTExpr* Parser::logicalOr()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = logicalAnd();

	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::LogicalOr))
	{
		Token logicalOperator = tokenStream.consume();
		ASTExpr* rhs = logicalAnd();
		lhs = arena->alloc<ASTLogical>(lhs, logicalOperator, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::logicalAnd()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = equality();

	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::LogicalAnd))
	{
		Token logicalOperator = tokenStream.consume();
		ASTExpr* rhs = equality();
		lhs = arena->alloc<ASTLogical>(lhs, logicalOperator, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::equality()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = bitwiseOr();

	while (tokenStream.tokensInStream() && tokenStream.matchToken({ TokenType::NotEq, TokenType::Equality }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseOr();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::bitwiseOr()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = bitwiseXor();

	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::BitwiseOr))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseXor();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::bitwiseXor()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = bitwiseAnd();

	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::BitwiseXor))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseAnd();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::bitwiseAnd()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = comparison();

	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::BitwiseAnd))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = comparison();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::comparison()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = bitwiseShift();
	
	while (tokenStream.tokensInStream() && tokenStream.matchToken({ TokenType::GreaterThan, TokenType::GreaterThanEq, TokenType::LessThan, TokenType::LessThanEq }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = bitwiseShift();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::bitwiseShift()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = term();

	while (tokenStream.tokensInStream() && tokenStream.matchToken({ TokenType::BitwiseLeftShift, TokenType::BitwiseRightShift }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = term();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::term()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = factor();

	while (tokenStream.tokensInStream() && tokenStream.matchToken({ TokenType::Minus, TokenType::Plus }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = factor();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::factor()
{
	Token start = tokenStream.get();
	ASTExpr* lhs = unary();

	while (tokenStream.tokensInStream() && tokenStream.matchToken({ TokenType::ForwardSlash, TokenType::Asterisk }))
	{
		Token op = tokenStream.consume();
		ASTExpr* rhs = unary();
		lhs = arena->alloc<ASTBinaryExpr>(lhs, op, rhs);
		lhs->line = start.lineNum;
		lhs->col = start.column;
	}

	return lhs;
}

ASTExpr* Parser::unary()
{
	// if the top token is "-" or "!", consume it and recursively call unary again. This allows expressions like !!(expression), -(expression)
	// and even !foo() or -foo() via the postfix() function at the bottom. call() will either result in a function call, or a simple primary() call
	Token start = tokenStream.get();
	if (tokenStream.matchToken({ TokenType::Minus, TokenType::LogicalNot, TokenType::BitwiseNot, TokenType::Increment, TokenType::Decrement }))
	{
		Token op = tokenStream.consume();
		ASTExpr* expr = unary();

		ASTUnaryExpr* node = arena->alloc<ASTUnaryExpr>(op, expr);
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	return postfix();
}

ASTExpr* Parser::postfix()
{
	// parse the primary production rule
	Token start = tokenStream.get();
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
			expr = arena->alloc<ASTPostfix>(expr, op);
			expr->line = start.lineNum;
			expr->col = start.column;
		}
		else
			break;
	}
	
	//expr->line = start.lineNum;
	//expr->col = start.column;
	return expr;
}

ASTExpr* Parser::parseFuncArgs(ASTExpr* callee)
{
	// consume the "(" token
	Token start = tokenStream.get();
	tokenStream.consume();
	
	ASTArgList* args = arena->alloc<ASTArgList>();
	args->line = start.lineNum;
	args->col = start.column;

	// if the next token after "(" is ")", then the function is being called with no arguments
	if (tokenStream.peek(TokenType::CloseParen))
	{
		// consume the ")" token and return an ASTCall node with no args
		tokenStream.consume();
		ASTCall* call = arena->alloc<ASTCall>(callee, args);
		call->line = start.lineNum;
		call->col = start.column;
		return call;
	}

	// otherwise, parse all arguments
	Token argStart = tokenStream.get();
	ASTArgument* arg = arena->alloc<ASTArgument>(expression());
	arg->line = argStart.lineNum;
	arg->col = argStart.column;
	args->args.emplace_back(arg);
	while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::Comma))
	{
		tokenStream.consume();
		Token argStart = tokenStream.get();
		ASTArgument* arg = arena->alloc<ASTArgument>(expression());
		arg->line = argStart.lineNum;
		arg->col = argStart.column;
		args->args.emplace_back(arg);
	}

	assertCurrent(TokenType::CloseParen, "Expect \")\" after function arguments");
	ASTCall* call = arena->alloc<ASTCall>(callee, args);
	call->line = start.lineNum;
	call->col = start.column;
	return call;
	//return arena->alloc<ASTCall>(callee, args);
}

ASTParamList* Parser::parseFuncParams()
{
	Token start = tokenStream.get();
	ASTParamList* node = arena->alloc<ASTParamList>();
	node->line = start.lineNum;
	node->col = start.column;

	if (!tokenStream.peek(TokenType::CloseParen))
	{
		assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
		Token paramIdentifier = tokenStream.prev();

		// this will also need to check for user defined types when that is added
		assertCurrent(TokenType::Colon, "Parameters need to be in the form of identifier : type");
		assertCurrentIsType("Expect type for function arguments");
		Token paramType = tokenStream.prev();

		node->params.emplace_back(arena->alloc<ASTParameter>(paramIdentifier, paramType));

		// same logic as above, just need to consume an additional comma token between parameters
		while (tokenStream.tokensInStream() && tokenStream.peek(TokenType::Comma))
		{
			tokenStream.consume();
			assertCurrent(TokenType::Identifier, "Expect parameter name in function declaration");
			Token paramIdentifier = tokenStream.prev();

			assertCurrent(TokenType::Colon, "Function parameters need to be in the form of identifier : type");
			assertCurrentIsType("Expect type for function arguments");
			Token paramType = tokenStream.prev();

			node->params.emplace_back(arena->alloc<ASTParameter>(paramIdentifier, paramType));
		}
	}

	return node;
}

ASTExpr* Parser::primary()
{
	if (tokenStream.peek(TokenType::IntLiteral))
	{
		Token start = tokenStream.get();
		ASTIntLiteral* node = arena->alloc<ASTIntLiteral>(std::get<uint64_t>(tokenStream.consume().value));
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::FloatLiteral))
	{
		Token start = tokenStream.get();
		ASTDoubleLiteral* node = arena->alloc<ASTDoubleLiteral>(std::get<double>(tokenStream.consume().value));
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::CharLiteral))
	{
		Token start = tokenStream.get();
		ASTCharLiteral* node = arena->alloc<ASTCharLiteral>(std::get<char>(tokenStream.consume().value));
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::Identifier))
	{
		Token start = tokenStream.get();
		ASTIdentifier* node = arena->alloc<ASTIdentifier>(tokenStream.consume());
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::trueKeyword))
	{
		Token start = tokenStream.get();
		tokenStream.consume();
		ASTBoolLiteral* node = arena->alloc<ASTBoolLiteral>(true);
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::falseKeyword))
	{
		Token start = tokenStream.get();
		tokenStream.consume();
		ASTBoolLiteral* node = arena->alloc<ASTBoolLiteral>(false);
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	if (tokenStream.peek(TokenType::OpenParen))
	{
		Token start = tokenStream.get();
		tokenStream.consume();
		ASTExpr* expr = expression();
		assertCurrent(TokenType::CloseParen, "Expect \")\" to close expression");
		ASTGroupExpr* node = arena->alloc<ASTGroupExpr>(expr);
		node->line = start.lineNum;
		node->col = start.column;
		return node;
	}

	tokenStream.consume();
	throwError("expected expression");
	return nullptr;
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
	diagnosticReporter->reportDiagnostic(errorMsg, DiagnosticLevel::Error, DiagnosticType::ParseError, ErrorPhase::Parser, errorToken.lineNum, errorToken.column);
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
	//if (tokenStream.peek(TokenType::CloseCurly))
	//	tokenStream.consume();
}