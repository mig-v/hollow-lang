#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_printer.h"
#include "semantic_analysis.h"
#include "bytecode_emitter.h"
#include "type_checker.h"

ASTVarDecl::ASTVarDecl(TokenType varType, Token varIdentifier, ASTExpr* initialization)
{
	this->varType = varType;
	this->varIdentifier = varIdentifier;
	this->initialization = initialization;
	this->scope = 0;
	this->slotIndex = 0;
	this->typeInfo = nullptr;
}

bool ASTVarDecl::operator==(const ASTNode& other) const
{
	if (const ASTVarDecl* node = dynamic_cast<const ASTVarDecl*>(&other))
	{
		return (astEqual(node->initialization, this->initialization))
			&& (node->varType == this->varType)
			&& (std::get<std::string>(node->varIdentifier.value) == std::get<std::string>(this->varIdentifier.value));
	}

	return false;
}

void ASTVarDecl::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitVarDecl(*this, depth);
}

void ASTVarDecl::accept(ASTVisitor& visitor)
{
	visitor.visitVarDecl(*this);
}

ASTFuncDecl::ASTFuncDecl(Token funcIdentifier, Token returnType, ASTParamList* params, ASTBlock* body)
{
	this->funcIdentifier = funcIdentifier;
	this->returnType = returnType;
	this->params = params;
	this->body = body;
	this->typeInfo = nullptr;
	this->scope = nullptr;
}

bool ASTFuncDecl::operator==(const ASTNode& other) const
{
	if (const ASTFuncDecl* node = dynamic_cast<const ASTFuncDecl*>(&other))
	{
		return ((std::get<std::string>(node->funcIdentifier.value) == std::get<std::string>(this->funcIdentifier.value))
			&& (astEqual(node->body, this->body))
			&& (astEqual(node->params, this->params)));
	}

	return false;
}

void ASTFuncDecl::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitFuncDecl(*this, depth);
}

void ASTFuncDecl::accept(ASTVisitor& visitor)
{
	visitor.visitFuncDecl(*this);
}

ASTExprStmt::ASTExprStmt(ASTExpr* expression)
{
	this->expression = expression;
}

bool ASTExprStmt::operator==(const ASTNode& other) const
{
	if (const ASTExprStmt* node = dynamic_cast<const ASTExprStmt*>(&other))
		return (astEqual(node->expression, this->expression));

	return false;
}

void ASTExprStmt::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitExprStmt(*this, depth);
}

void ASTExprStmt::accept(ASTVisitor& visitor)
{
	visitor.visitExprStmt(*this);
}

ASTReturn::ASTReturn(ASTExpr* returnVal)
{
	this->returnVal = returnVal;
}

bool ASTReturn::operator==(const ASTNode& other) const
{
	if (const ASTReturn* node = dynamic_cast<const ASTReturn*>(&other))
		return (astEqual(node->returnVal, this->returnVal));

	return false;
}

void ASTReturn::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitReturn(*this, depth);
}

void ASTReturn::accept(ASTVisitor& visitor)
{
	visitor.visitReturn(*this);
}

ASTBlock::ASTBlock()
{
	// by default scopes will create blocks, but control flow nodes like if, for, while, etc. Will manage the creation of scopes
	// and set this createScope bool to false. This only happens / matters during the semantic analysis phase
	this->createScope = true;
	this->scope = nullptr;
}

bool ASTBlock::operator==(const ASTNode& other) const
{
	if (const ASTBlock* node = dynamic_cast<const ASTBlock*>(&other))
	{
		if (node->statements.size() != this->statements.size())
			return false;

		for (size_t i = 0; i < this->statements.size(); i++)
		{
			if (!(astEqual(node->statements[i], this->statements[i])))
				return false;
		}

		return true;
	}

	return false;
}

void ASTBlock::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitBlock(*this, depth);
}

void ASTBlock::accept(ASTVisitor& visitor)
{
	visitor.visitBlock(*this);
}

ASTForLoop::ASTForLoop(ASTStmt* initializer, ASTExpr* condition, ASTExpr* increment, ASTBlock* body)
{
	this->initializer = initializer;
	this->condition = condition;
	this->increment = increment;
	this->body = body;
	this->scope = nullptr;
}

bool ASTForLoop::operator==(const ASTNode& other) const
{
	if (const ASTForLoop* node = dynamic_cast<const ASTForLoop*>(&other))
	{
		return ((astEqual(node->initializer, this->initializer))
			&& (astEqual(node->condition, this->condition))
			&& (astEqual(node->increment, this->increment))
			&& (astEqual(node->body, this->body)));
	}

	return false;
}

void ASTForLoop::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitForLoop(*this, depth);
}

void ASTForLoop::accept(ASTVisitor& visitor)
{
	visitor.visitForLoop(*this);
}

ASTIfStatement::ASTIfStatement(ASTExpr* condition, ASTStmt* trueBranch, ASTStmt* falseBranch)
{
	this->condition = condition;
	this->trueBranch = trueBranch;
	this->falseBranch = falseBranch;
}

bool ASTIfStatement::operator==(const ASTNode& other) const
{
	if (const ASTIfStatement* node = dynamic_cast<const ASTIfStatement*>(&other))
	{
		return ((astEqual(node->condition, this->condition))
			&& (astEqual(node->trueBranch, this->trueBranch))
			&& (astEqual(node->falseBranch, this->falseBranch)));
	}

	return false;
}

void ASTIfStatement::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitIfStatement(*this, depth);
}

void ASTIfStatement::accept(ASTVisitor& visitor)
{
	visitor.visitIfStatement(*this);
}

ASTWhileLoop::ASTWhileLoop(ASTExpr* condition, ASTBlock* body)
{
	this->condition = condition;
	this->body = body;
}

bool ASTWhileLoop::operator==(const ASTNode& other) const
{
	if (const ASTWhileLoop* node = dynamic_cast<const ASTWhileLoop*>(&other))
	{
		return ((astEqual(node->condition, this->condition))
			&& (astEqual(node->body, this->body)));
	}

	return false;
}

void ASTWhileLoop::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitWhileLoop(*this, depth);
}

void ASTWhileLoop::accept(ASTVisitor& visitor)
{
	visitor.visitWhileLoop(*this);
}
