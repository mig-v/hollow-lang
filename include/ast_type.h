#pragma once

#include "token.h"

class ASTExpr;

enum class ASTTypeKind
{
	Primitive,
	Array,
	Struct,
	Pointer,
	Function
};

class ASTType
{
public:
	virtual ~ASTType() = default;
	virtual bool operator==(const ASTType& other) const = 0;	// only used for testing the parser, should never be called when actually parsing

	ASTTypeKind astType;
};

// only used for testing, should never be called when actually parsing
inline bool astTypeEqual(const ASTType* lhs, const ASTType* rhs)
{
	if (!lhs || !rhs)
		return lhs == rhs;

	return (*lhs == *rhs);
}

class ASTPrimitiveType : public ASTType
{
public:
	ASTPrimitiveType(TokenType primitiveType);
	bool operator==(const ASTType& other) const override;

	TokenType primitiveType;
};

class ASTArrayType : public ASTType
{
public:
	ASTArrayType(ASTType* elementType, ASTExpr* size);
	bool operator==(const ASTType& other) const override;

	ASTType* elementType;
	ASTExpr* size;
};

class ASTStructType : public ASTType
{
public:

};

class ASTPointerType : public ASTType
{
public:
	ASTPointerType(ASTType* baseType);

	ASTType* baseType;
};