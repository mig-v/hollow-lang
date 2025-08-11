//#pragma once
//
//#include "ir/ir_core.h"
//
//#include "archive/register.h"
//
//#include <stdint.h>
//
//struct RegisterInterval
//{
//	uint32_t start; // instruction index the register begins its life
//	uint32_t end;   // last instruction that the register is being used
//	bool crossesCall; // marks if the interval crosses a function call, if it does, prefer non-volatile register
//};
//
//struct Register
//{
//	RegisterInterval interval; // life span of the register relative to the function its in
//	RegisterInfo info;
//	uint32_t tempID;           // IR temp ID
//};
//
//// defines a register that is constrained for a certain instruction
//struct RegisterConstraint
//{
//	std::vector<uint32_t> gpConstrainedRegisters;
//};