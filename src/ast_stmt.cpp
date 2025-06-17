#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_printer.h"
#include "semantic_analysis.h"

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

void ASTVarDecl::accept(ASTPrinter visitor)
{
	visitor.visitVarDecl(*this);
}

void ASTVarDecl::accept(SemanticAnalysis& visitor)
{
	visitor.visitVarDecl(*this);
}

ASTFuncDecl::ASTFuncDecl(Token funcIdentifier, Token returnType, ASTParamList* params, ASTBlock* body)
{
	this->funcIdentifier = funcIdentifier;
	this->returnType = returnType;
	this->params = params;
	this->body = body;
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

void ASTFuncDecl::accept(ASTPrinter visitor)
{
	visitor.visitFuncDecl(*this);
}

void ASTFuncDecl::accept(SemanticAnalysis& visitor)
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

void ASTExprStmt::accept(ASTPrinter visitor)
{
	visitor.visitExprStmt(*this);
}

void ASTExprStmt::accept(SemanticAnalysis& visitor)
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

void ASTReturn::accept(ASTPrinter visitor)
{
	visitor.visitReturn(*this);
}

void ASTReturn::accept(SemanticAnalysis& visitor)
{
	visitor.visitReturn(*this);
}

ASTBlock::ASTBlock()
{
	// by default scopes will create blocks, but control flow nodes like if, for, while, etc. Will manage the creation of scopes
	// and set this createScope bool to false. This only happens / matters during the semantic analysis phase
	this->createScope = true;
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

void ASTBlock::accept(ASTPrinter visitor)
{
	visitor.visitBlock(*this);
}

void ASTBlock::accept(SemanticAnalysis& visitor)
{
	visitor.visitBlock(*this);
}

ASTForLoop::ASTForLoop(ASTStmt* initializer, ASTExpr* condition, ASTExpr* increment, ASTBlock* body)
{
	this->initializer = initializer;
	this->condition = condition;
	this->increment = increment;
	this->body = body;
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

void ASTForLoop::accept(ASTPrinter visitor)
{
	visitor.visitForLoop(*this);
}

void ASTForLoop::accept(SemanticAnalysis& visitor)
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

void ASTIfStatement::accept(ASTPrinter visitor)
{
	visitor.visitIfStatement(*this);
}

void ASTIfStatement::accept(SemanticAnalysis& visitor)
{
	visitor.visitIfStatement(*this);
}