//#pragma once
//
//#include <stdint.h>
//
//enum class RegisterClass
//{
//	None,
//	GP,  // general purpose register
//	FP   // floating point register
//};
//
//// used for register allocation
//struct RegisterTraits
//{
//	uint32_t registerID;
//	bool isVolatile;
//	bool isParamReserved;
//};
//
//// used for codegen, should probably be named to StorageInfo or something
//struct RegisterInfo
//{
//	RegisterClass regClass;
//	uint32_t regID;            // physical register ID
//	uint32_t stackOffset;      // stack offset IF no register was available and we had to spill into the stack
//	bool spilled;
//};