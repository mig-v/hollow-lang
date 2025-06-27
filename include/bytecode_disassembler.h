#pragma once

#include "opcodes.h"
#include "opcode_data.h"

#include <vector>

class BytecodeDisassembler
{
public:
	BytecodeDisassembler();
	void setBytecode(const std::vector<uint8_t>* bytecode);
	void disassemble();

private:
	uint8_t  read8();
	uint16_t read16();
	uint32_t read32();
	uint64_t read64();

	float  read32AsFloat();
	double read64AsDouble();

	const char* opcodeToStr(Opcode opcode);

	void printOperands(Opcode opcode);

	OpcodeData opcodeData;
	size_t ip;
	const std::vector<uint8_t>* bytecode;
};