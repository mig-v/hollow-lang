#pragma once

#include "core/type_info.h"
#include "core/diagnostic.h"

#include "lexer/token.h"

class ASTType;

namespace SemanticUtils
{
	TypeKind getTypeFromToken(TokenType token);
	TypeKind getTypeFromASTType(ASTType* astType);
	bool typeIsVoid(TypeInfo* typeInfo);
	bool typeIsPrimitive(TypeKind type);
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