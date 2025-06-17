#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_printer.h"
#include "semantic_analysis.h"

ASTIntLiteral::ASTIntLiteral(uint64_t value)
{
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTIntLiteral::operator==(const ASTNode& other) const
{
	if (const ASTIntLiteral* node = dynamic_cast<const ASTIntLiteral*>(&other))
		return node->value == this->value;

	return false;
}

void ASTIntLiteral::accept(ASTPrinter visitor)
{
	visitor.visitIntLiteral(*this);
}

void ASTIntLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitIntLiteral(*this);
}

ASTDoubleLiteral::ASTDoubleLiteral(double value)
{
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTDoubleLiteral::operator==(const ASTNode& other) const
{
	if (const ASTDoubleLiteral* node = dynamic_cast<const ASTDoubleLiteral*>(&other))
		return node->value == this->value;

	return false;
}

void ASTDoubleLiteral::accept(ASTPrinter visitor)
{
	visitor.visitDoubleLiteral(*this);
}

void ASTDoubleLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitDoubleLiteral(*this);
}

ASTCharLiteral::ASTCharLiteral(char value)
{
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTCharLiteral::operator==(const ASTNode& other) const
{
	if (const ASTCharLiteral* node = dynamic_cast<const ASTCharLiteral*>(&other))
		return node->value == this->value;

	return false;
}

void ASTCharLiteral::accept(ASTPrinter visitor)
{
	visitor.visitCharLiteral(*this);
}

void ASTCharLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitCharLiteral(*this);
}

ASTBoolLiteral::ASTBoolLiteral(bool value)
{
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTBoolLiteral::operator==(const ASTNode& other) const
{
	if (const ASTBoolLiteral* node = dynamic_cast<const ASTBoolLiteral*>(&other))
		return node->value == this->value;

	return false;
}

void ASTBoolLiteral::accept(ASTPrinter visitor)
{
	visitor.visitBoolLiteral(*this);
}

void ASTBoolLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitBoolLiteral(*this);
}

ASTVariable::ASTVariable(Token identifier)
{
	this->identifier = identifier;
	this->typeInfo = nullptr;
}

bool ASTVariable::operator==(const ASTNode& other) const
{
	if (const ASTVariable* node = dynamic_cast<const ASTVariable*>(&other))
		return ((std::get<std::string>(node->identifier.value) == std::get<std::string>(this->identifier.value)));

	return false;
}

void ASTVariable::accept(ASTPrinter visitor)
{
	visitor.visitVariable(*this);
}

void ASTVariable::accept(SemanticAnalysis& visitor)
{
	visitor.visitVariable(*this);
}

ASTAssign::ASTAssign(Token identifier, Token op, ASTExpr* value)
{
	this->identifier = identifier;
	this->op = op;
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTAssign::operator==(const ASTNode& other) const
{
	if (const ASTAssign* node = dynamic_cast<const ASTAssign*>(&other))
	{
		return ((std::get<std::string>(node->identifier.value) == std::get<std::string>(this->identifier.value))
			&& (node->op.type == this->op.type)
			&& (astEqual(node->value, this->value)));
	}

	return false;
}

void ASTAssign::accept(ASTPrinter visitor)
{
	visitor.visitAssign(*this);
}

void ASTAssign::accept(SemanticAnalysis& visitor)
{
	visitor.visitAssign(*this);
}

ASTLogical::ASTLogical(ASTExpr* lhs, Token logicalOperator, ASTExpr* rhs)
{
	this->lhs = lhs;
	this->logicalOperator;
	this->rhs = rhs;
	this->typeInfo = nullptr;
}

bool ASTLogical::operator==(const ASTNode& other) const
{
	if (const ASTLogical* node = dynamic_cast<const ASTLogical*>(&other))
	{
		return ((astEqual(node->lhs, this->lhs))
			&& (astEqual(node->rhs, this->rhs))
			&& (node->logicalOperator.type == this->logicalOperator.type));
	}

	return false;
}

void ASTLogical::accept(ASTPrinter visitor)
{
	visitor.visitLogical(*this);
}

void ASTLogical::accept(SemanticAnalysis& visitor)
{
	visitor.visitLogical(*this);
}

ASTBinaryExpr::ASTBinaryExpr(ASTExpr* lhs, Token op, ASTExpr* rhs)
{
	this->lhs = lhs;
	this->op = op;
	this->rhs = rhs;
	this->typeInfo = nullptr;
}

bool ASTBinaryExpr::operator==(const ASTNode& other) const
{
	if (const ASTBinaryExpr* node = dynamic_cast<const ASTBinaryExpr*>(&other))
	{
		return ((astEqual(node->lhs, this->lhs))
			&& (astEqual(node->rhs, this->rhs))
			&& (node->op.type == this->op.type));
	}

	return false;
}

void ASTBinaryExpr::accept(ASTPrinter visitor)
{
	visitor.visitBinaryExpr(*this);
}

void ASTBinaryExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitBinaryExpr(*this);
}

