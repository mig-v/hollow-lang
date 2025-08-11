#include "optimization/dead_code_elimination.h"

#include <iostream>

bool DeadCodeElimination::run(IRData* data)
{
	this->data = data;
	this->changed = false;

	for (IRFunction* function : data->functionTable)
		analyzeFunction(*function);

	return changed;
}

void DeadCodeElimination::analyzeFunction(IRFunction& function)
{
	aliveTemps.clear();
	uint32_t index = 0;
	for (auto it = function.body.rbegin(); it != function.body.rend(); it++)
	{
		TacInstruction& instruction = *it;
		if (instructionHasSideEffects(instruction) || aliveTemps.find(instruction.dest.id) != aliveTemps.end())
		{
			markOperandsAlive(instruction);
		}
		else
		{
			// mark the instruction as dead
			instruction.op = TacOpcode::Dead;
			changed = true;
		}
	}

	// erase all dead instructions for the function
	function.body.erase(
		std::remove_if(function.body.begin(), function.body.end(), [](const TacInstruction& instr) {return instr.op == TacOpcode::Dead; }),
		function.body.end()
	);
}

void DeadCodeElimination::tryMarkOperandAlive(const IROperand& operand)
{
	if (operand.kind == ValueKind::Temporary || operand.kind == ValueKind::Variable)
		aliveTemps.insert(operand.id);
}

void DeadCodeElimination::markOperandsAlive(const TacInstruction& instruction)
{
	switch (instruction.op)
	{
		//case TacOpcode::Constant:	 

		// unimplemented
		case TacOpcode::StoreArrElem:
			break;

		// should do nothing
		case TacOpcode::FuncArg:
		case TacOpcode::Call:
		case TacOpcode::CallVoid:
			break;
		case TacOpcode::Return:
			tryMarkOperandAlive(instruction.dest);
			break;

		case TacOpcode::Add:
		case TacOpcode::Sub:
		case TacOpcode::Mul:
		case TacOpcode::Div:
		case TacOpcode::Mod:
		case TacOpcode::BitAnd:
		case TacOpcode::BitOr:
		case TacOpcode::BitXor:
		case TacOpcode::BitShiftLeft:
		case TacOpcode::BitShiftRight:
		case TacOpcode::NotEq:
		case TacOpcode::Eq:
		case TacOpcode::Gt:
		case TacOpcode::Gte:
		case TacOpcode::Lt:
		case TacOpcode::Lte:
		case TacOpcode::FieldAddr:
		case TacOpcode::LoadArrElem:
			tryMarkOperandAlive(instruction.src1);
			tryMarkOperandAlive(instruction.src2);
			break;

		case TacOpcode::JmpIfTrue:
		case TacOpcode::JmpIfFalse:
			tryMarkOperandAlive(instruction.src1);
			break;

		// NOTE: for Jmps and labels, can maybe maintain another vector of ids for labels then prune out whole labels if nothing jumps to them?
		case TacOpcode::Jmp:
		case TacOpcode::Label:
			break;

		case TacOpcode::Move:
		case TacOpcode::Store:
		case TacOpcode::Load:
		case TacOpcode::Neg:
		case TacOpcode::Not:
		case TacOpcode::BitNot:
		case TacOpcode::Address:
		case TacOpcode::Dereference:
			tryMarkOperandAlive(instruction.src1);
			break;
	}
}

bool DeadCodeElimination::instructionHasSideEffects(const TacInstruction& instruction)
{
	switch (instruction.op)
	{
		case TacOpcode::Move:
		case TacOpcode::Label:
		case TacOpcode::Store:
		case TacOpcode::Return:
		case TacOpcode::Jmp:
		case TacOpcode::JmpIfTrue:
		case TacOpcode::JmpIfFalse:
		case TacOpcode::LoweredCmpJmp:
		case TacOpcode::Load:
		case TacOpcode::FuncArg:
		// NOTE: im assuming all calls have side effects because I dont have const functions
		case TacOpcode::Call:	
			return true;

		default:
			std::cout << "instruction does not have side effects: " << (int)instruction.op << " dest = " << instruction.dest.id << std::endl;
			return false;
	}
}