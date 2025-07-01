#include "vm.h"

#include <iostream>
#include <algorithm>

VM::VM()
{
	this->functionTable = nullptr;
	this->ip = 0;
	this->sp = 0;
	fillOpcodeTable();
}

void VM::fillOpcodeTable()
{
	// fill all spots with NOP before filling in all valid opcodes, to make sure any gaps between Opcodes are filled with NOP
	for (size_t i = 0; i < static_cast<size_t>(Opcode::NUM_OPCODES); i++)
		opcodeTable[i] = &VM::NOP;

	opcodeTable[static_cast<size_t>(Opcode::PUSH_I8)] = &VM::PUSH_I8;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_I16)] = &VM::PUSH_I16;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_I32)] = &VM::PUSH_I32;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_I64)] = &VM::PUSH_I64;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_U8)] = &VM::PUSH_U8;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_U16)] = &VM::PUSH_U16;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_U32)] = &VM::PUSH_U32;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_U64)] = &VM::PUSH_U64;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_F32)] = &VM::PUSH_F32;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_F64)] = &VM::PUSH_F64;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_CHAR)] = &VM::PUSH_CHAR;
	opcodeTable[static_cast<size_t>(Opcode::PUSH_BOOL)] = &VM::PUSH_BOOL;

	opcodeTable[static_cast<size_t>(Opcode::LOAD_I8)] = &VM::LOAD_I8;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_I16)] = &VM::LOAD_I16;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_I32)] = &VM::LOAD_I32;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_I64)] = &VM::LOAD_I64;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_U8)] = &VM::LOAD_U8;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_U16)] = &VM::LOAD_U16;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_U32)] = &VM::LOAD_U32;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_U64)] = &VM::LOAD_U64;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_F32)] = &VM::LOAD_F32;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_F64)] = &VM::LOAD_F64;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_CHAR)] = &VM::LOAD_CHAR;
	opcodeTable[static_cast<size_t>(Opcode::LOAD_BOOL)] = &VM::LOAD_BOOL;

	opcodeTable[static_cast<size_t>(Opcode::STORE_I8)] = &VM::STORE_I8;
	opcodeTable[static_cast<size_t>(Opcode::STORE_I16)] = &VM::STORE_I16;
	opcodeTable[static_cast<size_t>(Opcode::STORE_I32)] = &VM::STORE_I32;
	opcodeTable[static_cast<size_t>(Opcode::STORE_I64)] = &VM::STORE_I64;
	opcodeTable[static_cast<size_t>(Opcode::STORE_U8)] = &VM::STORE_U8;
	opcodeTable[static_cast<size_t>(Opcode::STORE_U16)] = &VM::STORE_U16;
	opcodeTable[static_cast<size_t>(Opcode::STORE_U32)] = &VM::STORE_U32;
	opcodeTable[static_cast<size_t>(Opcode::STORE_U64)] = &VM::STORE_U64;
	opcodeTable[static_cast<size_t>(Opcode::STORE_F32)] = &VM::STORE_F32;
	opcodeTable[static_cast<size_t>(Opcode::STORE_F64)] = &VM::STORE_F64;
	opcodeTable[static_cast<size_t>(Opcode::STORE_CHAR)] = &VM::STORE_CHAR;
	opcodeTable[static_cast<size_t>(Opcode::STORE_BOOL)] = &VM::STORE_BOOL;

	opcodeTable[static_cast<size_t>(Opcode::JMP)] = &VM::JMP;
	opcodeTable[static_cast<size_t>(Opcode::JMP_IF_FALSE)] = &VM::JMP_IF_FALSE;
	opcodeTable[static_cast<size_t>(Opcode::JMP_IF_TRUE)] = &VM::JMP_IF_TRUE;
	opcodeTable[static_cast<size_t>(Opcode::CALL)] = &VM::CALL;
	opcodeTable[static_cast<size_t>(Opcode::RET)] = &VM::RET;
	opcodeTable[static_cast<size_t>(Opcode::DUP)] = &VM::DUP;

	// NOTE: This is a design decision that i think is pretty common, the POP and SWAP instructions are never actually emitted
	// by the bytecode emitter, they're implicitly used by the VM itself. So they're defaulted here to point to the NOP function
	// but the opcodes are never actually emitted so they will never go through the opcode table. The VM will explicitly call 
	// POP or SWAP whenever an instruction needs to
	opcodeTable[static_cast<size_t>(Opcode::POP)] = &VM::NOP;
	opcodeTable[static_cast<size_t>(Opcode::SWAP)] = &VM::NOP;

	opcodeTable[static_cast<size_t>(Opcode::ADD_I8)] = &VM::ADD_I8;
	opcodeTable[static_cast<size_t>(Opcode::ADD_I16)] = &VM::ADD_I16;
	opcodeTable[static_cast<size_t>(Opcode::ADD_I32)] = &VM::ADD_I32;
	opcodeTable[static_cast<size_t>(Opcode::ADD_I64)] = &VM::ADD_I64;
	opcodeTable[static_cast<size_t>(Opcode::ADD_U8)] = &VM::ADD_U8;
	opcodeTable[static_cast<size_t>(Opcode::ADD_U16)] = &VM::ADD_U16;
	opcodeTable[static_cast<size_t>(Opcode::ADD_U32)] = &VM::ADD_U32;
	opcodeTable[static_cast<size_t>(Opcode::ADD_U64)] = &VM::ADD_U64;
	opcodeTable[static_cast<size_t>(Opcode::ADD_F32)] = &VM::ADD_F32;
	opcodeTable[static_cast<size_t>(Opcode::ADD_F64)] = &VM::ADD_F64;

	opcodeTable[static_cast<size_t>(Opcode::SUB_I8)] = &VM::SUB_I8;
	opcodeTable[static_cast<size_t>(Opcode::SUB_I16)] = &VM::SUB_I16;
	opcodeTable[static_cast<size_t>(Opcode::SUB_I32)] = &VM::SUB_I32;
	opcodeTable[static_cast<size_t>(Opcode::SUB_I64)] = &VM::SUB_I64;
	opcodeTable[static_cast<size_t>(Opcode::SUB_U8)] = &VM::SUB_U8;
	opcodeTable[static_cast<size_t>(Opcode::SUB_U16)] = &VM::SUB_U16;
	opcodeTable[static_cast<size_t>(Opcode::SUB_U32)] = &VM::SUB_U32;
	opcodeTable[static_cast<size_t>(Opcode::SUB_U64)] = &VM::SUB_U64;
	opcodeTable[static_cast<size_t>(Opcode::SUB_F32)] = &VM::SUB_F32;
	opcodeTable[static_cast<size_t>(Opcode::SUB_F64)] = &VM::SUB_F64;

	opcodeTable[static_cast<size_t>(Opcode::MUL_I8)] = &VM::MUL_I8;
	opcodeTable[static_cast<size_t>(Opcode::MUL_I16)] = &VM::MUL_I16;
	opcodeTable[static_cast<size_t>(Opcode::MUL_I32)] = &VM::MUL_I32;
	opcodeTable[static_cast<size_t>(Opcode::MUL_I64)] = &VM::MUL_I64;
	opcodeTable[static_cast<size_t>(Opcode::MUL_U8)] = &VM::MUL_U8;
	opcodeTable[static_cast<size_t>(Opcode::MUL_U16)] = &VM::MUL_U16;
	opcodeTable[static_cast<size_t>(Opcode::MUL_U32)] = &VM::MUL_U32;
	opcodeTable[static_cast<size_t>(Opcode::MUL_U64)] = &VM::MUL_U64;
	opcodeTable[static_cast<size_t>(Opcode::MUL_F32)] = &VM::MUL_F32;
	opcodeTable[static_cast<size_t>(Opcode::MUL_F64)] = &VM::MUL_F64;

	opcodeTable[static_cast<size_t>(Opcode::DIV_I8)] = &VM::DIV_I8;
	opcodeTable[static_cast<size_t>(Opcode::DIV_I16)] = &VM::DIV_I16;
	opcodeTable[static_cast<size_t>(Opcode::DIV_I32)] = &VM::DIV_I32;
	opcodeTable[static_cast<size_t>(Opcode::DIV_I64)] = &VM::DIV_I64;
	opcodeTable[static_cast<size_t>(Opcode::DIV_U8)] = &VM::DIV_U8;
	opcodeTable[static_cast<size_t>(Opcode::DIV_U16)] = &VM::DIV_U16;
	opcodeTable[static_cast<size_t>(Opcode::DIV_U32)] = &VM::DIV_U32;
	opcodeTable[static_cast<size_t>(Opcode::DIV_U64)] = &VM::DIV_U64;
	opcodeTable[static_cast<size_t>(Opcode::DIV_F32)] = &VM::DIV_F32;
	opcodeTable[static_cast<size_t>(Opcode::DIV_F64)] = &VM::DIV_F64;

	opcodeTable[static_cast<size_t>(Opcode::MOD_I8)] = &VM::MOD_I8;
	opcodeTable[static_cast<size_t>(Opcode::MOD_I16)] = &VM::MOD_I16;
	opcodeTable[static_cast<size_t>(Opcode::MOD_I32)] = &VM::MOD_I32;
	opcodeTable[static_cast<size_t>(Opcode::MOD_I64)] = &VM::MOD_I64;
	opcodeTable[static_cast<size_t>(Opcode::MOD_U8)] = &VM::MOD_U8;
	opcodeTable[static_cast<size_t>(Opcode::MOD_U16)] = &VM::MOD_U16;
	opcodeTable[static_cast<size_t>(Opcode::MOD_U32)] = &VM::MOD_U32;
	opcodeTable[static_cast<size_t>(Opcode::MOD_U64)] = &VM::MOD_U64;

	opcodeTable[static_cast<size_t>(Opcode::NEG_I8)] = &VM::NEG_I8;
	opcodeTable[static_cast<size_t>(Opcode::NEG_I16)] = &VM::NEG_I16;
	opcodeTable[static_cast<size_t>(Opcode::NEG_I32)] = &VM::NEG_I32;
	opcodeTable[static_cast<size_t>(Opcode::NEG_I64)] = &VM::NEG_I64;
	opcodeTable[static_cast<size_t>(Opcode::NEG_F32)] = &VM::NEG_F32;
	opcodeTable[static_cast<size_t>(Opcode::NEG_F64)] = &VM::NEG_F64;

	opcodeTable[static_cast<size_t>(Opcode::EQ_I8)] = &VM::EQ_I8;
	opcodeTable[static_cast<size_t>(Opcode::EQ_I16)] = &VM::EQ_I16;
	opcodeTable[static_cast<size_t>(Opcode::EQ_I32)] = &VM::EQ_I32;
	opcodeTable[static_cast<size_t>(Opcode::EQ_I64)] = &VM::EQ_I64;
	opcodeTable[static_cast<size_t>(Opcode::EQ_U8)] = &VM::EQ_U8;
	opcodeTable[static_cast<size_t>(Opcode::EQ_U16)] = &VM::EQ_U16;
	opcodeTable[static_cast<size_t>(Opcode::EQ_U32)] = &VM::EQ_U32;
	opcodeTable[static_cast<size_t>(Opcode::EQ_U64)] = &VM::EQ_U64;
	opcodeTable[static_cast<size_t>(Opcode::EQ_F32)] = &VM::EQ_F32;
	opcodeTable[static_cast<size_t>(Opcode::EQ_F64)] = &VM::EQ_F64;
	opcodeTable[static_cast<size_t>(Opcode::EQ_CHAR)] = &VM::EQ_CHAR;
	opcodeTable[static_cast<size_t>(Opcode::EQ_BOOL)] = &VM::EQ_BOOL;

	opcodeTable[static_cast<size_t>(Opcode::NEQ_I8)] = &VM::NEQ_I8;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_I16)] = &VM::NEQ_I16;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_I32)] = &VM::NEQ_I32;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_I64)] = &VM::NEQ_I64;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_U8)] = &VM::NEQ_U8;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_U16)] = &VM::NEQ_U16;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_U32)] = &VM::NEQ_U32;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_U64)] = &VM::NEQ_U64;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_F32)] = &VM::NEQ_F32;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_F64)] = &VM::NEQ_F64;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_CHAR)] = &VM::NEQ_CHAR;
	opcodeTable[static_cast<size_t>(Opcode::NEQ_BOOL)] = &VM::NEQ_BOOL;

	opcodeTable[static_cast<size_t>(Opcode::GT_I8)] = &VM::GT_I8;
	opcodeTable[static_cast<size_t>(Opcode::GT_I16)] = &VM::GT_I16;
	opcodeTable[static_cast<size_t>(Opcode::GT_I32)] = &VM::GT_I32;
	opcodeTable[static_cast<size_t>(Opcode::GT_I64)] = &VM::GT_I64;
	opcodeTable[static_cast<size_t>(Opcode::GT_U8)] = &VM::GT_U8;
	opcodeTable[static_cast<size_t>(Opcode::GT_U16)] = &VM::GT_U16;
	opcodeTable[static_cast<size_t>(Opcode::GT_U32)] = &VM::GT_U32;
	opcodeTable[static_cast<size_t>(Opcode::GT_U64)] = &VM::GT_U64;
	opcodeTable[static_cast<size_t>(Opcode::GT_F32)] = &VM::GT_F32;
	opcodeTable[static_cast<size_t>(Opcode::GT_F64)] = &VM::GT_F64;
	opcodeTable[static_cast<size_t>(Opcode::GT_CHAR)] = &VM::GT_CHAR;

	opcodeTable[static_cast<size_t>(Opcode::GTE_I8)] = &VM::GTE_I8;
	opcodeTable[static_cast<size_t>(Opcode::GTE_I16)] = &VM::GTE_I16;
	opcodeTable[static_cast<size_t>(Opcode::GTE_I32)] = &VM::GTE_I32;
	opcodeTable[static_cast<size_t>(Opcode::GTE_I64)] = &VM::GTE_I64;
	opcodeTable[static_cast<size_t>(Opcode::GTE_U8)] = &VM::GTE_U8;
	opcodeTable[static_cast<size_t>(Opcode::GTE_U16)] = &VM::GTE_U16;
	opcodeTable[static_cast<size_t>(Opcode::GTE_U32)] = &VM::GTE_U32;
	opcodeTable[static_cast<size_t>(Opcode::GTE_U64)] = &VM::GTE_U64;
	opcodeTable[static_cast<size_t>(Opcode::GTE_F32)] = &VM::GTE_F32;
	opcodeTable[static_cast<size_t>(Opcode::GTE_F64)] = &VM::GTE_F64;
	opcodeTable[static_cast<size_t>(Opcode::GTE_CHAR)] = &VM::GTE_CHAR;

	opcodeTable[static_cast<size_t>(Opcode::LT_I8)] = &VM::LT_I8;
	opcodeTable[static_cast<size_t>(Opcode::LT_I16)] = &VM::LT_I16;
	opcodeTable[static_cast<size_t>(Opcode::LT_I32)] = &VM::LT_I32;
	opcodeTable[static_cast<size_t>(Opcode::LT_I64)] = &VM::LT_I64;
	opcodeTable[static_cast<size_t>(Opcode::LT_U8)] = &VM::LT_U8;
	opcodeTable[static_cast<size_t>(Opcode::LT_U16)] = &VM::LT_U16;
	opcodeTable[static_cast<size_t>(Opcode::LT_U32)] = &VM::LT_U32;
	opcodeTable[static_cast<size_t>(Opcode::LT_U64)] = &VM::LT_U64;
	opcodeTable[static_cast<size_t>(Opcode::LT_F32)] = &VM::LT_F32;
	opcodeTable[static_cast<size_t>(Opcode::LT_F64)] = &VM::LT_F64;
	opcodeTable[static_cast<size_t>(Opcode::LT_CHAR)] = &VM::LT_CHAR;

	opcodeTable[static_cast<size_t>(Opcode::LTE_I8)] = &VM::LTE_I8;
	opcodeTable[static_cast<size_t>(Opcode::LTE_I16)] = &VM::LTE_I16;
	opcodeTable[static_cast<size_t>(Opcode::LTE_I32)] = &VM::LTE_I32;
	opcodeTable[static_cast<size_t>(Opcode::LTE_I64)] = &VM::LTE_I64;
	opcodeTable[static_cast<size_t>(Opcode::LTE_U8)] = &VM::LTE_U8;
	opcodeTable[static_cast<size_t>(Opcode::LTE_U16)] = &VM::LTE_U16;
	opcodeTable[static_cast<size_t>(Opcode::LTE_U32)] = &VM::LTE_U32;
	opcodeTable[static_cast<size_t>(Opcode::LTE_U64)] = &VM::LTE_U64;
	opcodeTable[static_cast<size_t>(Opcode::LTE_F32)] = &VM::LTE_F32;
	opcodeTable[static_cast<size_t>(Opcode::LTE_F64)] = &VM::LTE_F64;
	opcodeTable[static_cast<size_t>(Opcode::LTE_CHAR)] = &VM::LTE_CHAR;

	opcodeTable[static_cast<size_t>(Opcode::LOGICAL_AND)] = &VM::LOGICAL_AND;
	opcodeTable[static_cast<size_t>(Opcode::LOGICAL_OR)] = &VM::LOGICAL_OR;
	opcodeTable[static_cast<size_t>(Opcode::LOGICAL_NOT)] = &VM::LOGICAL_NOT;

	opcodeTable[static_cast<size_t>(Opcode::INC_I8)] = &VM::INC_I8;
	opcodeTable[static_cast<size_t>(Opcode::INC_I16)] = &VM::INC_I16;
	opcodeTable[static_cast<size_t>(Opcode::INC_I32)] = &VM::INC_I32;
	opcodeTable[static_cast<size_t>(Opcode::INC_I64)] = &VM::INC_I64;
	opcodeTable[static_cast<size_t>(Opcode::INC_U8)] = &VM::INC_U8;
	opcodeTable[static_cast<size_t>(Opcode::INC_U16)] = &VM::INC_U16;
	opcodeTable[static_cast<size_t>(Opcode::INC_U32)] = &VM::INC_U32;
	opcodeTable[static_cast<size_t>(Opcode::INC_U64)] = &VM::INC_U64;

	opcodeTable[static_cast<size_t>(Opcode::DEC_I8)] = &VM::DEC_I8;
	opcodeTable[static_cast<size_t>(Opcode::DEC_I16)] = &VM::DEC_I16;
	opcodeTable[static_cast<size_t>(Opcode::DEC_I32)] = &VM::DEC_I32;
	opcodeTable[static_cast<size_t>(Opcode::DEC_I64)] = &VM::DEC_I64;
	opcodeTable[static_cast<size_t>(Opcode::DEC_U8)] = &VM::DEC_U8;
	opcodeTable[static_cast<size_t>(Opcode::DEC_U16)] = &VM::DEC_U16;
	opcodeTable[static_cast<size_t>(Opcode::DEC_U32)] = &VM::DEC_U32;
	opcodeTable[static_cast<size_t>(Opcode::DEC_U64)] = &VM::DEC_U64;

	opcodeTable[static_cast<size_t>(Opcode::NOT_I8)] = &VM::NOT_I8;
	opcodeTable[static_cast<size_t>(Opcode::NOT_I16)] = &VM::NOT_I16;
	opcodeTable[static_cast<size_t>(Opcode::NOT_I32)] = &VM::NOT_I32;
	opcodeTable[static_cast<size_t>(Opcode::NOT_I64)] = &VM::NOT_I64;
	opcodeTable[static_cast<size_t>(Opcode::NOT_U8)] = &VM::NOT_U8;
	opcodeTable[static_cast<size_t>(Opcode::NOT_U16)] = &VM::NOT_U16;
	opcodeTable[static_cast<size_t>(Opcode::NOT_U32)] = &VM::NOT_U32;
	opcodeTable[static_cast<size_t>(Opcode::NOT_U64)] = &VM::NOT_U64;

	opcodeTable[static_cast<size_t>(Opcode::AND_I8)] = &VM::AND_I8;
	opcodeTable[static_cast<size_t>(Opcode::AND_I16)] = &VM::AND_I16;
	opcodeTable[static_cast<size_t>(Opcode::AND_I32)] = &VM::AND_I32;
	opcodeTable[static_cast<size_t>(Opcode::AND_I64)] = &VM::AND_I64;
	opcodeTable[static_cast<size_t>(Opcode::AND_U8)] = &VM::AND_U8;
	opcodeTable[static_cast<size_t>(Opcode::AND_U16)] = &VM::AND_U16;
	opcodeTable[static_cast<size_t>(Opcode::AND_U32)] = &VM::AND_U32;
	opcodeTable[static_cast<size_t>(Opcode::AND_U64)] = &VM::AND_U64;

	opcodeTable[static_cast<size_t>(Opcode::OR_I8)] = &VM::OR_I8;
	opcodeTable[static_cast<size_t>(Opcode::OR_I16)] = &VM::OR_I16;
	opcodeTable[static_cast<size_t>(Opcode::OR_I32)] = &VM::OR_I32;
	opcodeTable[static_cast<size_t>(Opcode::OR_I64)] = &VM::OR_I64;
	opcodeTable[static_cast<size_t>(Opcode::OR_U8)] = &VM::OR_U8;
	opcodeTable[static_cast<size_t>(Opcode::OR_U16)] = &VM::OR_U16;
	opcodeTable[static_cast<size_t>(Opcode::OR_U32)] = &VM::OR_U32;
	opcodeTable[static_cast<size_t>(Opcode::OR_U64)] = &VM::OR_U64;

	opcodeTable[static_cast<size_t>(Opcode::XOR_I8)] = &VM::XOR_I8;
	opcodeTable[static_cast<size_t>(Opcode::XOR_I16)] = &VM::XOR_I16;
	opcodeTable[static_cast<size_t>(Opcode::XOR_I32)] = &VM::XOR_I32;
	opcodeTable[static_cast<size_t>(Opcode::XOR_I64)] = &VM::XOR_I64;
	opcodeTable[static_cast<size_t>(Opcode::XOR_U8)] = &VM::XOR_U8;
	opcodeTable[static_cast<size_t>(Opcode::XOR_U16)] = &VM::XOR_U16;
	opcodeTable[static_cast<size_t>(Opcode::XOR_U32)] = &VM::XOR_U32;
	opcodeTable[static_cast<size_t>(Opcode::XOR_U64)] = &VM::XOR_U64;

	opcodeTable[static_cast<size_t>(Opcode::SHL_I8)] = &VM::SHL_I8;
	opcodeTable[static_cast<size_t>(Opcode::SHL_I16)] = &VM::SHL_I16;
	opcodeTable[static_cast<size_t>(Opcode::SHL_I32)] = &VM::SHL_I32;
	opcodeTable[static_cast<size_t>(Opcode::SHL_I64)] = &VM::SHL_I64;
	opcodeTable[static_cast<size_t>(Opcode::SHL_U8)] = &VM::SHL_U8;
	opcodeTable[static_cast<size_t>(Opcode::SHL_U16)] = &VM::SHL_U16;
	opcodeTable[static_cast<size_t>(Opcode::SHL_U32)] = &VM::SHL_U32;
	opcodeTable[static_cast<size_t>(Opcode::SHL_U64)] = &VM::SHL_U64;

	opcodeTable[static_cast<size_t>(Opcode::SHR_I8)] = &VM::SHR_I8;
	opcodeTable[static_cast<size_t>(Opcode::SHR_I16)] = &VM::SHR_I16;
	opcodeTable[static_cast<size_t>(Opcode::SHR_I32)] = &VM::SHR_I32;
	opcodeTable[static_cast<size_t>(Opcode::SHR_I64)] = &VM::SHR_I64;
	opcodeTable[static_cast<size_t>(Opcode::SHR_U8)] = &VM::SHR_U8;
	opcodeTable[static_cast<size_t>(Opcode::SHR_U16)] = &VM::SHR_U16;
	opcodeTable[static_cast<size_t>(Opcode::SHR_U32)] = &VM::SHR_U32;
	opcodeTable[static_cast<size_t>(Opcode::SHR_U64)] = &VM::SHR_U64;
}

