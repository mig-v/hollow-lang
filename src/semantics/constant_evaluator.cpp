#include "semantics/constant_evaluator.h"

ConstantEvaluator::ConstantEvaluator()
{
	constant.valid = false;
	constant.value = 0;
}

ConstantValue ConstantEvaluator::eval(ASTExpr* expr)
{
	// reset state just in case
	constant.valid = false;
	constant.value = 0;

	expr->accept(*this);
	return constant;
}

void ConstantEvaluator::visitIntLiteral(ASTIntLiteral& node)
{
	constant.valid = true;
	constant.value = static_cast<int64_t>(node.value);
}

void ConstantEvaluator::visitBinaryExpr(ASTBinaryExpr& node)
{
	ConstantValue lhs;
	ConstantValue rhs;

	constant.valid = false;
	node.lhs->accept(*this);
	lhs = constant;

	// return if any of the constants are invalid after visiting them
	if (!lhs.valid)
		return;

	constant.valid = false;
	node.rhs->accept(*this);
	rhs = constant;

	if (!rhs.valid)
		return;

	switch (node.op.type)
	{
		case TokenType::Plus:
		{
			constant.value = lhs.value + rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::Minus:
		{
			constant.value = lhs.value - rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::Asterisk:
		{
			constant.value = lhs.value * rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::ForwardSlash:
		{
			if (rhs.value == 0)
			{
				constant.valid = false;
				return;
			}

			constant.value = lhs.value / rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseLeftShift:
		{
			constant.value = lhs.value << rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseRightShift:
		{
			constant.value = lhs.value >> rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseAnd:
		{
			constant.value = lhs.value & rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseOr:
		{
			constant.value = lhs.value | rhs.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseXor:
		{
			constant.value = lhs.value ^ rhs.value;
			constant.valid = true;
			break;
		}
		default:
			constant.valid = false;
			break;
	}
}

void ConstantEvaluator::visitUnaryExpr(ASTUnaryExpr& node)
{
	ConstantValue nodeVal;
	constant.valid = false;
	node.expr->accept(*this);
	nodeVal = constant;

	if (!nodeVal.valid)
		return;

	switch (node.op.type)
	{
		case TokenType::Minus:
		{
			constant.value = -nodeVal.value;
			constant.valid = true;
			break;
		}
		case TokenType::BitwiseNot:
		{
			constant.value = ~nodeVal.value;
			constant.valid = true;
			break;
		}
		default:
			constant.valid = false;
			break;
	}
}

void ConstantEvaluator::visitGroupExpr(ASTGroupExpr& node)
{
	constant.valid = false;
	node.expr->accept(*this);
}

