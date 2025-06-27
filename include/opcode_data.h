#pragma once

#include "opcodes.h"

#include <array>

constexpr inline size_t MAX_OPERANDS = 3;

enum class OperandType : uint8_t
{
	None,
	U8,
	U16,
	U32,
	U64,
	I8,
	I16,
	I32,
	I64,
	F32,
	F64,
	Char,
	Bool
};

struct OperandData
{
	uint8_t size;        // size of operand in bytes
	OperandType type;    // type of operand
};

struct OpcodeFormat
{
	uint8_t operandCount;							// number of operands for the opcode
	std::array<OperandData, MAX_OPERANDS> operandData;	// data for each operand the opcode has
};

class OpcodeData
{
public:
	OpcodeData();
	inline const OpcodeFormat& getFormat(Opcode opcode) const { return opcodeData[static_cast<size_t>(opcode)]; }
private:
	std::array<OpcodeFormat, static_cast<size_t>(Opcode::NUM_OPCODES)> opcodeData;
};