uint8_t VM::read8()
{
	return bytecode[ip++];
}

uint16_t VM::read16()
{
	uint16_t val = static_cast<uint16_t>(bytecode[ip])
		| static_cast<uint16_t>(bytecode[ip + 1]) << 8;
	ip += 2;
	return val;
}

uint32_t VM::read32()
{
	uint32_t val = static_cast<uint32_t>(bytecode[ip])
		| static_cast<uint32_t>(bytecode[ip + 1]) << 8
		| static_cast<uint32_t>(bytecode[ip + 2]) << 16
		| static_cast<uint32_t>(bytecode[ip + 3]) << 24;

	ip += 4;
	return val;
}

uint64_t VM::read64()
{
	uint64_t val = static_cast<uint64_t>(bytecode[ip])
		| static_cast<uint64_t>(bytecode[ip + 1]) << 8
		| static_cast<uint64_t>(bytecode[ip + 2]) << 16
		| static_cast<uint64_t>(bytecode[ip + 3]) << 24
		| static_cast<uint64_t>(bytecode[ip + 4]) << 32
		| static_cast<uint64_t>(bytecode[ip + 5]) << 40
		| static_cast<uint64_t>(bytecode[ip + 6]) << 48
		| static_cast<uint64_t>(bytecode[ip + 7]) << 56;

	ip += 8;
	return val;
}

