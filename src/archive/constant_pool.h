#pragma once

#include "ir/ir_core.h"

#include <array>
#include <vector>

enum class PoolType
{
	f32, f64, NumPoolTypes
};

class ConstantPool
{
public:
	ConstantPool() = default;
	~ConstantPool() = default;
	void allocate(IRData* data);
	void dumpConstants();

	std::vector<float> f32Pool;
	std::vector<double> f64Pool;
private:
	void analyzeFunction(IRFunction* function);
	void tryPoolOperand(IROperand& operand);
	void poolConstant(IROperand& operand);

	bool typeIsPoolable(TypeKind type);
	uint32_t valueAlreadyPooled(float val);
	uint32_t valueAlreadyPooled(double val);

	uint32_t f32PoolID;
	uint32_t f64PoolID;

	const uint32_t INVALID_POOL_ID = UINT32_MAX;
};