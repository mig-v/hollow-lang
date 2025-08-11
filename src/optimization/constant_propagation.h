#pragma once

#include "optimization/optimization_pass.h"

#include "ir/ir_core.h"

class ConstantPropagation : public OptimizationPass
{
public:
	ConstantPropagation() = default;
	~ConstantPropagation() = default;
	bool run(IRData* data) override;

private:
	void analyzeFunction(IRFunction& function);
	bool operandCanPropagate(IROperand& operand);
	void tryPropagateOperand(IROperand& operand);
};