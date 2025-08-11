#include "utils/semantic_utils.h"
#include "utils/debug_utils.h"

#include "parser/ast_type.h"

#include <iostream>

namespace SemanticUtils
{
	TypeKind getTypeFromToken(TokenType token)
	{
		switch (token)
		{
			case TokenType::u8Keyword:     return TypeKind::u8;
			case TokenType::u16Keyword:    return TypeKind::u16;
			case TokenType::u32Keyword:    return TypeKind::u32;
			case TokenType::u64Keyword:    return TypeKind::u64;
			case TokenType::i8Keyword:     return TypeKind::i8;
			case TokenType::i16Keyword:    return TypeKind::i16;
			case TokenType::i32Keyword:    return TypeKind::i32;
			case TokenType::i64Keyword:    return TypeKind::i64;
			case TokenType::f32Keyword:    return TypeKind::f32;
			case TokenType::f64Keyword:    return TypeKind::f64;
			case TokenType::charKeyword:   return TypeKind::Char;
			case TokenType::boolKeyword:   return TypeKind::Bool;
			case TokenType::structKeyword: return TypeKind::Struct;
			case TokenType::voidKeyword:   return TypeKind::Void;
			default:
				return TypeKind::Unknown;
		}
	}

	TypeKind getTypeFromASTType(ASTType* astType)
	{
		switch (astType->astType)
		{
			case ASTTypeKind::Primitive:
				return getTypeFromToken(static_cast<ASTPrimitiveType*>(astType)->primitiveType);
			case ASTTypeKind::Array:
				return TypeKind::Array;
			case ASTTypeKind::Struct:
				return TypeKind::Struct;
			case ASTTypeKind::Pointer:
				return TypeKind::Pointer;
			case ASTTypeKind::Function:
				return TypeKind::Function;
			default:
				return TypeKind::Unknown;
		}
	}

	bool typeIsVoid(TypeInfo* typeInfo)
	{
		// simple case, base type is void
		if (typeInfo->type == TypeKind::Void)
			return true;

		// recursively check to see if the base type of an array is void
		if (typeInfo->type == TypeKind::Array)
			return typeIsVoid(typeInfo->elementType);

		// recursively check to see if the base type of a pointer is void
		if (typeInfo->type == TypeKind::Pointer)
			return typeIsVoid(typeInfo->ptrBaseType);

		return false;
	}

	bool typeIsPrimitive(TypeKind type)
	{
		switch (type)
		{
			case TypeKind::u8:
			case TypeKind::i8:
			case TypeKind::u16:
			case TypeKind::i16:
			case TypeKind::u32:
			case TypeKind::i32:
			case TypeKind::u64:
			case TypeKind::i64:
			case TypeKind::f32:
			case TypeKind::f64:
			case TypeKind::Bool:
			case TypeKind::Char:
				return true;
			default:
				return false;
		}
	}

	bool isComparisonOp(TokenType op)
	{
		switch (op)
		{
			case TokenType::Equality:
			case TokenType::NotEq:
			case TokenType::GreaterThan:
			case TokenType::GreaterThanEq:
			case TokenType::LessThan:
			case TokenType::LessThanEq:
				return true;
			default:
				return false;
		}
	}

	bool isSigned(TypeKind type)
	{
		return (type == TypeKind::i8 || type == TypeKind::i16 || type == TypeKind::i32 || type == TypeKind::i64);
	}

	bool isUnsigned(TypeKind type)
	{
		return (type == TypeKind::u8 || type == TypeKind::u16 || type == TypeKind::u32 || type == TypeKind::u64);

	}

	bool isInteger(TypeKind type)
	{
		return (isSigned(type) || isUnsigned(type));
	}

	bool isFloat(TypeKind type)
	{
		return (type == TypeKind::f32 || type == TypeKind::f64);
	}

	bool isNumeric(TypeKind type)
	{
		return (isSigned(type) || isUnsigned(type) || isFloat(type));
	}

	int getIntegerWidth(TypeKind type)
	{
		switch (type)
		{
			case TypeKind::u8:
			case TypeKind::i8:
				return 1;
			case TypeKind::u16:
			case TypeKind::i16:
				return 2;
			case TypeKind::u32:
			case TypeKind::i32:
				return 4;
			case TypeKind::u64:
			case TypeKind::i64:
				return 8;
		}
	}

	int getFloatWidth(TypeKind type)
	{
		switch (type)
		{
			case TypeKind::f32: return 4;
			case TypeKind::f64: return 8;
		}
	}

	bool isArithmeticOp(TokenType op)
	{
		switch (op)
		{
			case TokenType::Plus:
			case TokenType::Minus:
			case TokenType::Asterisk:
			case TokenType::ForwardSlash:
				return true;
			default:
				return false;
		}
	}

	bool isBitwiseOp(TokenType op)
	{
		switch (op)
		{
			case TokenType::BitwiseAnd:
			case TokenType::BitwiseOr:
			case TokenType::BitwiseXor:
			case TokenType::BitwiseLeftShift:
			case TokenType::BitwiseRightShift:
				return true;
			default:
				return false;
		}
	}

	bool isArithmeticAssignment(TokenType op)
	{
		switch (op)
		{
			case TokenType::PlusEquals:
			case TokenType::MinusEquals:
			case TokenType::TimesEquals:
			case TokenType::DividedEquals:
				return true;
			default:
				return false;
		}
	}

	bool isBitwiseAssignment(TokenType op)
	{
		switch (op)
		{
			case TokenType::BitwiseAndEquals:
			case TokenType::BitwiseOrEquals:
			case TokenType::BitwiseXorEquals:
			case TokenType::BitwiseLeftShiftEquals:
			case TokenType::BitwiseRightShiftEquals:
				return true;
			default:
				return false;
		}
	}
}