void VM::execute(std::vector<uint8_t>* code, VMFunctionTable* functionTable, int globalVarCount)
{
	this->bytecode = std::move(*code);
	this->functionTable = functionTable;

	// advance stack pointer past the global var count so we don't override global vars when pushing / popping
	sp = globalVarCount;

	// push global stack frame
	pushStackFrame(0, 0, sp);

	// if there's a huge amount of global vars, resize the stack accordingly, otherwise start at INITIAL_STACK_SIZE
	this->stack.resize(std::max(INITIAL_STACK_SIZE, static_cast<size_t>(globalVarCount)));

	std::cout << "\n===== VM Execution Start =====\n";
	std::cout << "Global var count: " << globalVarCount << " sp initialized to " << sp << std::endl;
	debugViewState();
	while (ip < bytecode.size())
	{
		Opcode opcode = static_cast<Opcode>(read16());         // fetch opcode

		if (opcode == Opcode::HALT)                            // stop program execution if we read a HALT instruction
			break;

		(this->*(opcodeTable[static_cast<size_t>(opcode)]))(); // call corresponding function in opcode table
		debugViewState();                                      // view VM state after every opcode (SUPER temporary)
	}
}

void VM::debugViewState()
{
	std::cout << "[VM State] ip = " << ip << " sp = " << sp << " stack view = [";

	size_t count = 0;
	for (size_t i = callStack.back().basePointer; count < 10 && i < sp; i++)
	{
		std::cout << stack[i] << ", ";
		count++;
	}

	std::cout << "]\n";

}

