#pragma once

#include "ir/ir_core.h"

class IRLowering
{
public:
	void lower(IRData* data);
private:
	void lowerFunction(IRFunction* function);
	uint32_t followConditionChain(uint32_t startID, uint32_t instructionIndex, TacInstruction& conditional);
	bool opcodeIsComparison(TacOpcode opcode);
	JumpKind getJumpKind(TacOpcode comparison, bool negate);

	IRData* data;

	// map of when a tempID was defined, say t0 = neq t1, t2, mapping would be t0 -> 0th instruction
	std::unordered_map<uint32_t, TacInstruction*> valueDefinitions;
	const uint32_t INVALID_INSTRUCTION_INDEX = UINT32_MAX;
};