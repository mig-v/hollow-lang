#pragma once

#include <string>

#include "token.h"

enum class ErrorType
{
	NoError,
	FileDNE,
	SyntaxError,
	SemanticError,
	TypeError
	
	// etc. Add more when needed
};

struct ErrorReport
{
	ErrorType   errorType;
	std::string errorMessage;
	Token       errorToken;
};