uint64_t VM::loadLocal(uint16_t slot)
{
	std::cout << "[Loading local] slot: " << slot << " value: " << stack[callStack.back().basePointer + slot] << std::endl;
	return stack[callStack.back().basePointer + slot];
}

void VM::storeLocal(uint16_t slot, uint64_t value)
{
	std::cout << "[Storing local] slot: " << slot << " value: " << value << std::endl;
	stack[callStack.back().basePointer + slot] = value;
}

void VM::pushStackFrame(size_t basePointer, size_t returnAddress, size_t retSP)
{
	StackFrame frame;
	frame.basePointer = basePointer;
	frame.retAddress = returnAddress;
	frame.retSP = retSP;
	callStack.push_back(frame);
	std::cout << "Pushing stack frame with base = " << basePointer << " return address = " << returnAddress << " retSP = " << retSP << std::endl;
}

void VM::popStackFrame()
{

}

void VM::NOP()
{
	// no op
}

uint64_t VM::POP()
{
	//uint64_t ret = stack.back();
	//stack.pop_back();
	//return ret;
	return stack[--sp];
}

uint64_t VM::PEEK()
{
	return stack[sp - 1];
}

void VM::PUSH(uint64_t value)
{
	//stack.push_back(value);
	stack[sp++] = value;
}

