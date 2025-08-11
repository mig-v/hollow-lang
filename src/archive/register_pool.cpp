//#include "archive/register_pool.h"
//
//#include <unordered_set>
//#include <iostream>
//
//void RegisterPool::initialize(TargetInfo* targetInfo)
//{
//	this->targetInfo = targetInfo;
//	this->accumulatedSpillOffset = 0;
//
//	for (size_t i = 0; i < targetInfo->gpRegisters.size(); i++)
//	{
//		PooledRegister reg;
//		reg.inUse = false;
//		reg.traits = targetInfo->gpRegisters[i];
//		gpRegisterPool.emplace_back(reg);
//	}
//
//	for (size_t i = 0; i < targetInfo->fpRegisters.size(); i++)
//	{
//		PooledRegister reg;
//		reg.inUse = false;
//		reg.traits = targetInfo->fpRegisters[i];
//		fpRegisterPool.emplace_back(reg);
//	}
//
//	std::cout << "register pool initialized to " << gpRegisterPool.size() << " GP registers\n";
//	resetInternalState();
//}
//
//void RegisterPool::allocateRegister(Register& reg)
//{
//	if (reg.info.regClass == RegisterClass::GP)
//		allocateGpRegister(reg);
//	else
//		allocateFpRegister(reg);
//}
//
//void RegisterPool::allocateRegisterWithConstraints(Register& reg, std::unordered_set<uint32_t>& constraints)
//{
//	if (constraints.empty())
//	{
//		allocateRegister(reg);
//		return;
//	}
//
//	if (reg.info.regClass == RegisterClass::GP)
//		allocateGpRegisterWithConstraints(reg, constraints);
//
//	// this is assuming that fp registers aren't constrained and can be freely used
//	// if for some reason I come across an instruction that constrains a fp register, I will need to refactor this
//	// and how im storing constraints
//	else if (reg.info.regClass == RegisterClass::FP)
//		allocateRegister(reg);
//}
//
//void RegisterPool::allocateGpRegisterWithConstraints(Register& reg, std::unordered_set<uint32_t>& constraints)
//{
//	for (size_t i = 0; i < gpRegisterPool.size(); i++)
//	{
//		if (constraints.find(/*i*/gpRegisterPool[i].traits.registerID) != constraints.end())
//			continue;
//
//		if (gpRegisterPool[i].inUse)
//			continue;
//
//		// cant just pop back here since we could be taking a register from the middle of the vector
//		reg.info.spilled = false;
//		reg.info.regID = gpRegisterPool[i].traits.registerID;
//		gpRegisterPool[i].inUse = true;
//		return;
//	}
//
//	allocateSpill(reg);
//}
//
//void RegisterPool::freeRegister(Register& reg)
//{
//	// free up a physical register ID if 'reg' was not spilled
//	if (!reg.info.spilled)
//	{
//		if (reg.info.regClass == RegisterClass::GP)
//			freeGPRegister(reg);
//		else if (reg.info.regClass == RegisterClass::FP)
//			freeFPRegister(reg);
//	}
//
//	// otherwise, push the stack offset of the spilled register into the spill pool to reuse spill slots
//	else
//	{
//		spillPool.push_back(reg.info.stackOffset);
//	}
//}
//
//void RegisterPool::freeGPRegister(Register& reg)
//{
//	for (size_t i = 0; i < gpRegisterPool.size(); i++)
//	{
//		if (reg.info.regID == gpRegisterPool[i].traits.registerID)
//		{
//			gpRegisterPool[i].inUse = false;
//			gpRegistersAvailable++;
//			return;
//		}
//	}
//}
//
//void RegisterPool::freeFPRegister(Register& reg)
//{
//	for (size_t i = 0; i < fpRegisterPool.size(); i++)
//	{
//		if (reg.info.regID == fpRegisterPool[i].traits.registerID)
//		{
//			fpRegisterPool[i].inUse = false;
//			return;
//		}
//	}
//}
//
//void RegisterPool::resetInternalState()
//{
//	//gpRegisters.clear();
//	//fpRegisters.clear();
//
//	//gpPool.clear();
//	//fpPool.clear();
//
//	for (size_t i = 0; i < gpRegisterPool.size(); i++)
//		gpRegisterPool[i].inUse = false;
//
//	for (size_t i = 0; i < fpRegisterPool.size(); i++)
//		fpRegisterPool[i].inUse = false;
//
//	gpRegistersAvailable = gpRegisterPool.size();
//	fpRegistersAvailable = fpRegisterPool.size();
//	accumulatedSpillOffset = 0;
//}
//
//void RegisterPool::allocateGpRegister(Register& reg)
//{
//	std::cout << "allocate gp register, available GP registers = " << gpRegistersAvailable << " ";
//	// gp register is available
//	if (gpRegistersAvailable > 0)
//	{
//		// a register is available, so it's not spilled
//		reg.info.spilled = false;
//		gpRegistersAvailable--;
//
//		if (reg.interval.crossesCall)
//		{
//			std::cout << "reg crosses call ";
//			if (!allocateNonVolatileGPRegister(reg))
//				allocateVolatileGPRegister(reg);
//		}
//		else
//		{
//			std::cout << "reg does not cross call ";
//			if (!allocateVolatileGPRegister(reg))
//				allocateNonVolatileGPRegister(reg);
//		}
//	}
//
//	// no registers are available, need to spill
//	else
//	{
//		allocateSpill(reg);
//	}
//
//	std::cout << "allocate gp reg call done!\n";
//}
//
//bool RegisterPool::allocateVolatileGPRegister(Register& reg)
//{
//	for (size_t i = 0; i < gpRegisterPool.size(); i++)
//	{
//		if (gpRegisterPool[i].inUse)
//			continue;
//
//		if (gpRegisterPool[i].traits.isVolatile)
//		{
//			reg.info.regID = gpRegisterPool[i].traits.registerID;
//			gpRegisterPool[i].inUse = true;
//			std::cout << " volatile reg found ID = " << gpRegisterPool[i].traits.registerID << " ";
//
//			return true;
//		}
//	}
//
//	std::cout << " volatile reg NOT found ";
//
//	return false;
//}
//
//bool RegisterPool::allocateNonVolatileGPRegister(Register& reg)
//{
//	for (size_t i = 0; i < gpRegisterPool.size(); i++)
//	{
//		if (gpRegisterPool[i].inUse)
//			continue;
//
//		if (!gpRegisterPool[i].traits.isVolatile)
//		{
//			reg.info.regID = gpRegisterPool[i].traits.registerID;
//			gpRegisterPool[i].inUse = true;
//			std::cout << " non-volatile reg found ID = " << gpRegisterPool[i].traits.registerID << " ";
//			return true;
//		}
//	}
//
//	std::cout << " non-volatile reg NOT found ";
//	return false;
//}
//
//void RegisterPool::allocateFpRegister(Register& reg)
//{
//	// there are available floating point registers
//	//if (volatileFPRegPool.size() > 0)
//	//{
//	//	reg.info.spilled = false;
//	//	reg.info.regID = volatileFPRegPool.back();
//	//	volatileFPRegPool.pop_back();
//	//}
//	//else if (nonVolatileFPRegPool.size() > 0)
//	//{
//	//	reg.info.spilled = false;
//	//	reg.info.regID = nonVolatileFPRegPool.back();
//	//	nonVolatileFPRegPool.pop_back();
//	//}
//
//	//// need to spill
//	//else
//	//{
//	//	allocateSpill(reg);
//	//}
//}
//
//size_t RegisterPool::getNextSpillSlot()
//{
//	// if the allocator didnt have to spill any registers, return 0 as the offset, since this offset is used as the initial stack frame size of the function
//	if (accumulatedSpillOffset == 0)
//		return accumulatedSpillOffset;
//	else
//		return accumulatedSpillOffset + 8;
//}
//
//bool RegisterPool::registerInUse(Register& reg)
//{
//	if (reg.info.regClass == RegisterClass::GP)
//	{
//		for (size_t i = 0; i < gpRegisterPool.size(); i++)
//		{
//			if (reg.info.regID == gpRegisterPool[i].traits.registerID && gpRegisterPool[i].inUse)
//				return true;
//		}
//
//		return false;
//	}
//	else if (reg.info.regClass == RegisterClass::FP)
//	{
//		for (size_t i = 0; i < fpRegisterPool.size(); i++)
//		{
//			if (reg.info.regID == fpRegisterPool[i].traits.registerID && fpRegisterPool[i].inUse)
//				return true;
//		}
//
//		return false;
//	}
//}
//
//void RegisterPool::allocateSpill(Register& reg)
//{
//	std::cout << "allocating spill ";
//	reg.info.spilled = true;
//
//	if (spillPool.size() > 0)
//	{
//		std::cout << "spills available in pool";
//		reg.info.stackOffset = spillPool.back();
//		spillPool.pop_back();
//	}
//	else
//	{
//		// maybe make this smarter to operate off of the type?
//		accumulatedSpillOffset += 8;
//		std::cout << "no spills available in pool, using accumulatedSpillOffset: " << accumulatedSpillOffset;
//		reg.info.stackOffset = accumulatedSpillOffset;
//	}
//
//	// free up the register being spilled
//	if (reg.info.regClass == RegisterClass::GP)
//		freeGPRegister(reg);
//	else if (reg.info.regClass == RegisterClass::FP)
//		freeFPRegister(reg);
//}
