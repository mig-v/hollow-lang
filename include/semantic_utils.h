#pragma once

#include "type_info.h"
#include "token.h"
#include "diagnostic.h"

namespace SemanticUtils
{
	TypeKind getTypeFromToken(TokenType token);
	bool isComparisonOp(TokenType op);
	bool isArithmeticOp(TokenType op);
	bool isBitwiseOp(TokenType op);
	bool isSigned(TypeKind type);
	bool isUnsigned(TypeKind type);
	bool isInteger(TypeKind type);
	bool isFloat(TypeKind type);
	bool isNumeric(TypeKind type);
	bool isArithmeticAssignment(TokenType op);
	bool isBitwiseAssignment(TokenType op);
	int  getIntegerWidth(TypeKind type);
	int  getFloatWidth(TypeKind type);
}