ASTUnaryExpr::ASTUnaryExpr(Token op, ASTExpr* expr)
{
	this->op = op;
	this->expr = expr;
	this->typeInfo = nullptr;
}

bool ASTUnaryExpr::operator==(const ASTNode& other) const
{
	if (const ASTUnaryExpr* node = dynamic_cast<const ASTUnaryExpr*>(&other))
	{
		return ((node->op.type == this->op.type)
			&& (astEqual(node->expr, this->expr)));
	}

	return false;
}

void ASTUnaryExpr::accept(ASTPrinter visitor)
{
	visitor.visitUnaryExpr(*this);
}

void ASTUnaryExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitUnaryExpr(*this);
}

ASTCall::ASTCall(ASTNode* callee, ASTArgList* args)
{
	this->callee = callee;
	this->args = args;
	this->typeInfo = nullptr;
}

bool ASTCall::operator==(const ASTNode& other) const
{
	if (const ASTCall* node = dynamic_cast<const ASTCall*>(&other))
	{
		return ((astEqual(node->callee, this->callee))
			&& (astEqual(node->args, this->args)));
	}

	return false;
}

void ASTCall::accept(ASTPrinter visitor)
{
	visitor.visitCall(*this);
}

void ASTCall::accept(SemanticAnalysis& visitor)
{
	visitor.visitCall(*this);
}

ASTGroupExpr::ASTGroupExpr(ASTExpr* expr)
{
	this->expr = expr;
	this->typeInfo = nullptr;
}

bool ASTGroupExpr::operator==(const ASTNode& other) const
{
	if (const ASTGroupExpr* node = dynamic_cast<const ASTGroupExpr*>(&other))
		return (astEqual(node->expr, this->expr));

	return false;
}

void ASTGroupExpr::accept(ASTPrinter visitor)
{
	visitor.visitGroupExpr(*this);
}

void ASTGroupExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitGroupExpr(*this);
}

ASTPostfix::ASTPostfix(ASTExpr* expr, Token op)
{
	this->expr = expr;
	this->op = op;
	this->typeInfo = nullptr;
}

bool ASTPostfix::operator==(const ASTNode& other) const
{
	if (const ASTPostfix* node = dynamic_cast<const ASTPostfix*>(&other))
	{
		return ((node->op.type == this->op.type)
			&& (astEqual(node->expr, this->expr)));
	}

	return false;
}

void ASTPostfix::accept(ASTPrinter visitor)
{
	visitor.visitPostfix(*this);
}

void ASTPostfix::accept(SemanticAnalysis& visitor)
{
	visitor.visitPostfix(*this);
}

ASTArgument::ASTArgument(ASTExpr* value)
{
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTArgument::operator==(const ASTNode& other) const
{
	if (const ASTArgument* node = dynamic_cast<const ASTArgument*>(&other))
		return (astEqual(node->value, this->value));

	return false;
}

void ASTArgument::accept(ASTPrinter visitor)
{
	visitor.visitArgument(*this);
}

void ASTArgument::accept(SemanticAnalysis& visitor)
{
	visitor.visitArgument(*this);
}