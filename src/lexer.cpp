#include <fstream>
#include <iostream>

#include "lexer.h"

Lexer::Lexer()
{
	reset();

	keywords["i8"]  = TokenType::i8Keyword;
	keywords["i16"] = TokenType::i16Keyword;
	keywords["i32"] = TokenType::i32Keyword;
	keywords["i64"] = TokenType::i64Keyword;
	keywords["u8"]  = TokenType::u8Keyword;
	keywords["u16"] = TokenType::u16Keyword;
	keywords["u32"] = TokenType::u32Keyword;
	keywords["u64"] = TokenType::u64Keyword;
			
	keywords["f32"] = TokenType::f32Keyword;
	keywords["f64"] = TokenType::f64Keyword;
			
	keywords["bool"]  = TokenType::boolKeyword;
	keywords["true"]  = TokenType::trueKeyword;
	keywords["false"] = TokenType::falseKeyword;
	keywords["char"]  = TokenType::charKeyword;
			
	keywords["type"]   = TokenType::typeKeyword;
	keywords["func"]   = TokenType::funcKeyword;
	keywords["return"] = TokenType::returnKeyword;
	keywords["break"]  = TokenType::breakKeyword;
	keywords["if"]     = TokenType::ifKeyword;
	keywords["for"]    = TokenType::forKeyword;
	keywords["while"]  = TokenType::whileKeyword;

	validEscapeChars["\'\\n'"] = '\n';
	validEscapeChars["\'\\t'"] = '\t';
	validEscapeChars["\'\\b'"] = '\b';
	validEscapeChars["\'\\r'"] = '\r';
	validEscapeChars["\'\\a'"] = '\a';
	validEscapeChars["\'\\\"'"] = '\"';
	validEscapeChars["\'\\\\'"] = '\\';
	validEscapeChars["\'\\\'\'"] = '\'';
	validEscapeChars["\'\\0'"] = '\0';
	validEscapeChars["\'\\v'"] = '\v';
	validEscapeChars["\'\\f'"] = '\f';
}

void Lexer::reset()
{
	state = LexerState::StartState;
	currentToken.type = TokenType::None;
	tokenIndex = 0;
	lineNum = 1;

	currentLiteral.clear();
	tokens.clear();
	fileContents.clear();
	fileContents.str("");
}

Token* Lexer::getToken()
{
	return (tokenIndex < tokens.size()) ? &tokens[tokenIndex++] : nullptr;
}

Token* Lexer::peek()
{
	return (tokenIndex < tokens.size()) ? &tokens[tokenIndex] : nullptr;
}

void Lexer::dumpTokens()
{
	for (size_t i = 0; i < tokens.size(); i++)
		printToken(tokens[i]);
}

void Lexer::printToken(const Token& token)
{
	switch (token.type)
	{
		case TokenType::i8Keyword:		std::cout << "<i8Keyword  : i8> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::i16Keyword:		std::cout << "<i16Keyword : i16> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::i32Keyword:		std::cout << "<i32Keyword : i32> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::i64Keyword:		std::cout << "<i64Keyword : i64> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::u8Keyword:		std::cout << "<u8Keyword  : u8> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::u16Keyword:		std::cout << "<u16Keyword : u16> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::u32Keyword:		std::cout << "<u32Keyword : u32> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::u64Keyword:		std::cout << "<u64Keyword : u64> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::f32Keyword:		std::cout << "<f32Keyword : f32> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::f64Keyword:		std::cout << "<f64Keyword : f64> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::charKeyword:	std::cout << "<charKeyword : char> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::boolKeyword:	std::cout << "<boolKeyword : bool> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::falseKeyword:	std::cout << "<falseKeyword : false> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::trueKeyword:	std::cout << "<trueKeyword : true> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::typeKeyword:	std::cout << "<typeKeyword : type> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::funcKeyword:	std::cout << "<funcKeyword : func> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::returnKeyword:	std::cout << "<returnKeyword : return> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::breakKeyword:	std::cout << "<breakKeyword : break> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::ifKeyword:		std::cout << "<ifKeyword : if> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::forKeyword:		std::cout << "<forKeyword : for> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::whileKeyword:	std::cout << "<whileKeyword : while> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::IntLiteral:		std::cout << "<IntLiteral : " << std::get<uint64_t>(token.value) << "> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::FloatLiteral:	std::cout << "<FloatLiteral : " << std::get<double>(token.value) << "> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::CharLiteral:	std::cout << "<CharLiteral : " << std::string(1, std::get<char>(token.value)) << "  |  (char code: " << (int)std::get<char>(token.value) << ")> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Identifier:		std::cout << "<Identifier : " << std::get<std::string>(token.value) << "> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Semicolon:		std::cout << "<Semicolon> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Assign:			std::cout << "<Assign> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Invalid:		std::cout << "<Invalid : " << std::get<std::string>(token.value) << "> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::LessThan:		std::cout << "<LessThan> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::LessThanEq:		std::cout << "<LessThanEq> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::GreaterThan:	std::cout << "<GreaterThan> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::GreaterThanEq:	std::cout << "<GreaterThanEq> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::LogicalNot:		std::cout << "<LogicalNot> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::NotEq:			std::cout << "<NotEq> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Equality:		std::cout << "<Equality> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::OpenParen:		std::cout << "<OpenParen> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::CloseParen:		std::cout << "<CloseParen> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::OpenCurly:		std::cout << "<OpenCurly> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::CloseCurly:		std::cout << "<CloseCurly> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Colon:			std::cout << "<Colon> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Plus:			std::cout << "<Plus> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Minus:			std::cout << "<Minus> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Asterisk:		std::cout << "<Asterisk> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::ForwardSlash:	std::cout << "<ForwardSlash> (line " << token.lineNum << ")" << std::endl; break;
		case TokenType::Eof:			std::cout << "<EOF> (line " << token.lineNum << ")" << std::endl; break;
	}
}

