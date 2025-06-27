#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_printer.h"
#include "semantic_analysis.h"
#include "bytecode_emitter.h"
#include "type_checker.h"

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

void ASTIntLiteral::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitIntLiteral(*this, depth);
}

void ASTIntLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitIntLiteral(*this);
}

void ASTIntLiteral::accept(BytecodeEmitter& visitor)
{
	visitor.visitIntLiteral(*this);
}

void ASTIntLiteral::accept(TypeChecker& visitor)
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
	{
		return std::fabs(node->value - this->value) < std::numeric_limits<double>::epsilon();
	}

	return false;
}

void ASTDoubleLiteral::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitDoubleLiteral(*this, depth);
}

void ASTDoubleLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitDoubleLiteral(*this);
}

void ASTDoubleLiteral::accept(BytecodeEmitter& visitor)
{
	visitor.visitDoubleLiteral(*this);
}

void ASTDoubleLiteral::accept(TypeChecker& visitor)
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

void ASTCharLiteral::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitCharLiteral(*this, depth);
}

void ASTCharLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitCharLiteral(*this);
}

void ASTCharLiteral::accept(BytecodeEmitter& visitor)
{
	visitor.visitCharLiteral(*this);
}

void ASTCharLiteral::accept(TypeChecker& visitor)
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

void ASTBoolLiteral::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitBoolLiteral(*this, depth);
}

void ASTBoolLiteral::accept(SemanticAnalysis& visitor)
{
	visitor.visitBoolLiteral(*this);
}

void ASTBoolLiteral::accept(BytecodeEmitter& visitor)
{
	visitor.visitBoolLiteral(*this);
}

void ASTBoolLiteral::accept(TypeChecker& visitor)
{
	visitor.visitBoolLiteral(*this);
}

ASTIdentifier::ASTIdentifier(Token identifier)
{
	this->identifier = identifier;
	this->typeInfo = nullptr;
	this->scope = 0;
	this->slotIndex = 0;
}

bool ASTIdentifier::operator==(const ASTNode& other) const
{
	if (const ASTIdentifier* node = dynamic_cast<const ASTIdentifier*>(&other))
		return ((std::get<std::string>(node->identifier.value) == std::get<std::string>(this->identifier.value)));

	return false;
}

void ASTIdentifier::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitIdentifier(*this, depth);
}

void ASTIdentifier::accept(SemanticAnalysis& visitor)
{
	visitor.visitIdentifier(*this);
}

void ASTIdentifier::accept(BytecodeEmitter& visitor)
{
	visitor.visitIdentifier(*this);
}

void ASTIdentifier::accept(TypeChecker& visitor)
{
	visitor.visitIdentifier(*this);
}

ASTAssign::ASTAssign(ASTExpr* assignee, Token op, ASTExpr* value)
{
	this->assignee = assignee;
	this->op = op;
	this->value = value;
	this->typeInfo = nullptr;
}

bool ASTAssign::operator==(const ASTNode& other) const
{
	if (const ASTAssign* node = dynamic_cast<const ASTAssign*>(&other))
	{
		return ((astEqual(node->assignee, this->assignee))
			&& (node->op.type == this->op.type)
			&& (astEqual(node->value, this->value)));
	}

	return false;
}

void ASTAssign::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitAssign(*this, depth);
}

void ASTAssign::accept(SemanticAnalysis& visitor)
{
	visitor.visitAssign(*this);
}

void ASTAssign::accept(BytecodeEmitter& visitor)
{
	visitor.visitAssign(*this);
}

void ASTAssign::accept(TypeChecker& visitor)
{
	visitor.visitAssign(*this);
}

ASTLogical::ASTLogical(ASTExpr* lhs, Token logicalOperator, ASTExpr* rhs)
{
	this->lhs = lhs;
	this->logicalOperator = logicalOperator;
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

void ASTLogical::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitLogical(*this, depth);
}

