#include "opcode_data.h"

OpcodeData::OpcodeData()
{
	// generated via scripts/opcode_data_generator.py
	opcodeData[static_cast<size_t>(Opcode::PUSH_I8)] = { 1, { OperandData{1, OperandType::I8}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_I16)] = { 1, { OperandData{2, OperandType::I16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_I32)] = { 1, { OperandData{4, OperandType::I32}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_I64)] = { 1, { OperandData{8, OperandType::I64}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_U8)] = { 1, { OperandData{1, OperandType::U8}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_U16)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_U32)] = { 1, { OperandData{4, OperandType::U32}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_U64)] = { 1, { OperandData{8, OperandType::U64}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_F32)] = { 1, { OperandData{4, OperandType::F32}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_F64)] = { 1, { OperandData{8, OperandType::F64}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_CHAR)] = { 1, { OperandData{1, OperandType::Char}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::PUSH_BOOL)] = { 1, { OperandData{1, OperandType::Bool}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::LOAD_I8)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_I16)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_I32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_I64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_U8)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_U16)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_U32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_U64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_F32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_F64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_CHAR)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOAD_BOOL)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::STORE_I8)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_I16)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_I32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_I64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_U8)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_U16)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_U32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_U64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_F32)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_F64)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_CHAR)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::STORE_BOOL)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::JMP)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::JMP_IF_FALSE)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::JMP_IF_TRUE)] = { 1, { OperandData{2, OperandType::U16}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::CALL)] = { 2, { OperandData{2, OperandType::U16}, OperandData{1, OperandType::U8}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::RET)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::POP)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DUP)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SWAP)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::ADD_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::ADD_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::SUB_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SUB_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::MUL_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MUL_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::DIV_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DIV_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::MOD_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::MOD_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::NEG_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEG_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEG_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEG_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEG_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEG_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::EQ_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::EQ_BOOL)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::NEQ_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NEQ_BOOL)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::GT_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GT_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::GTE_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::GTE_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::LT_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LT_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::LTE_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_F32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_F64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LTE_CHAR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::LOGICAL_AND)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOGICAL_OR)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::LOGICAL_NOT)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::INC)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::DEC)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::NOT_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::NOT_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::AND_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::AND_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::OR_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::OR_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::XOR_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::XOR_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::SHL_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHL_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };

	opcodeData[static_cast<size_t>(Opcode::SHR_I8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_I16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_I32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_I64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_U8)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_U16)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_U32)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
	opcodeData[static_cast<size_t>(Opcode::SHR_U64)] = { 0, { OperandData{0, OperandType::None}, OperandData{0, OperandType::None}, OperandData{0, OperandType::None}} };
}