#include "ir/ir_lowering.h"

#include <iostream>

void IRLowering::lower(IRData* data)
{
	this->data = data;

	for (IRFunction* function : data->functionTable)
		lowerFunction(function);
}

void IRLowering::lowerFunction(IRFunction* function)
{
	valueDefinitions.clear();

	for (size_t i = 0; i < function->body.size(); i++)
	{
		TacInstruction& instruction = function->body[i];
		
		// when we reach a conditional jmp instruction, chase the conditionID chain until we find the root comparison instruction
		// (eq, neq, lt, lte, gt, gte)
		if (instruction.op == TacOpcode::JmpIfFalse || instruction.op == TacOpcode::JmpIfTrue)
		{
			// after following the condition chain, we'll have a range of instruction indices from the root comparison to the conditional jmp
			// we can safely remove all instructions that introduced temps like t4 = not t3
			uint32_t rootInstructionIndex = followConditionChain(instruction.src1.id, i, instruction);

			if (rootInstructionIndex == INVALID_INSTRUCTION_INDEX)
				continue;

			function->body.erase(function->body.begin() + rootInstructionIndex, function->body.begin() + i);
		}

		// otherwise, build a mapping of definitions: id -> instruction
		else
		{
			valueDefinitions[instruction.dest.id] = &instruction;
		}
	}
}

uint32_t IRLowering::followConditionChain(uint32_t startID, uint32_t instructionIndex, TacInstruction& conditional)
{
	uint32_t conditionID = startID;
	uint32_t endIndex = instructionIndex;
	bool negate = (conditional.op == TacOpcode::JmpIfFalse);

	while (true)
	{
		instructionIndex--;
		auto lookup = valueDefinitions.find(conditionID);

		if (lookup == valueDefinitions.end())
			return INVALID_INSTRUCTION_INDEX;

		TacInstruction* instr = lookup->second;

		// root comparison found
		if (opcodeIsComparison(instr->op))
		{
			// modify the JmpIfTrue / JmpIfFalse instruction in place to the lowered version
			// dest still holds the jump label, and src1 and src2 are the operands to be compared
			conditional.src1 = instr->src1;
			conditional.src2 = instr->src2;
			conditional.op = TacOpcode::LoweredCmpJmp;
			conditional.jumpKind = getJumpKind(instr->op, negate);
			break;
		}
		else if (instr->op == TacOpcode::Not)
		{
			// chain negations and continue to follow the condition chain
			negate = !negate;
			conditionID = instr->src1.id;
		}
		else
		{
			// this should never really be reached, but just to be safe we'll return invalid instruction index
			return INVALID_INSTRUCTION_INDEX;
		}
	}

	return instructionIndex;
}

bool IRLowering::opcodeIsComparison(TacOpcode opcode)
{
	switch (opcode)
	{
		case TacOpcode::Eq:
		case TacOpcode::NotEq:
		case TacOpcode::Gt:
		case TacOpcode::Gte:
		case TacOpcode::Lt:
		case TacOpcode::Lte:
			return true;
		default:
			return false;
	}
}

JumpKind IRLowering::getJumpKind(TacOpcode comparison, bool negate)
{
	switch (comparison)
	{
		case TacOpcode::Eq:    return negate ? JumpKind::Jne : JumpKind::Je;
		case TacOpcode::NotEq: return negate ? JumpKind::Je : JumpKind::Jne;
		case TacOpcode::Gt:    return negate ? JumpKind::Jle : JumpKind::Jg;
		case TacOpcode::Gte:   return negate ? JumpKind::Jl : JumpKind::Jge;
		case TacOpcode::Lt:    return negate ? JumpKind::Jge : JumpKind::Jl;
		case TacOpcode::Lte:   return negate ? JumpKind::Jg : JumpKind::Jle;
	}
}