void ASTLogical::accept(SemanticAnalysis& visitor)
{
	visitor.visitLogical(*this);
}

void ASTLogical::accept(BytecodeEmitter& visitor)
{
	visitor.visitLogical(*this);
}

void ASTLogical::accept(TypeChecker& visitor)
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

void ASTBinaryExpr::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitBinaryExpr(*this, depth);
}

void ASTBinaryExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitBinaryExpr(*this);
}

void ASTBinaryExpr::accept(BytecodeEmitter& visitor)
{
	visitor.visitBinaryExpr(*this);
}

void ASTBinaryExpr::accept(TypeChecker& visitor)
{
	visitor.visitBinaryExpr(*this);
}

ASTUnaryExpr::ASTUnaryExpr(Token op, ASTExpr* expr)
{
	this->op = op;
	this->expr = expr;
	this->typeInfo = nullptr;
	this->slotIndex = 0;
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

void ASTUnaryExpr::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitUnaryExpr(*this, depth);
}

void ASTUnaryExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitUnaryExpr(*this);
}

void ASTUnaryExpr::accept(BytecodeEmitter& visitor)
{
	visitor.visitUnaryExpr(*this);
}

void ASTUnaryExpr::accept(TypeChecker& visitor)
{
	visitor.visitUnaryExpr(*this);
}

ASTCall::ASTCall(ASTExpr* callee, ASTArgList* args)
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

void ASTCall::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitCall(*this, depth);
}

void ASTCall::accept(SemanticAnalysis& visitor)
{
	visitor.visitCall(*this);
}

void ASTCall::accept(BytecodeEmitter& visitor)
{
	visitor.visitCall(*this);
}

void ASTCall::accept(TypeChecker& visitor)
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

void ASTGroupExpr::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitGroupExpr(*this, depth);
}

void ASTGroupExpr::accept(SemanticAnalysis& visitor)
{
	visitor.visitGroupExpr(*this);
}

void ASTGroupExpr::accept(BytecodeEmitter& visitor)
{
	visitor.visitGroupExpr(*this);
}

void ASTGroupExpr::accept(TypeChecker& visitor)
{
	visitor.visitGroupExpr(*this);
}

ASTPostfix::ASTPostfix(ASTExpr* expr, Token op)
{
	this->expr = expr;
	this->op = op;
	this->typeInfo = nullptr;
	this->slotIndex = 0;
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

void ASTPostfix::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitPostfix(*this, depth);
}

void ASTPostfix::accept(SemanticAnalysis& visitor)
{
	visitor.visitPostfix(*this);
}

void ASTPostfix::accept(BytecodeEmitter& visitor)
{
	visitor.visitPostfix(*this);
}

void ASTPostfix::accept(TypeChecker& visitor)
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

void ASTArgument::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitArgument(*this, depth);
}

void ASTArgument::accept(SemanticAnalysis& visitor)
{
	visitor.visitArgument(*this);
}

void ASTArgument::accept(BytecodeEmitter& visitor)
{
	visitor.visitArgument(*this);
}

void ASTArgument::accept(TypeChecker& visitor)
{
	visitor.visitArgument(*this);
}

ASTCast::ASTCast(ASTExpr* expr)
{
	this->expr = expr;
}

bool ASTCast::operator==(const ASTNode& other) const
{
	if (const ASTCast* node = dynamic_cast<const ASTCast*>(&other))
		return (astEqual(node->expr, this->expr));

	return false;
}

void ASTCast::accept(ASTPrinter visitor, uint32_t depth)
{
	visitor.visitCast(*this, depth);
}

void ASTCast::accept(SemanticAnalysis& visitor)
{
	visitor.visitCast(*this);
}

void ASTCast::accept(BytecodeEmitter& visitor)
{
	visitor.visitCast(*this);
}

void ASTCast::accept(TypeChecker& visitor)
{
	visitor.visitCast(*this);
}
