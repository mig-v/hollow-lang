#pragma once

#include <stdint.h>
#include <vector>

#include "lexer/token.h"

#include "core/type_info.h"

// forward declare any visitor classes needed
class ASTVisitor;
class ASTPrinter;
class SemanticAnalysis;
class BytecodeEmitter;
class TypeChecker;
class IRGenerator;
struct IROperand;

class ASTExpr;
class ASTStmt;
class ASTArgument;
class ASTType;

class ASTNode
{
public:
	virtual ~ASTNode() = default;
	virtual bool operator==(const ASTNode& other) const = 0 ;	// only used for testing the parser, should never be called when actually parsing
	virtual void accept(ASTPrinter visitor, uint32_t depth) = 0;
	virtual void accept(ASTVisitor& visitor) = 0;
	virtual IROperand accept(IRGenerator& visitor) = 0;

	uint32_t line;
	uint32_t col;
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
	ASTParameter(Token paramIdentifier, ASTType* type);

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	Token paramIdentifier;
	ASTType* type;
	TypeInfo* typeInfo;
};

class ASTParamList : public ASTNode
{
public:
	ASTParamList() = default;

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	std::vector<ASTParameter*> params;
};

class ASTArgList : public ASTNode
{
public:
	ASTArgList() = default;

	bool operator==(const ASTNode& other) const;
	void accept(ASTPrinter visitor, uint32_t depth);
	void accept(ASTVisitor& visitor);
	IROperand accept(IRGenerator& visitor);

	std::vector<ASTArgument*> args;
};