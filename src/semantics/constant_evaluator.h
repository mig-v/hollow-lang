#pragma once

#include "core/ast_visitor.h"

struct ConstantValue
{
	bool valid;
	int64_t value;
};

// very simple constant evaluator, has limited type info since this needs to be done during symbol resolution for array sizes
// will only work on integers. Constant Folding will be the full system that works on everything and has type information to
// optimize out constant expressions
class ConstantEvaluator : public ASTVisitor
{
public:
	ConstantEvaluator();
	ConstantValue eval(ASTExpr* expr);

	void visitIntLiteral(ASTIntLiteral& node) override;
	void visitBinaryExpr(ASTBinaryExpr& node) override;
	void visitUnaryExpr(ASTUnaryExpr& node) override;
	void visitGroupExpr(ASTGroupExpr& node) override;

	ConstantValue constant;
};