void VM::SWAP()
{

}

void VM::PUSH_I8()
{
	PUSH(static_cast<uint64_t>(read8()));
}

void VM::PUSH_I16()
{
	PUSH(static_cast<uint64_t>(read16()));
}

void VM::PUSH_I32()
{
	PUSH(static_cast<uint64_t>(read32()));
}

void VM::PUSH_I64()
{
	PUSH(static_cast<uint64_t>(read64()));
}

void VM::PUSH_U8()
{
	PUSH(static_cast<uint64_t>(read8()));
}

void VM::PUSH_U16()
{
	PUSH(static_cast<uint64_t>(read16()));
}

void VM::PUSH_U32()
{
	PUSH(static_cast<uint64_t>(read32()));
}

void VM::PUSH_U64()
{
	PUSH(read64());
}

void VM::PUSH_F32()
{

}

void VM::PUSH_F64()
{

}

void VM::PUSH_CHAR()
{
	PUSH(static_cast<uint64_t>(read8()));
}

void VM::PUSH_BOOL()
{
	PUSH(static_cast<uint64_t>(read8()));
}

void VM::LOAD_I8()
{
	// get slot we're storing in as the direct operand within the bytecode
	uint16_t slot = read16();

	// get the local value
	int8_t value = static_cast<int8_t>(loadLocal(slot));
	
	// push the local value onto the stack. Logic for all loads is essentially the same
	PUSH(static_cast<uint64_t>(value));;
}

