#include "ast_type.h"
#include "ast_expr.h"

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

ASTArrayType::ASTArrayType(ASTType* elementType, ASTExpr* size)
{
	this->elementType = elementType;
	this->size = size;
	this->astType = ASTTypeKind::Array;
}

bool ASTArrayType::operator==(const ASTType& other) const
{
	//if (const ASTArrayType* type = dynamic_cast<const ASTArrayType*>(&other))
	//	return ((type->astType == this->astType) && type.);

	return false;
}

ASTPointerType::ASTPointerType(ASTType* baseType)
{
	this->baseType = baseType;
}

