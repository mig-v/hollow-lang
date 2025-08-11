#pragma once

#include "optimization/optimization_pass.h"

#include <unordered_set>

class DeadCodeElimination : public OptimizationPass
{
public:
	DeadCodeElimination() = default;
	~DeadCodeElimination() = default;

	bool run(IRData* data) override;
private:
	void analyzeFunction(IRFunction& function);
	void markOperandsAlive(const TacInstruction& instruction);
	void tryMarkOperandAlive(const IROperand& operand);
	bool instructionHasSideEffects(const TacInstruction& instruction);

	std::unordered_set<uint32_t> aliveTemps;
};