void VM::LOAD_I16()
{
	uint16_t slot = read16();
	int16_t value = static_cast<int16_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_I32()
{
	uint16_t slot = read16();
	int32_t value = static_cast<int32_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_I64()
{
	uint16_t slot = read16();
	int64_t value = static_cast<int64_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_U8()
{
	uint16_t slot = read16();
	uint8_t value = static_cast<uint8_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_U16()
{
	uint16_t slot = read16();
	uint16_t value = static_cast<uint16_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_U32()
{
	uint16_t slot = read16();
	uint32_t value = static_cast<uint32_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_U64()
{
	uint16_t slot = read16();
	uint64_t value = static_cast<uint64_t>(loadLocal(slot));
	PUSH(value);
}

void VM::LOAD_F32()
{

}

void VM::LOAD_F64()
{

}

void VM::LOAD_CHAR()
{
	uint16_t slot = read16();
	uint8_t value = static_cast<uint8_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::LOAD_BOOL()
{
	uint16_t slot = read16();
	uint8_t value = static_cast<uint8_t>(loadLocal(slot));
	PUSH(static_cast<uint64_t>(value));
}

void VM::STORE_I8()
{
	// POP the value we're storing
	int8_t value = static_cast<int8_t>(POP());

	// get the slot we're storing the value in
	uint16_t slot = read16();

	// store the local value at 'slot' on the stack, all logic for STORE instructions are essentially the same
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_I16()
{
	int16_t value = static_cast<int16_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_I32()
{
	int32_t value = static_cast<int32_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_I64() 
{
	int64_t value = static_cast<int64_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_U8()
{
	uint8_t value = static_cast<uint8_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_U16()
{
	uint16_t value = static_cast<uint16_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_U32()
{
	uint32_t value = static_cast<uint32_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_U64()
{
	uint64_t value = POP();
	uint16_t slot = read16();
	storeLocal(slot, value);
}

void VM::STORE_F32()
{

}

void VM::STORE_F64()
{

}

void VM::STORE_CHAR()
{
	uint8_t value = static_cast<uint8_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::STORE_BOOL()
{
	uint8_t value = static_cast<uint8_t>(POP());
	uint16_t slot = read16();
	storeLocal(slot, static_cast<uint64_t>(value));
}

void VM::JMP()
{
	uint16_t jumpAddress = read16();
	ip = jumpAddress;
}

void VM::JMP_IF_FALSE()
{
	uint8_t condition = static_cast<uint8_t>(POP());
	uint16_t jumpAddress = read16(); // always read jump address, we need to advance ip even if we dont execute the jump

	if (!condition)
		ip = jumpAddress;
}

void VM::JMP_IF_TRUE()
{
	uint8_t condition = static_cast<uint8_t>(POP());
	uint16_t jumpAddress = read16(); 

	if (condition)
		ip = jumpAddress;
}

void VM::CALL()
{
	// CALL instruction is formatted in the bytecode as:
	// 2 bytes   2 bytes	   1 byte
	//   CALL   FUNC_INDEX   ARG_COUNT
	uint16_t funcIndex = read16();
	uint8_t argCount = read8();
	
	// [global, sp] arg count 1 --> [global, arg1, sp]
	// ip points to the instruction after CALL after read16 and read8, so we save it as the return address
	// the frames base pointer needs to point to the start of the functions parameters. At this point the parameters are already
	// pushed onto the stack, so the base is just whatever sp is - the number of args
	pushStackFrame(sp - static_cast<size_t>(argCount), ip, sp - static_cast<size_t>(argCount));

	// set the current sp to begin after the function parameters so we don't override them with PUSH / POP calls
	sp = callStack.back().basePointer + static_cast<size_t>(argCount);

	// jump to the functions start address and push the new stack frame onto the call stack
	ip = functionTable->getFunctionByIndex(funcIndex).startAddress;
}

void VM::RET()
{
	StackFrame frame = callStack.back();  // get the function frame
	uint64_t returnValue = POP();         // get the return value left on top of stack BEFORE restoring sp
	ip = frame.retAddress;                // restore ip
	sp = frame.retSP;                     // restore sp
	callStack.pop_back();                 // pop the function frame, now we're back in the callees frame
	PUSH(returnValue);                    // push the return value so the callee can retrieve it if it wants to
}

void VM::DUP()
{
	uint64_t top = PEEK();
	PUSH(top);
}

void VM::ADD_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());
	int8_t result = lhs + rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::ADD_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());
	int16_t result = lhs + rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::ADD_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());
	int32_t result = lhs + rhs;
	std::cout << "adding " << lhs << " + " << rhs << " = " << result << std::endl;
	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::ADD_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());
	int64_t result = lhs + rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::ADD_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());
	uint8_t result = lhs + rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::ADD_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());
	uint16_t result = lhs + rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::ADD_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());
	uint32_t result = lhs + rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::ADD_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());
	uint64_t result = lhs + rhs;

	PUSH(result);
}

void VM::ADD_F32()
{

}

void VM::ADD_F64()
{

}

void VM::SUB_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());
	int8_t result = lhs - rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::SUB_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());
	int16_t result = lhs - rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::SUB_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());
	int32_t result = lhs - rhs;
	std::cout << "subtracting " << lhs << " - " << rhs << " = " << result << std::endl;
	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::SUB_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());
	int64_t result = lhs - rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::SUB_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());
	uint8_t result = lhs - rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::SUB_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());
	uint16_t result = lhs - rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::SUB_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());
	uint32_t result = lhs - rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::SUB_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());
	uint64_t result = lhs - rhs;

	PUSH(result);
}

