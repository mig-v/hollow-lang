#include "vm_function_table.h"

#include <iostream>
#include <iomanip>

VMFunctionTable::VMFunctionTable()
{
	this->nextIndex = 0;
}

void VMFunctionTable::addFunction(const std::string& name, uint8_t argCount, size_t startAddress)
{
	functionRegistry.emplace_back(VMFunctionEntry{ name, nextIndex++, argCount, startAddress });
}

VMFunctionEntry* VMFunctionTable::lookupFunctionByName(const std::string& name)
{
	for (VMFunctionEntry& function : functionRegistry)
	{
		if (function.name == name)
			return &function;
	}
}

VMFunctionEntry& VMFunctionTable::getFunctionByIndex(uint16_t index)
{
	return functionRegistry[index];
}

void VMFunctionTable::dumpTable()
{
	std::cout << "===== Bytecode Emitter Function Table =====\n";
	for (const VMFunctionEntry& function : functionRegistry)
	{
		std::cout
			<< "\"" << function.name << "\""
			<< " arg count = " << static_cast<uint32_t>(function.argCount)
			<< " func index = " << function.funcIndex
			<< " start address = " << std::setw(4) << std::setfill('0') << function.startAddress
			<< std::endl;
	}
}