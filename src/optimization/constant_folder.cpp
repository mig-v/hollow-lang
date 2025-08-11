#include "optimization/constant_folder.h"

#include "utils/semantic_utils.h"

#include <iostream>

bool ConstantFolder::run(IRData* data)
{
	this->data = data;
	this->changed = false;

	for (IRFunction* function : data->functionTable)
		foldFunction(*function);

	return changed;
}

void ConstantFolder::foldFunction(IRFunction& function)
{
	for (TacInstruction& instruction : function.body)
	{
		switch (instruction.op)
		{
			case TacOpcode::JmpIfFalse:
			case TacOpcode::JmpIfTrue:
				tryFoldConditionalJump(instruction);
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
				tryFoldArithmeticInstruction(instruction);
				break;
				
			case TacOpcode::NotEq:
			case TacOpcode::Eq:
			case TacOpcode::Gt:
			case TacOpcode::Gte:
			case TacOpcode::Lt:
			case TacOpcode::Lte:
				tryFoldComparisonInstruction(instruction);
				break;

			case TacOpcode::Neg:
			case TacOpcode::BitNot:
			case TacOpcode::Not:
				tryFoldUnaryInstruction(instruction);
				break;

			case TacOpcode::LogicalAnd:
			case TacOpcode::LogicalOr:
				tryFoldLogicalInstruction(instruction);
				break;

			case TacOpcode::LoweredCmpJmp:
				tryFoldLoweredCmpJmpInstruction(instruction);
				break;
		}
	}
}

void ConstantFolder::tryFoldConditionalJump(TacInstruction& instruction)
{
	// transform conditional jumps into a non conditional jmp if the branch is taken, if the branch can never be taken, remove jmp entirely
	if (instruction.src1.kind != ValueKind::Constant)
		return;

	bool condition = std::get<bool>(instruction.src1.data);

	if (instruction.op == TacOpcode::JmpIfTrue)
	{
		if (condition)
			instruction.op = TacOpcode::Jmp;
		else
			instruction.op = TacOpcode::Dead;
	}
	else
	{
		if (!condition)
			instruction.op = TacOpcode::Jmp;
		else
			instruction.op = TacOpcode::Dead;
	}

	instruction.src1.kind = ValueKind::None;

}

void ConstantFolder::tryFoldComparisonInstruction(TacInstruction& instruction)
{
	// if either operands are not constants, early return
	if (instruction.src1.kind != ValueKind::Constant || instruction.src2.kind != ValueKind::Constant)
		return;

	if (SemanticUtils::isSigned(instruction.src1.type))
	{
		bool result = foldIntComparisonInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<bool>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<bool>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (SemanticUtils::isUnsigned(instruction.src1.type))
	{
		bool result = foldUIntComparisonInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<bool>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<bool>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (SemanticUtils::isFloat(instruction.src1.type))
	{
		bool result = foldFloatComparisonInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<bool>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<bool>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (instruction.src1.type == TypeKind::Bool)
	{
		bool result = foldBoolComparisonInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<bool>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<bool>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
}

void ConstantFolder::tryFoldLogicalInstruction(TacInstruction& instruction)
{
	// if either operands are not constants, early return
	if (instruction.src1.kind != ValueKind::Constant || instruction.src2.kind != ValueKind::Constant)
		return;

	bool result;

	if (instruction.op == TacOpcode::LogicalAnd)
		result = std::get<bool>(instruction.src1.data) && std::get<bool>(instruction.src2.data);
	else
		result = std::get<bool>(instruction.src1.data) || std::get<bool>(instruction.src2.data);

	instruction.op = TacOpcode::Constant;
	instruction.dest.data.emplace<bool>(result);
	instruction.dest.kind = ValueKind::Constant;

	data->values[instruction.dest.id].data.emplace<bool>(result);
	data->values[instruction.dest.id].kind = ValueKind::Constant;
	changed = true;
}

void ConstantFolder::tryFoldLoweredCmpJmpInstruction(TacInstruction& instruction)
{
	if (instruction.src1.kind != ValueKind::Constant || instruction.src2.kind != ValueKind::Constant)
		return;

	bool condition = false;

	switch (instruction.jumpKind)
	{
		case JumpKind::Je:  condition = (instruction.src1.data == instruction.src2.data); break;
		case JumpKind::Jne: condition = (instruction.src1.data != instruction.src2.data); break;
		case JumpKind::Jg:  condition = (instruction.src1.data > instruction.src2.data);  break;
		case JumpKind::Jge: condition = (instruction.src1.data >= instruction.src2.data); break;
		case JumpKind::Jl:  condition = (instruction.src1.data < instruction.src2.data);  break;
		case JumpKind::Jle: condition = (instruction.src1.data <= instruction.src2.data); break;
	}

	// if the condition is true, we can replace the cond instruction with a non-conditional jump to the label
	if (condition)
		instruction.op = TacOpcode::Jmp;

	// if the condition is not true, then the jump will never fire, so we can mark the instruction as dead so DCE can remove it
	else
		instruction.op = TacOpcode::Dead;

	// not strictly necessary, since when a Jmp opcode is encountered, only the dest ID is used, but to be safe
	// we can null out the valueKind of the other operands to be extra safe
	instruction.src1.kind = ValueKind::None;
	instruction.src2.kind = ValueKind::None;
}

bool ConstantFolder::foldIntComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::NotEq: return std::get<int64_t>(lhs.data) != std::get<int64_t>(rhs.data);
		case TacOpcode::Eq:    return std::get<int64_t>(lhs.data) == std::get<int64_t>(rhs.data);
		case TacOpcode::Gt:    return std::get<int64_t>(lhs.data) > std::get<int64_t>(rhs.data);
		case TacOpcode::Gte:   return std::get<int64_t>(lhs.data) >= std::get<int64_t>(rhs.data);
		case TacOpcode::Lt:    return std::get<int64_t>(lhs.data) < std::get<int64_t>(rhs.data);
		case TacOpcode::Lte:   return std::get<int64_t>(lhs.data) <= std::get<int64_t>(rhs.data);
	}
}

bool ConstantFolder::foldUIntComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::NotEq: return std::get<uint64_t>(lhs.data) != std::get<uint64_t>(rhs.data);
		case TacOpcode::Eq:    return std::get<uint64_t>(lhs.data) == std::get<uint64_t>(rhs.data);
		case TacOpcode::Gt:    return std::get<uint64_t>(lhs.data) > std::get<uint64_t>(rhs.data);
		case TacOpcode::Gte:   return std::get<uint64_t>(lhs.data) >= std::get<uint64_t>(rhs.data);
		case TacOpcode::Lt:    return std::get<uint64_t>(lhs.data) < std::get<uint64_t>(rhs.data);
		case TacOpcode::Lte:   return std::get<uint64_t>(lhs.data) <= std::get<uint64_t>(rhs.data);
	}
}

