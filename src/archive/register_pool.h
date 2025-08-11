//#pragma once
//
//#include "archive/register_allocation_core.h"
//
//#include "codegen/target_info.h"
//
//#include <vector>
//#include <unordered_set>
//
//// internal usage only, used for tracking data about registers like if they're volatile, used for parameters, in use, etc
//struct PooledRegister
//{
//	RegisterTraits traits;
//	bool inUse;
//};
//
//class RegisterPool
//{
//public:
//	RegisterPool() = default;
//	~RegisterPool() = default;
//
//	void initialize(TargetInfo* targetInfo);
//	void resetInternalState();
//
//	void allocateRegister(Register& reg);
//	void allocateRegisterWithConstraints(Register& reg, std::unordered_set<uint32_t>& constraints);
//	void freeRegister(Register& reg);
//	void allocateSpill(Register& reg);
//
//	size_t getNextSpillSlot();
//
//	bool registerInUse(Register& reg);
//
//private:
//	void allocateGpRegister(Register& reg);
//	void allocateFpRegister(Register& reg);
//	void allocateGpRegisterWithConstraints(Register& reg, std::unordered_set<uint32_t>& constraints);
//
//	bool allocateVolatileGPRegister(Register& reg);
//	bool allocateNonVolatileGPRegister(Register& reg);
//
//	
//	void freeGPRegister(Register& reg);
//	void freeFPRegister(Register& reg);
//
//
//	TargetInfo* targetInfo;
//	size_t accumulatedSpillOffset;
//
//	// pool of available physical register ID's and spill slots
//	std::vector<PooledRegister> gpRegisterPool;
//	std::vector<PooledRegister> fpRegisterPool;
//	uint32_t gpRegistersAvailable;
//	uint32_t fpRegistersAvailable;
//
//	//std::vector<uint32_t> volatileGPRegPool;
//	//std::vector<uint32_t> nonVolatileGPRegPool;
//	//std::vector<uint32_t> volatileFPRegPool;
//	//std::vector<uint32_t> nonVolatileFPRegPool;
//	std::vector<uint32_t> spillPool;
//};