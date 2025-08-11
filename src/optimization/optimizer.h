#pragma once

#include "ir/ir_core.h"

enum class OptimizationLevel
{
	O0,
	O1,
	O2
};

class Optimizer
{
public:
	void optimize(IRData* data, OptimizationLevel level);
};