bool Lexer::lexFile(const std::string& filename)
{
	std::ifstream inFile(filename);

	if (!inFile.is_open())
	{
		std::cout << "Could not open file at path: <" << filename << ">" << std::endl;
		return false;
	}

	fileContents << inFile.rdbuf();
	fileContents.seekp(fileContents.beg);

	// Lex here, then return true indicating the file was correctly lexed. I.E. a stream of tokens was produced from the source code
	while (fileContents)
	{
		char current = getChar();

		// Skip any whitespace
		if ((current == ' ' || current == '\n' || current == '\t') )
		{
			if (state != LexerState::StartState)
				emitToken();

			if (current == '\n')
				lineNum++;
			
			continue;
		}

		if (consumeChar(current))
		{
			ungetChar();
			emitToken();
		}
		else
			currentLiteral += current;
	}

	// Emit an EOF token before returning
	currentToken.type = TokenType::Eof;
	emitToken();
	inFile.close();

	return true;
}

bool Lexer::consumeChar(char current)
{
	state = transitionTable.getTransition(currentToken, state, current);
	return (state == LexerState::AcceptState);
}

void Lexer::emitToken()
{
	switch (currentToken.type)
	{
		case TokenType::Identifier:
		{
			identifierOrKeyword();

			// Only store the string literal value in Identifier tokens. Keywords token string values can be deduced from their TokenType alone
			if (currentToken.type == TokenType::Identifier)
				currentToken.value.emplace<std::string>(currentLiteral);

			break;
		}
		
		case TokenType::IntLiteral:
		{
			currentToken.value = std::stoull(currentLiteral);
			break;
		}

		case TokenType::FloatLiteral:
		{
			// Make sure the float literal is comprised of digits and not just a '.' character before calling std::stod
			if (currentLiteral.length() > 1)
				currentToken.value = std::stod(currentLiteral);
			else
			{
				currentToken.type = TokenType::Invalid;
				currentToken.value.emplace<std::string>(currentLiteral);
			}
			break;
		}

		case TokenType::Invalid:
		{
			currentToken.value.emplace<std::string>(currentLiteral);
			break;
		}

		case TokenType::CharLiteral:
		{
			// Valid char literals should be string of 4 length. I.E. "'a'" -> [', a, ', \0]
			if (currentLiteral.length() == 4)
			{
				currentToken.value = currentLiteral[1];
			}
			else
			{
				currentToken.type = TokenType::Invalid;
				currentToken.value.emplace<std::string>(currentLiteral);
			}
			break;
		}

		case TokenType::EscapedCharLiteral:
		{
			if (validEscapeSequence())
			{
				currentToken.type = TokenType::CharLiteral;
				currentToken.value = validEscapeChars[currentLiteral];
			}
			else
			{
				currentToken.type = TokenType::Invalid;
				currentToken.value.emplace<std::string>(currentLiteral);
			}
		}
	}

	currentToken.lineNum = lineNum;
	tokens.emplace_back(currentToken);
	currentLiteral.clear();
	state = LexerState::StartState;
	currentToken.type = TokenType::None;
}

bool Lexer::validEscapeSequence()
{
	return (validEscapeChars.find(currentLiteral) != validEscapeChars.end());
}

void Lexer::identifierOrKeyword()
{
	if (keywords.find(currentLiteral) != keywords.end())
		currentToken.type = keywords[currentLiteral];
	else
		currentToken.type = TokenType::Identifier;
}

char Lexer::getChar()
{
	char current = fileContents.get();
	//fileContents >> current;
	return current;
}

void Lexer::ungetChar()
{
	fileContents.seekg(-1, std::ios::cur);
}