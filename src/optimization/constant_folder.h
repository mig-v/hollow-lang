#pragma once

#include "optimization/optimization_pass.h"

class ConstantFolder : public OptimizationPass
{
public:
	ConstantFolder() = default;
	~ConstantFolder() = default;

	bool run(IRData* data) override;
private:
	void foldFunction(IRFunction& function);

	void tryFoldArithmeticInstruction(TacInstruction& instruction);
	void tryFoldUnaryInstruction(TacInstruction& instruction);
	void tryFoldComparisonInstruction(TacInstruction& instruction);
	void tryFoldLogicalInstruction(TacInstruction& instruction);
	void tryFoldLoweredCmpJmpInstruction(TacInstruction& instruction);
	void tryFoldConditionalJump(TacInstruction& instruction);

	int64_t foldBinaryIntInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
	uint64_t foldBinaryUIntInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
	double foldBinaryFloatInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);

	bool foldIntComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
	bool foldUIntComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
	bool foldFloatComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
	bool foldBoolComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op);
};