bool ConstantFolder::foldFloatComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::NotEq: return std::get<double>(lhs.data) != std::get<double>(rhs.data);
		case TacOpcode::Eq:    return std::get<double>(lhs.data) == std::get<double>(rhs.data);
		case TacOpcode::Gt:    return std::get<double>(lhs.data) > std::get<double>(rhs.data);
		case TacOpcode::Gte:   return std::get<double>(lhs.data) >= std::get<double>(rhs.data);
		case TacOpcode::Lt:    return std::get<double>(lhs.data) < std::get<double>(rhs.data);
		case TacOpcode::Lte:   return std::get<double>(lhs.data) <= std::get<double>(rhs.data);
	}
}

bool ConstantFolder::foldBoolComparisonInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::NotEq: return std::get<bool>(lhs.data) != std::get<bool>(rhs.data);
		case TacOpcode::Eq:    return std::get<bool>(lhs.data) == std::get<bool>(rhs.data);
	}
}

void ConstantFolder::tryFoldUnaryInstruction(TacInstruction& instruction)
{
	// !x, ~x, -x
	if (instruction.src1.kind != ValueKind::Constant)
		return;

	if (instruction.op == TacOpcode::Not)
	{
		bool result = !std::get<bool>(instruction.src1.data);
		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<bool>(result);
		instruction.dest.kind = ValueKind::Constant;
		std::cout << "folding unary logical not, value to : " << result << std::endl;
		data->values[instruction.dest.id].data.emplace<bool>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (instruction.op == TacOpcode::BitNot)
	{
		if (SemanticUtils::isSigned(instruction.src1.type))
		{
			int64_t result = ~std::get<int64_t>(instruction.src1.data);

			instruction.op = TacOpcode::Constant;
			instruction.dest.data.emplace<int64_t>(result);
			instruction.dest.kind = ValueKind::Constant;

			data->values[instruction.dest.id].data.emplace<int64_t>(result);
			data->values[instruction.dest.id].kind = ValueKind::Constant;
			changed = true;
		}
		else if (SemanticUtils::isUnsigned(instruction.src1.type))
		{
			uint64_t result = ~std::get<uint64_t>(instruction.src1.data);

			instruction.op = TacOpcode::Constant;
			instruction.dest.data.emplace<uint64_t>(result);
			instruction.dest.kind = ValueKind::Constant;

			data->values[instruction.dest.id].data.emplace<uint64_t>(result);
			data->values[instruction.dest.id].kind = ValueKind::Constant;
			changed = true;
		}
	}
	else if (instruction.op == TacOpcode::Neg)
	{
		if (SemanticUtils::isSigned(instruction.src1.type))
		{
			int64_t result = -std::get<int64_t>(instruction.src1.data);

			instruction.op = TacOpcode::Constant;
			instruction.dest.data.emplace<int64_t>(result);
			instruction.dest.kind = ValueKind::Constant;

			data->values[instruction.dest.id].data.emplace<int64_t>(result);
			data->values[instruction.dest.id].kind = ValueKind::Constant;
			changed = true;
		}
		else if (SemanticUtils::isUnsigned(instruction.src1.type))
		{
			uint64_t result = -std::get<uint64_t>(instruction.src1.data);

			instruction.op = TacOpcode::Constant;
			instruction.dest.data.emplace<uint64_t>(result);
			instruction.dest.kind = ValueKind::Constant;

			data->values[instruction.dest.id].data.emplace<uint64_t>(result);
			data->values[instruction.dest.id].kind = ValueKind::Constant;
			changed = true;
		}
		else if (SemanticUtils::isFloat(instruction.src1.type))
		{
			double result = -std::get<double>(instruction.src1.data);

			instruction.op = TacOpcode::Constant;
			instruction.dest.data.emplace<double>(result);
			instruction.dest.kind = ValueKind::Constant;

			data->values[instruction.dest.id].data.emplace<double>(result);
			data->values[instruction.dest.id].kind = ValueKind::Constant;
			changed = true;
		}
	}
}

void ConstantFolder::tryFoldArithmeticInstruction(TacInstruction& instruction)
{
	// if either operands are not constants, early return
	if (instruction.src1.kind != ValueKind::Constant || instruction.src2.kind != ValueKind::Constant)
		return;

	//TacValue& dest = data->values[instruction.dest.id];
	if (SemanticUtils::isSigned(instruction.src1.type))
	{
		int64_t result = foldBinaryIntInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;   
		instruction.dest.data.emplace<int64_t>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<int64_t>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (SemanticUtils::isUnsigned(instruction.src1.type))
	{
		uint64_t result = foldBinaryUIntInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<uint64_t>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<uint64_t>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
	else if (SemanticUtils::isFloat(instruction.src1.type))
	{
		double result = foldBinaryFloatInstruction(instruction.src1, instruction.src2, instruction.op);

		instruction.op = TacOpcode::Constant;
		instruction.dest.data.emplace<double>(result);
		instruction.dest.kind = ValueKind::Constant;

		data->values[instruction.dest.id].data.emplace<double>(result);
		data->values[instruction.dest.id].kind = ValueKind::Constant;
		changed = true;
	}
}

int64_t ConstantFolder::foldBinaryIntInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::Add:           return std::get<int64_t>(lhs.data) + std::get<int64_t>(rhs.data);
		case TacOpcode::Sub:           return std::get<int64_t>(lhs.data) - std::get<int64_t>(rhs.data);
		case TacOpcode::Mul:           return std::get<int64_t>(lhs.data) * std::get<int64_t>(rhs.data);
		case TacOpcode::Div:           return std::get<int64_t>(lhs.data) / std::get<int64_t>(rhs.data);
		case TacOpcode::BitAnd:        return std::get<int64_t>(lhs.data) & std::get<int64_t>(rhs.data);
		case TacOpcode::BitOr:         return std::get<int64_t>(lhs.data) | std::get<int64_t>(rhs.data);
		case TacOpcode::BitXor:        return std::get<int64_t>(lhs.data) ^ std::get<int64_t>(rhs.data);
		case TacOpcode::BitShiftLeft:  return std::get<int64_t>(lhs.data) << std::get<int64_t>(rhs.data);
		case TacOpcode::BitShiftRight: return std::get<int64_t>(lhs.data) >> std::get<int64_t>(rhs.data);
	}
}

uint64_t ConstantFolder::foldBinaryUIntInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::Add:           return std::get<uint64_t>(lhs.data) + std::get<uint64_t>(rhs.data);
		case TacOpcode::Sub:           return std::get<uint64_t>(lhs.data) - std::get<uint64_t>(rhs.data);
		case TacOpcode::Mul:           return std::get<uint64_t>(lhs.data) * std::get<uint64_t>(rhs.data);
		case TacOpcode::Div:           return std::get<uint64_t>(lhs.data) / std::get<uint64_t>(rhs.data);
		case TacOpcode::BitAnd:        return std::get<uint64_t>(lhs.data) & std::get<uint64_t>(rhs.data);
		case TacOpcode::BitOr:         return std::get<uint64_t>(lhs.data) | std::get<uint64_t>(rhs.data);
		case TacOpcode::BitXor:        return std::get<uint64_t>(lhs.data) ^ std::get<uint64_t>(rhs.data);
		case TacOpcode::BitShiftLeft:  return std::get<uint64_t>(lhs.data) << std::get<uint64_t>(rhs.data);
		case TacOpcode::BitShiftRight: return std::get<uint64_t>(lhs.data) >> std::get<uint64_t>(rhs.data);
	}
}

double ConstantFolder::foldBinaryFloatInstruction(IROperand& lhs, IROperand& rhs, TacOpcode op)
{
	switch (op)
	{
		case TacOpcode::Add: return std::get<double>(lhs.data) + std::get<double>(rhs.data);
		case TacOpcode::Sub: return std::get<double>(lhs.data) - std::get<double>(rhs.data);
		case TacOpcode::Mul: return std::get<double>(lhs.data) * std::get<double>(rhs.data);
		case TacOpcode::Div: return std::get<double>(lhs.data) / std::get<double>(rhs.data);
	}
}