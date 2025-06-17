#pragma once

#include <stdint.h>
#include <vector>

#include "token.h"
#include "type_info.h"

// forward declare any visitor classes needed
class ASTPrinter;
class SemanticAnalysis;

class ASTExpr;
class ASTStmt;
class ASTArgument;

class ASTNode
{
public:
	virtual bool operator==(const ASTNode& other) const = 0 ;	// only used for testing the parser, should never be called when actually parsing
	virtual void accept(ASTPrinter visitor) = 0;
	virtual void accept(SemanticAnalysis& visitor) = 0;
};

// only used when testing the parser, this and the operator overload == should never be called when actually parsing
inline bool astEqual(const ASTNode* lhs, const ASTNode* rhs)
{
	if (!lhs || !rhs)
		return lhs == rhs;

	return (*lhs == *rhs);
}

class ASTParameter : public ASTNode
{
public:
	ASTParameter(Token paramIdentifier, Token paramType);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	Token paramIdentifier;
	Token paramType;
	TypeInfo* typeInfo;
};

class ASTParamList : public ASTNode
{
public:
	ASTParamList() = default;

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	std::vector<ASTParameter*> params;
};

class ASTArgList : public ASTNode
{
public:
	ASTArgList() = default;

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor);
	void accept(SemanticAnalysis& visitor);

	std::vector<ASTArgument*> args;
};