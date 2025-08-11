#include "optimization/constant_propagation.h"

#include <iostream>

bool ConstantPropagation::run(IRData* data)
{
	this->data = data;
	this->changed = false;

	for (IRFunction* function : data->functionTable)
		analyzeFunction(*function);

	return changed;
}

bool ConstantPropagation::operandCanPropagate(IROperand& operand)
{
	return (operand.kind == ValueKind::Temporary && data->values[operand.id].kind == ValueKind::Constant);
}

void ConstantPropagation::tryPropagateOperand(IROperand& operand)
{
	// check to make sure the operand can be propagated -> criteria -> operand is a temporary whos table entry is a const value
	if (!operandCanPropagate(operand))
		return;

	TacValue& tableValue = data->values[operand.id];

	switch (tableValue.temporaryType)
	{
		case TypeKind::i8:
		case TypeKind::i16:
		case TypeKind::i32:
		case TypeKind::i64:
			operand.data.emplace<int64_t>(std::get<int64_t>(tableValue.data));
			break;
		case TypeKind::u8:
		case TypeKind::u16:
		case TypeKind::u32:
		case TypeKind::u64:
			operand.data.emplace<uint64_t>(std::get<uint64_t>(tableValue.data));
			break;
		case TypeKind::f32:
		case TypeKind::f64:
			operand.data.emplace<double>(std::get<double>(tableValue.data));
			break;
		case TypeKind::Char:
			operand.data.emplace<char>(std::get<char>(tableValue.data));
			break;
		case TypeKind::Bool:
			operand.data.emplace<bool>(std::get<bool>(tableValue.data));
			break;
	}

	operand.type = tableValue.temporaryType;
	operand.kind = ValueKind::Constant;
	changed = true;
}

void ConstantPropagation::analyzeFunction(IRFunction& function)
{
	/*
	constant propagation essentially scaffolds temps that are actually just constants into a constant immediate operand
	so
	-> t0 = add const 10, const 10
	-> t1 = add t0, const 20
	
	-> Run Folder
	-> t0 = const 20
	-> t1 = add t0, const 20

	-> Run Propagation
	-> t0 = const 20
	-> t1 = add const 20, const 20
	-> ... so on
	*/
	for (TacInstruction& instruction : function.body)
	{
		switch (instruction.op)
		{
			case TacOpcode::JmpIfFalse:
			case TacOpcode::JmpIfTrue:
				tryPropagateOperand(instruction.src1);
				break;
				
			case TacOpcode::Add:
			case TacOpcode::Sub:
			case TacOpcode::Mul:
			case TacOpcode::Div:
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
				tryPropagateOperand(instruction.src1);
				tryPropagateOperand(instruction.src2);
				break;

			case TacOpcode::Store:
				tryPropagateOperand(instruction.src1);
				break;
		}
	}
}