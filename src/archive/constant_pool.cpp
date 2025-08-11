#include "archive/constant_pool.h"

#include "utils/semantic_utils.h"

#include <iostream>

void ConstantPool::allocate(IRData* data)
{
	this->f32PoolID = 0;
	this->f64PoolID = 0;
	
	for (IRFunction* function : data->functionTable)
		analyzeFunction(function);
}

void ConstantPool::dumpConstants()
{
	for (size_t i = 0; i < f32Pool.size(); i++)
		std::cout << "f32_const_" << i << " = " << f32Pool[i] << std::endl;

	std::cout << std::endl;

	for (size_t i = 0; i < f64Pool.size(); i++)
		std::cout << "f64_const_" << i << " = " << f64Pool[i] << std::endl;
}

void ConstantPool::analyzeFunction(IRFunction* function)
{
	for (TacInstruction& instruction : function->body)
	{
		switch (instruction.op)
		{
			// I dont think the IR will have any constant instructions at this point
			//case TacOpcode::Constant:

			// bitwise operations are only defined for integers and ints dont need to be pooled since they can be inlined in asm
			//case TacOpcode::BitOr:
			//case TacOpcode::BitXor:
			//case TacOpcode::BitAnd:
			//case TacOpcode::BitShiftLeft:
			//case TacOpcode::BitShiftRight:

			// these instructions dont make sense to check for constants
			//case TacOpcode::Address:
			//case TacOpcode::Dereference:
			//case TacOpcode::FieldAddr:
			//case TacOpcode::Call:
			//case TacOpcode::CallVoid:
			//case TacOpcode::LoadArrElem:
			//case TacOpcode::Label:
			//case TacOpcode::Jmp:
			//case TacOpcode::Load:

			// unary ops / operations with one valid operand to check for, need to check just one operand
			case TacOpcode::Neg:
			case TacOpcode::Not:
			case TacOpcode::BitNot:
			case TacOpcode::Store:
			case TacOpcode::Move:
			case TacOpcode::StoreArrElem:
				tryPoolOperand(instruction.src1);
				break;
			case TacOpcode::FuncArg:
			case TacOpcode::Return:
				tryPoolOperand(instruction.dest);
				break;

			// binary ops, need to check both operands
			case TacOpcode::Gt:
			case TacOpcode::Gte:
			case TacOpcode::Lt:
			case TacOpcode::Lte:
			case TacOpcode::LogicalAnd:
			case TacOpcode::LogicalOr:
			case TacOpcode::Add:
			case TacOpcode::Sub:
			case TacOpcode::Mul:
			case TacOpcode::Div:
			case TacOpcode::Mod:
			case TacOpcode::Eq:
			case TacOpcode::NotEq:
			case TacOpcode::LoweredCmpJmp:
				tryPoolOperand(instruction.src1);
				tryPoolOperand(instruction.src2);
				break;
				break;
		}
	}
}

void ConstantPool::poolConstant(IROperand& operand)
{
	if (operand.type == TypeKind::f32)
	{
		// if the value is found in the table, reuse that constant pool entry instead of allocating another one for the same value
		float val = static_cast<float>(std::get<double>(operand.data));

		uint32_t alreadyPooledID = valueAlreadyPooled(val);
		if (alreadyPooledID != INVALID_POOL_ID)
			operand.constPoolID = alreadyPooledID;
		else
		{
			// otherwise, generate a pool entry
			operand.constPoolID = f32Pool.size();
			f32Pool.push_back(val);
		}
	}
	else if (operand.type == TypeKind::f64)
	{
		// same logic as f32
		double val = std::get<double>(operand.data);
		
		uint32_t alreadyPooledID = valueAlreadyPooled(val);
		if (alreadyPooledID != INVALID_POOL_ID)
			operand.constPoolID = alreadyPooledID;
		else
		{
			// otherwise, generate a pool entry
			operand.constPoolID = f64Pool.size();
			f64Pool.push_back(val);
		}
	}
}

bool ConstantPool::typeIsPoolable(TypeKind type)
{
	switch (type)
	{
		case TypeKind::f32:
		case TypeKind::f64:
			return true;
		default:
			return false;
	}
}

uint32_t ConstantPool::valueAlreadyPooled(float val)
{
	for (size_t i = 0; i < f32Pool.size(); i++)
	{
		if (f32Pool[i] == val)
			return i;
	}

	return INVALID_POOL_ID;
}

uint32_t ConstantPool::valueAlreadyPooled(double val)
{
	for (size_t i = 0; i < f64Pool.size(); i++)
	{
		if (f64Pool[i] == val)
			return i;
	}

	return INVALID_POOL_ID;
}

void ConstantPool::tryPoolOperand(IROperand& operand)
{
	// if the operand isnt a constant or is not a poolable type, early return
	if (operand.kind != ValueKind::Constant || !typeIsPoolable(operand.type))
		return;

	poolConstant(operand);
}
