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
	assertCurrent(TokenType::OpenCurly, "Expect \"{\" before function body");

	// GET RID OF THIS, JUST TEMP TO TEST THE ABOVE CODE
	assertCurrent(TokenType::CloseCurly, "Expect \"}\" before function body");

	std::vector<ASTNode*> tempBody;
	inFuncDecl = false;
	return new ASTFuncDecl(functionIdentifier, params, tempBody);
}

ASTNode* Parser::statement()
{
	// TODO: add forStmt, ifStmt ... , block
	return expressionStatement();
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