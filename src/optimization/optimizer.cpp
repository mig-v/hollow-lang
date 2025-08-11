#include "optimization/optimizer.h"
#include "optimization/constant_folder.h"
#include "optimization/constant_propagation.h"
#include "optimization/dead_code_elimination.h"

#include <iostream>

void Optimizer::optimize(IRData* data, OptimizationLevel level)
{
	// in the future can do more optimizations / run more iterations of an optimization loop depending on the level
	uint32_t iterationCount = 0;
	bool changed;

	ConstantFolder folder;
	ConstantPropagation propagator;
	DeadCodeElimination dce;

	do
	{
		changed = false;
		changed |= folder.run(data);
		changed |= propagator.run(data);
		changed |= dce.run(data);
		iterationCount++;
	} while (changed);

	std::cout << "Optimizer ran " << iterationCount << " iteration(s)\n";
}