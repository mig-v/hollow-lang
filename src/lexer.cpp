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
	keywords["else"] = TokenType::elseKeyword;
	keywords["continue"] = TokenType::continueKeyword;

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
	column = 1;
	tokenStartCol = 1;

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
		case TokenType::i8Keyword:		std::cout << "<i8Keyword  : i8>"; break;
		case TokenType::i16Keyword:		std::cout << "<i16Keyword : i16>"; break;
		case TokenType::i32Keyword:		std::cout << "<i32Keyword : i32>"; break;
		case TokenType::i64Keyword:		std::cout << "<i64Keyword : i64>"; break;
		case TokenType::u8Keyword:		std::cout << "<u8Keyword  : u8>"; break;
		case TokenType::u16Keyword:		std::cout << "<u16Keyword : u16>"; break;
		case TokenType::u32Keyword:		std::cout << "<u32Keyword : u32>"; break;
		case TokenType::u64Keyword:		std::cout << "<u64Keyword : u64>"; break;
		case TokenType::f32Keyword:		std::cout << "<f32Keyword : f32>"; break;
		case TokenType::f64Keyword:		std::cout << "<f64Keyword : f64>"; break;
		case TokenType::charKeyword:	std::cout << "<charKeyword : char>"; break;
		case TokenType::boolKeyword:	std::cout << "<boolKeyword : bool>"; break;
		case TokenType::falseKeyword:	std::cout << "<falseKeyword : false>"; break;
		case TokenType::trueKeyword:	std::cout << "<trueKeyword : true>"; break;
		case TokenType::typeKeyword:	std::cout << "<typeKeyword : type>"; break;
		case TokenType::funcKeyword:	std::cout << "<funcKeyword : func>"; break;
		case TokenType::returnKeyword:	std::cout << "<returnKeyword : return>"; break;
		case TokenType::breakKeyword:	std::cout << "<breakKeyword : break>"; break;
		case TokenType::ifKeyword:		std::cout << "<ifKeyword : if>"; break;
		case TokenType::forKeyword:		std::cout << "<forKeyword : for>"; break;
		case TokenType::whileKeyword:	std::cout << "<whileKeyword : while>"; break;
		case TokenType::elseKeyword:	std::cout << "<elseKeyword : else>"; break;
		case TokenType::continueKeyword:std::cout << "<continueKeyword : continue>"; break;
		case TokenType::IntLiteral:		std::cout << "<IntLiteral : " << std::get<uint64_t>(token.value) << ">"; break;
		case TokenType::FloatLiteral:	std::cout << "<FloatLiteral : " << std::get<double>(token.value) << ">"; break;
		case TokenType::CharLiteral:	std::cout << "<CharLiteral : " << std::string(1, std::get<char>(token.value)) << "  |  (char code: " << (int)std::get<char>(token.value) << ")>"; break;
		case TokenType::Identifier:		std::cout << "<Identifier : " << std::get<std::string>(token.value) << ">"; break;
		case TokenType::Semicolon:		std::cout << "<Semicolon>"; break;
		case TokenType::Assign:			std::cout << "<Assign>"; break;
		case TokenType::Invalid:		std::cout << "<Invalid : " << std::get<std::string>(token.value) << ">"; break;
		case TokenType::LessThan:		std::cout << "<LessThan>"; break;
		case TokenType::LessThanEq:		std::cout << "<LessThanEq>"; break;
		case TokenType::GreaterThan:	std::cout << "<GreaterThan>"; break;
		case TokenType::GreaterThanEq:	std::cout << "<GreaterThanEq>"; break;
		case TokenType::LogicalNot:		std::cout << "<LogicalNot>"; break;
		case TokenType::NotEq:			std::cout << "<NotEq>"; break;
		case TokenType::Equality:		std::cout << "<Equality>"; break;
		case TokenType::OpenParen:		std::cout << "<OpenParen>"; break;
		case TokenType::CloseParen:		std::cout << "<CloseParen>"; break;
		case TokenType::OpenCurly:		std::cout << "<OpenCurly>"; break;
		case TokenType::CloseCurly:		std::cout << "<CloseCurly>"; break;
		case TokenType::Colon:			std::cout << "<Colon>"; break;
		case TokenType::Comma:			std::cout << "<Comma>"; break;
		case TokenType::Plus:			std::cout << "<Plus>"; break;
		case TokenType::Minus:			std::cout << "<Minus>"; break;
		case TokenType::Asterisk:		std::cout << "<Asterisk>"; break;
		case TokenType::ForwardSlash:	std::cout << "<ForwardSlash>"; break;
		case TokenType::LogicalAnd:     std::cout << "<LogicalAnd>"; break;
		case TokenType::LogicalOr:      std::cout << "<LogicalOr>"; break;
		case TokenType::BitwiseAnd:     std::cout << "<BitwiseAnd>"; break;
		case TokenType::BitwiseOr:      std::cout << "<BitwiseOr>"; break;
		case TokenType::BitwiseXor:     std::cout << "<BitwiseXor>"; break;
		case TokenType::BitwiseNot:     std::cout << "<BitwiseNot>"; break;
		case TokenType::BitwiseLeftShift:  std::cout << "<BitwiseLeftShift>"; break;
		case TokenType::BitwiseRightShift: std::cout << "<BitwiseRightShift>"; break;
		case TokenType::Increment:         std::cout << "<Increment ++>"; break;
		case TokenType::Decrement:         std::cout << "<Decrement -->"; break;
		case TokenType::PlusEquals:        std::cout << "<PlusEquals +=>"; break;
		case TokenType::MinusEquals:       std::cout << "<MinusEquals -=>"; break;
		case TokenType::TimesEquals:       std::cout << "<TimesEquals *=>"; break;
		case TokenType::DividedEquals:     std::cout << "<DividedEquals /=>"; break;
		case TokenType::DotOperator:     std::cout << "<DotOperator .>"; break;
		case TokenType::OpenBracket:     std::cout << "<OpenBracket [>"; break;
		case TokenType::CloseBracket:     std::cout << "<CloseBracket ]>"; break;
		case TokenType::BitwiseAndEquals:     std::cout << "<BitwiseAndEquals &=>"; break;
		case TokenType::BitwiseOrEquals:     std::cout << "<BitwiseOrEquals |=>"; break;
		case TokenType::BitwiseXorEquals:        std::cout << "<BitwiseXorEquals ^=>"; break;
		case TokenType::BitwiseLeftShiftEquals:  std::cout << "<BitwiseLeftShiftEquals <<= >"; break;
		case TokenType::BitwiseRightShiftEquals: std::cout << "<BitwiseRightShiftEquals >>= >"; break;
		case TokenType::Eof:			std::cout << "<EOF>"; break;
	}

	std::cout << " line " << token.lineNum << " col " << token.column << std::endl;
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
		if ((current == ' ' || current == '\n' || current == '\t'))
		{
			if (state != LexerState::StartState && state != LexerState::CommentState)
				emitToken();

			if (current == '\n')
			{
				if (state == LexerState::CommentState)
					emitToken();

				lineNum++;
				column = 1;
				tokenStartCol = column;
			}
			
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
			// Valid char literals should be string of 3 length. I.E. "'a'" -> [', a, ']
			if (currentLiteral.length() == 3)
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
	currentToken.column = tokenStartCol;
	
	// only store non-comment tokens
	if (currentToken.type != TokenType::Comment)
		tokens.emplace_back(currentToken);

	currentLiteral.clear();
	state = LexerState::StartState;
	currentToken.type = TokenType::None;
	tokenStartCol = column;
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
	column++;
	//fileContents >> current;
	return current;
}

void Lexer::ungetChar()
{
	fileContents.seekg(-1, std::ios::cur);
	column--;
}