void VM::SUB_F32()
{

}

void VM::SUB_F64()
{

}

void VM::MUL_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());
	int8_t result = lhs * rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::MUL_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());
	int16_t result = lhs * rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::MUL_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());
	int32_t result = lhs * rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::MUL_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());
	int64_t result = lhs * rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MUL_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());
	uint8_t result = lhs * rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MUL_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());
	uint16_t result = lhs * rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MUL_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());
	uint32_t result = lhs * rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MUL_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());
	uint64_t result = lhs * rhs;

	PUSH(result);
}

void VM::MUL_F32()
{

}

void VM::MUL_F64()
{

}

void VM::DIV_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());
	int8_t result = lhs / rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DIV_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());
	int16_t result = lhs / rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DIV_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());
	int32_t result = lhs / rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DIV_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());
	int64_t result = lhs / rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::DIV_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());
	uint8_t result = lhs / rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::DIV_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());
	uint16_t result = lhs / rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::DIV_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());
	uint32_t result = lhs / rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::DIV_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());
	uint64_t result = lhs / rhs;

	PUSH(result);
}

void VM::DIV_F32()
{

}

void VM::DIV_F64()
{

}

void VM::MOD_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());
	int8_t result = lhs % rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());
	int16_t result = lhs % rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());
	int32_t result = lhs % rhs;

	int64_t signExtended = static_cast<int64_t>(result);
	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());
	int64_t result = lhs % rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());
	uint8_t result = lhs % rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());
	uint16_t result = lhs % rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());
	uint32_t result = lhs % rhs;

	PUSH(static_cast<uint64_t>(result));
}

void VM::MOD_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());
	uint64_t result = lhs % rhs;

	PUSH(result);
}

void VM::NEG_I8()
{
	int8_t val = static_cast<int8_t>(POP());
	val = -val;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::NEG_I16()
{
	int16_t val = static_cast<int16_t>(POP());
	val = -val;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::NEG_I32()
{
	int32_t val = static_cast<int32_t>(POP());
	val = -val;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::NEG_I64()
{
	int64_t val = static_cast<int32_t>(POP());
	val = -val;

	PUSH(static_cast<uint64_t>(val));
}

void VM::NEG_F32()
{

}

void VM::NEG_F64()
{

}

void VM::EQ_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_F32()
{

}

void VM::EQ_F64()
{

}

void VM::EQ_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::EQ_BOOL()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs == rhs));
}

void VM::NEQ_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(lhs != rhs);
}

void VM::NEQ_F32()
{

}

void VM::NEQ_F64()
{

}

void VM::NEQ_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::NEQ_BOOL()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs != rhs));
}

void VM::GT_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GT_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(lhs > rhs);
}

void VM::GT_F32()
{

}

void VM::GT_F64()
{

}

void VM::GT_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs > rhs));
}

void VM::GTE_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::GTE_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(lhs >= rhs);
}

void VM::GTE_F32()
{

}

void VM::GTE_F64()
{

}

void VM::GTE_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs >= rhs));
}

void VM::LT_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LT_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(lhs < rhs);
}

void VM::LT_F32()
{

}

void VM::LT_F64()
{

}

void VM::LT_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs < rhs));
}

