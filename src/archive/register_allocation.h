//#pragma once
//
//#include "ir/ir_core.h"
//
//#include "archive/register_allocation_core.h"
//#include "archive/register_pool.h"
//
//#include "codegen/target_info.h"
//
//#include <set>
//
//class RegisterAllocation
//{
//public:
//	void allocate(IRData* data, TargetInfo* targetInfo);
//private:
//	void updateRegInterval(uint32_t instructionIndex, const IROperand& operand);
//	void calcIntervalsForFunction(IRFunction* function);
//	void linearScan();
//	void resetState();
//	void printRegisterAllocation(const Register& reg);
//	void getIntervalRestraints(RegisterInterval interval, std::unordered_set<uint32_t>& outConstraintIDs);
//	void spillActiveConstrainedRegisters(std::list<Register*>& active, std::unordered_set<uint32_t>& constrainedIDs, RegisterInterval interval);
//	void markIntervalsAliveAcrossCalls();
//
//	bool opcodeHasRestraint(TacOpcode opcode);
//
//	RegisterClass getRegisterClass(const IROperand& operand);
//
//	IRData* irData;
//	IRFunction* currentFunction;
//	TargetInfo* targetInfo;
//	RegisterPool registerPool;
//
//	// maps IR temp IDS to an index into the registers vector
//	std::unordered_map<uint32_t, uint32_t> registerMapping;
//	std::vector<Register> registers;
//
//	std::vector<IROperand*> argsForCall;
//
//	// instruction indices of all calls made within the function
//	std::vector<uint32_t> callSites;
//
//	// marks the start of the constrained interval
//	std::set<uint32_t> constrainedInstructions;
//};