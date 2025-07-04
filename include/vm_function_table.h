#pragma once

#include <stdint.h>
#include <string>
#include <vector>


struct VMFunctionEntry
{
	std::string name;
	uint16_t funcIndex;
	uint8_t argCount;
	size_t startAddress;
	uint16_t localsCount;
};

class VMFunctionTable
{
public:
	VMFunctionTable();
	void addFunction(const std::string& name, uint8_t argCount, size_t startAddress, uint16_t localsCount);
	VMFunctionEntry* lookupFunctionByName(const std::string& name);
	VMFunctionEntry& getFunctionByIndex(uint16_t index);

	void dumpTable();

private:
	std::vector<VMFunctionEntry> functionRegistry;
	uint16_t nextIndex;
};