void VM::LTE_I8()
{
	int8_t rhs = static_cast<int8_t>(POP());
	int8_t lhs = static_cast<int8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_I16()
{
	int16_t rhs = static_cast<int16_t>(POP());
	int16_t lhs = static_cast<int16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_I32()
{
	int32_t rhs = static_cast<int32_t>(POP());
	int32_t lhs = static_cast<int32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_I64()
{
	int64_t rhs = static_cast<int64_t>(POP());
	int64_t lhs = static_cast<int64_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_U8()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_U16()
{
	uint16_t rhs = static_cast<uint16_t>(POP());
	uint16_t lhs = static_cast<uint16_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_U32()
{
	uint32_t rhs = static_cast<uint32_t>(POP());
	uint32_t lhs = static_cast<uint32_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LTE_U64()
{
	uint64_t rhs = static_cast<uint64_t>(POP());
	uint64_t lhs = static_cast<uint64_t>(POP());

	PUSH(lhs <= rhs);
}

void VM::LTE_F32()
{

}

void VM::LTE_F64()
{

}

void VM::LTE_CHAR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs <= rhs));
}

void VM::LOGICAL_AND()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs && rhs));
}

void VM::LOGICAL_OR()
{
	uint8_t rhs = static_cast<uint8_t>(POP());
	uint8_t lhs = static_cast<uint8_t>(POP());

	PUSH(static_cast<uint64_t>(lhs || rhs));
}

void VM::LOGICAL_NOT()
{
	uint8_t val = static_cast<uint8_t>(POP());
	val = !val;
	PUSH(static_cast<uint64_t>(val));
}

void VM::INC_I8()
{
	int8_t val = static_cast<int8_t>(POP());
	val++;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::INC_I16()
{
	int16_t val = static_cast<int16_t>(POP());
	val++;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::INC_I32()
{
	int32_t val = static_cast<int32_t>(POP());
	val++;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::INC_I64()
{
	int64_t val = static_cast<int64_t>(POP());
	val++;

	PUSH(static_cast<uint64_t>(val));
}

void VM::INC_U8()
{
	uint8_t val = static_cast<uint8_t>(POP());
	val++;

	PUSH(static_cast<uint64_t>(val));
}

void VM::INC_U16()
{
	uint16_t val = static_cast<uint16_t>(POP());
	val++;

	PUSH(static_cast<uint64_t>(val));
}

void VM::INC_U32()
{
	uint32_t val = static_cast<uint32_t>(POP());
	val++;

	PUSH(static_cast<uint64_t>(val));
}

void VM::INC_U64()
{
	uint64_t val = static_cast<uint64_t>(POP());
	val++;

	PUSH(val);
}

void VM::DEC_I8()
{
	int8_t val = static_cast<int8_t>(POP());
	val--;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DEC_I16()
{
	int16_t val = static_cast<int16_t>(POP());
	val--;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DEC_I32()
{
	int32_t val = static_cast<int32_t>(POP());
	val--;

	int64_t signExtended = static_cast<int64_t>(val);
	PUSH(static_cast<uint64_t>(signExtended));
}

void VM::DEC_I64()
{
	int64_t val = static_cast<int64_t>(POP());
	val--;

	PUSH(static_cast<uint64_t>(val));
}

void VM::DEC_U8()
{
	uint8_t val = static_cast<uint8_t>(POP());
	val--;

	PUSH(static_cast<uint64_t>(val));
}

void VM::DEC_U16()
{
	uint16_t val = static_cast<uint16_t>(POP());
	val--;

	PUSH(static_cast<uint64_t>(val));
}

void VM::DEC_U32()
{
	uint32_t val = static_cast<uint32_t>(POP());
	val--;

	PUSH(static_cast<uint64_t>(val));
}

void VM::DEC_U64()
{
	uint64_t val = static_cast<uint64_t>(POP());
	val--;

	PUSH(val);
}


void VM::NOT_I8()
{

}

void VM::NOT_I16()
{

}

void VM::NOT_I32()
{

}

void VM::NOT_I64()
{

}

void VM::NOT_U8()
{

}

void VM::NOT_U16()
{

}

void VM::NOT_U32()
{

}

void VM::NOT_U64()
{

}

void VM::AND_I8()
{

}

void VM::AND_I16()
{

}

void VM::AND_I32()
{

}

void VM::AND_I64()
{

}

void VM::AND_U8()
{

}

void VM::AND_U16()
{

}

void VM::AND_U32()
{

}

void VM::AND_U64()
{

}

void VM::OR_I8()
{

}

void VM::OR_I16()
{

}

void VM::OR_I32()
{

}

void VM::OR_I64()
{

}

void VM::OR_U8()
{

}

void VM::OR_U16()
{

}

void VM::OR_U32()
{

}

void VM::OR_U64()
{

}

void VM::XOR_I8()
{

}

void VM::XOR_I16()
{

}

void VM::XOR_I32()
{

}

void VM::XOR_I64()
{

}

void VM::XOR_U8()
{

}

void VM::XOR_U16()
{

}

void VM::XOR_U32()
{

}

void VM::XOR_U64()
{

}

void VM::SHL_I8()
{

}

void VM::SHL_I16()
{

}

void VM::SHL_I32()
{

}

void VM::SHL_I64()
{

}

void VM::SHL_U8()
{

}

void VM::SHL_U16()
{

}

void VM::SHL_U32()
{

}

void VM::SHL_U64()
{

}

void VM::SHR_I8()
{

}

void VM::SHR_I16()
{

}

void VM::SHR_I32()
{

}

void VM::SHR_I64()
{

}

void VM::SHR_U8()
{

}

void VM::SHR_U16()
{

}

void VM::SHR_U32()
{

}

void VM::SHR_U64()
{

}