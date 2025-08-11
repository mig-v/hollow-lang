#pragma once

#include "ir/ir_core.h"

#include <fstream>

class IRLogger
{
public:
	IRLogger() = default;
	~IRLogger() = default;
	void log(IRData* data, const std::string& path);
private:
	void logIRFunction(IRFunction* function);
	void logIRInstruction(const TacInstruction& instruction);
	std::string irOperandToString(const IROperand& value);
	std::string irConstToString(const IROperand& value);

	std::ofstream out;
	IRData* data;
};