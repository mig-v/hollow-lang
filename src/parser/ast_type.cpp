#include "parser/ast_type.h"
#include "parser/ast_expr.h"

ASTPrimitiveType::ASTPrimitiveType(TokenType primitiveType)
{
	this->primitiveType = primitiveType;
	this->astType = ASTTypeKind::Primitive;
}

bool ASTPrimitiveType::operator==(const ASTType& other) const
{
	if (const ASTPrimitiveType* type = dynamic_cast<const ASTPrimitiveType*>(&other))
		return ((type->astType == this->astType) && type->primitiveType == this->primitiveType);

	return false;
}

ASTArrayType::ASTArrayType(ASTType* elementType, ASTExpr* size, uint8_t dimensionCount)
{
	this->elementType = elementType;
	this->size = size;
	this->astType = ASTTypeKind::Array;
	this->dimensionCount = dimensionCount;
}

bool ASTArrayType::operator==(const ASTType& other) const
{
	if (const ASTArrayType* type = dynamic_cast<const ASTArrayType*>(&other))
		return (astTypeEqual(type->elementType, this->elementType)
			 && type->astType == this->astType
			 && astEqual(type->size, this->size));

	return false;
}

ASTStructType::ASTStructType(Token identifier)
{
	this->identifier = identifier;
	this->astType = ASTTypeKind::Struct;
}

bool ASTStructType::operator==(const ASTType& other) const
{
	if (const ASTStructType* type = dynamic_cast<const ASTStructType*>(&other))
		return (type->astType == this->astType
			&& type->identifier.value == this->identifier.value);

	return false;
}

ASTPointerType::ASTPointerType(ASTType* baseType, uint8_t indirectionLevel)
{
	this->baseType = baseType;
	this->astType = ASTTypeKind::Pointer;
	this->indirectionLevel = indirectionLevel;
}

bool ASTPointerType::operator==(const ASTType& other) const
{
	if (const ASTPointerType* type = dynamic_cast<const ASTPointerType*>(&other))
		return (type->astType == this->astType
			&& astTypeEqual(type->baseType, this->baseType));

	return false;
}


