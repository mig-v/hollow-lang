//#include "archive/register_allocation.h"
//
//#include "utils/semantic_utils.h"
//#include "utils/debug_utils.h"
//
//#include <iostream>
//#include <algorithm>
//#include <list>
//
//void RegisterAllocation::allocate(IRData* data, TargetInfo* targetInfo)
//{
//	this->irData = data;
//	this->targetInfo = targetInfo;
//	this->currentFunction = nullptr;
//	registerPool.initialize(targetInfo);
//
//	for (IRFunction* function : data->functionTable)
//	{
//		this->currentFunction = function;
//		resetState();
//		calcIntervalsForFunction(function);
//		std::sort(registers.begin(), registers.end(), [](const Register& a, const Register& b) {return a.interval.start < b.interval.start; });
//		markIntervalsAliveAcrossCalls();
//		linearScan();
//		for (const Register& reg : registers)
//		{
//			irData->values[reg.tempID].regInfo = reg.info;
//			printRegisterAllocation(reg);
//		}
//	}
//}
//
//void RegisterAllocation::printRegisterAllocation(const Register& reg)
//{
//	std::cout << "t" << std::to_string(reg.tempID) << " [" << std::to_string(reg.interval.start) << ", " << std::to_string(reg.interval.end) << "]";
//	if (!reg.info.spilled)
//		std::cout << " --> Register ID = " << reg.info.regID << " (" << DebugUtils::registerClassToString(reg.info.regClass) << ")" << std::endl;
//	else
//		std::cout << " --> Spill to stack offset [rbp - " << reg.info.stackOffset << "]\n";
//}
//
//RegisterClass RegisterAllocation::getRegisterClass(const IROperand& operand)
//{
//	TypeKind type = operand.kind == ValueKind::Variable ? irData->values[operand.id].typeInfo->type : operand.type;
//
//	switch (type)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//		case TypeKind::Pointer:
//			return RegisterClass::GP;
//		case TypeKind::f32:
//		case TypeKind::f64:
//			return RegisterClass::FP;
//		default:
//			return RegisterClass::None;
//	}
//}
//
//bool RegisterAllocation::opcodeHasRestraint(TacOpcode opcode)
//{
//	return targetInfo->registerConstraints.find(opcode) != targetInfo->registerConstraints.end();
//}
//
//void RegisterAllocation::getIntervalRestraints(RegisterInterval interval, std::unordered_set<uint32_t>& outConstraintIDs)
//{
//	outConstraintIDs.clear();
//	auto it = constrainedInstructions.lower_bound(interval.start);
//
//	while (it != constrainedInstructions.end() && *it < interval.end)
//	{
//		uint32_t instructionIndex = *it;
//		std::cout << "found constraint on interval [" << interval.start << ", " << interval.end << "] " << "for instruction index " << instructionIndex << std::endl;
//		RegisterConstraint* constraint = &targetInfo->registerConstraints[currentFunction->body[instructionIndex].op];
//		for (uint32_t id : constraint->gpConstrainedRegisters)
//			outConstraintIDs.insert(id);
//
//		it++;
//	}
//}
//
//void RegisterAllocation::spillActiveConstrainedRegisters(std::list<Register*>& active, std::unordered_set<uint32_t>& constrainedIDs, RegisterInterval interval)
//{
//	std::cout << "Spilling active constrained registers!\n";
//	for (auto it = active.begin(); it != active.end();)
//	{
//		Register* activeReg = *it;
//		if ((!activeReg->info.spilled) && (activeReg->interval.end >= interval.start) && (constrainedIDs.count(activeReg->info.regID)) && (registerPool.registerInUse(*activeReg)))
//		{
//			std::cout << "ACTUALLY DOING SPILL\n";
//			registerPool.allocateSpill(*activeReg);
//			it = active.erase(it);
//		}
//		else
//		{
//			it++;
//		}
//	}
//
//	std::cout << "Done Spilling active constrained registers!\n";
//}
//
//void RegisterAllocation::resetState()
//{
//	registers.clear();
//	registerMapping.clear();
//	registerPool.resetInternalState();
//}
//
//void RegisterAllocation::updateRegInterval(uint32_t instructionIndex, const IROperand& operand)
//{
//	// if more value kinds are added, this may need to be expanded, but right now only temporaries should be given a register interval
//	if (operand.kind == ValueKind::Constant || operand.kind == ValueKind::Param || operand.kind == ValueKind::Variable)
//		return;
//
//	//if (irData->values[operand.id].kind == ValueKind::Variable)
//	//	return;
//
//	// try to find a mapping from tN -> reg, if we find it, update the end interval of the register
//	auto reg = registerMapping.find(operand.id);
//	if (reg != registerMapping.end())
//		registers[reg->second].interval.end = instructionIndex;
//
//	// if we dont find a mapping, create one but only fill in the interval
//	else
//	{
//		Register newReg;
//		newReg.tempID = operand.id;
//		newReg.interval.start = instructionIndex;
//		newReg.interval.end = instructionIndex;
//		newReg.info.regClass = getRegisterClass(operand);
//		registers.emplace_back(newReg);
//		registerMapping[operand.id] = registers.size() - 1;
//
//		if (opcodeHasRestraint(currentFunction->body[instructionIndex].op))
//			constrainedInstructions.insert(instructionIndex);
//	}
//}
//
//void RegisterAllocation::markIntervalsAliveAcrossCalls()
//{
//	// given an interval like [2, 5], if interval.start < callSiteIndex && interval.end > callSiteIndex, it's alive across the call
//	// NOTE: can definitely be improved, but for now it suffices
//	for (size_t i = 0; i < registers.size(); i++)
//	{
//		for (size_t j = 0; j < callSites.size(); j++)
//		{
//			if (registers[i].interval.start < callSites[j] && registers[i].interval.end >= callSites[j])
//			{
//				std::cout << "marking interval as crosses call: [" << registers[i].interval.start << ", " << registers[i].interval.end << "] for call at index: " << j << std::endl;
//				registers[i].interval.crossesCall = true;
//			}
//		}
//	}
//}
//
//void RegisterAllocation::linearScan()
//{
//	std::list<Register*> active;
//	std::unordered_set<uint32_t> constrainedRegIDs;
//
//	for (size_t i = 0; i < registers.size(); i++)
//	{
//		Register& currentReg = registers[i];
//		
//		// expire old intervals and free up those registers
//		for (auto it = active.begin(); it != active.end();)
//		{
//			if ((*it)->interval.end < currentReg.interval.start)
//			{
//				Register* reg = (*it);
//				registerPool.freeRegister(*reg);
//				it = active.erase(it);
//			}
//			else
//				it++;
//		}
//
//		getIntervalRestraints(currentReg.interval, constrainedRegIDs);
//		if (constrainedRegIDs.size() > 0)
//		{
//			spillActiveConstrainedRegisters(active, constrainedRegIDs, currentReg.interval);
//			registerPool.allocateRegisterWithConstraints(currentReg, constrainedRegIDs);
//		}
//		else
//			registerPool.allocateRegister(currentReg);
//
//		if (!currentReg.info.spilled)
//			active.push_back(&currentReg);
//	}
//}
//
//void RegisterAllocation::calcIntervalsForFunction(IRFunction* function)
//{
//	uint32_t instructionIndex = 0;
//	for (TacInstruction& instruction : function->body)
//	{
//		switch (instruction.op)
//		{
//			case TacOpcode::FuncArg:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				argsForCall.push_back(&instruction.dest);
//				break;
//			}
//			case TacOpcode::Call:
//			{
//				callSites.push_back(instructionIndex);
//				updateRegInterval(instructionIndex, instruction.dest);
//
//				for (IROperand* arg : argsForCall)
//					updateRegInterval(instructionIndex, *arg);
//
//				argsForCall.clear();
//				break;
//			}
//
//			case TacOpcode::JmpIfFalse:
//			case TacOpcode::JmpIfTrue:
//			{
//				updateRegInterval(instructionIndex, instruction.src1);
//				break;
//			}
//
//			case TacOpcode::LoweredCmpJmp:
//			{
//				updateRegInterval(instructionIndex, instruction.src1);
//				updateRegInterval(instructionIndex, instruction.src2);
//				break;
//			}
//			case TacOpcode::Constant:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				break;
//			}
//			case TacOpcode::Return:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				break;
//			}
//			case TacOpcode::Store:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				updateRegInterval(instructionIndex, instruction.src1);
//				break;
//			}
//			case TacOpcode::Load:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				updateRegInterval(instructionIndex, instruction.src1);
//				break;
//			}
//			case TacOpcode::Move:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				updateRegInterval(instructionIndex, instruction.src1);
//				break;
//			}
//			case TacOpcode::Mul:
//			case TacOpcode::Add:
//			case TacOpcode::Sub:
//			case TacOpcode::Div:
//			case TacOpcode::Mod:
//			case TacOpcode::BitAnd:
//			case TacOpcode::BitXor:
//			case TacOpcode::BitOr:
//			case TacOpcode::BitShiftLeft:
//			case TacOpcode::BitShiftRight:
//			case TacOpcode::NotEq:
//			case TacOpcode::Eq:
//			case TacOpcode::Gt:
//			case TacOpcode::Gte:
//			case TacOpcode::Lt:
//			case TacOpcode::Lte:
//			{
//				updateRegInterval(instructionIndex, instruction.dest);
//				updateRegInterval(instructionIndex, instruction.src1);
//				updateRegInterval(instructionIndex, instruction.src2);
//				break;
//			}
//		}
//
//		instructionIndex++;
//	}
//}
