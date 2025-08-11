#pragma once

#include "ir/ir_core.h"

class OptimizationPass
{
public:
	virtual bool run(IRData* data) = 0;

protected:
	IRData* data;
	bool changed;
};