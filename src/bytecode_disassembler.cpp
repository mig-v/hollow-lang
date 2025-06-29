#include "bytecode_disassembler.h"

#include <unordered_map>
#include <iostream>
#include <iomanip>

BytecodeDisassembler::BytecodeDisassembler()
{
    this->ip = 0;
    this->bytecode = nullptr;
}

void BytecodeDisassembler::setBytecode(const std::vector<uint8_t>* bytecode)
{
    this->bytecode = bytecode;
}

void BytecodeDisassembler::disassemble()
{
    if (!bytecode)
    {
        std::cout << "no valid bytecode for disassembler\n";
        return;
    }
    
    std::cout << "\n===== Bytecode Disassembly =====\n";
    while (ip < bytecode->size())
    {
        Opcode opcode = static_cast<Opcode>(read16());  // fetch opcode
        std::cout << std::setw(4)  << std::setfill('0') // print opcode
                  << ip - 2 << " " << opcodeToStr(opcode) << " "; 
        
        printOperands(opcode);                          // print opcode's operands
    }
}

void BytecodeDisassembler::printOperands(Opcode opcode)
{
    const OpcodeFormat& format = opcodeData.getFormat(opcode);

    for (size_t i = 0; i < format.operandCount; i++)
    {
        switch (format.operandData[i].size)
        {
            case 1: std::cout << static_cast<uint32_t>(read8()) << " "; break;
            case 2: std::cout << read16() << " "; break;
                // instead of calling read32 and read64 directly, need to check if operand type is F32 or F64, if it is, call another function
                // like read32AsFloat, and read64AsDouble
            case 4:
            {
                if (format.operandData[i].type == OperandType::F32 || format.operandData[i].type == OperandType::F64)
                    std::cout << read32AsFloat() << " ";
                else
                    std::cout << read32() << " ";

                break;
            }
            case 8:
            {
                if (format.operandData[i].type == OperandType::F32 || format.operandData[i].type == OperandType::F64)
                    std::cout << read64AsDouble() << " ";
                else
                    std::cout << read64() << " ";

                break;
            }
        }
    }

    std::cout << std::endl;
}

uint8_t BytecodeDisassembler::read8()
{
    return (*bytecode)[ip++];
}

uint16_t BytecodeDisassembler::read16()
{
    uint16_t val = static_cast<uint16_t>((*bytecode)[ip]) 
                 | static_cast<uint16_t>((*bytecode)[ip + 1]) << 8;
    ip += 2;
    return val;
}

uint32_t BytecodeDisassembler::read32()
{
    uint32_t val = static_cast<uint32_t>((*bytecode)[ip])
        | static_cast<uint32_t>((*bytecode)[ip + 1]) << 8
        | static_cast<uint32_t>((*bytecode)[ip + 2]) << 16
        | static_cast<uint32_t>((*bytecode)[ip + 3]) << 24;
    
    ip += 4;
    return val;
}

uint64_t BytecodeDisassembler::read64()
{
    uint64_t val = static_cast<uint64_t>((*bytecode)[ip])
        | static_cast<uint64_t>((*bytecode)[ip + 1]) << 8
        | static_cast<uint64_t>((*bytecode)[ip + 2]) << 16
        | static_cast<uint64_t>((*bytecode)[ip + 3]) << 24
        | static_cast<uint64_t>((*bytecode)[ip + 4]) << 32
        | static_cast<uint64_t>((*bytecode)[ip + 5]) << 40
        | static_cast<uint64_t>((*bytecode)[ip + 6]) << 48
        | static_cast<uint64_t>((*bytecode)[ip + 7]) << 56;

    ip += 8;
    return val;
}

float BytecodeDisassembler::read32AsFloat()
{
    uint32_t bits = read32();
    float val;
    std::memcpy(&val, &bits, sizeof(val));
    return val;
}

double BytecodeDisassembler::read64AsDouble()
{
    uint64_t bits = read64();
    double val;
    std::memcpy(&val, &bits, sizeof(val));
    return val;
}

const char* BytecodeDisassembler::opcodeToStr(Opcode opcode)
{
    static const std::unordered_map<Opcode, const char*> opcodeLookup = 
    {
        {Opcode::PUSH_I8, "PUSH_I8"},
        {Opcode::PUSH_I16, "PUSH_I16"},
        {Opcode::PUSH_I32, "PUSH_I32"},
        {Opcode::PUSH_I64, "PUSH_I64"},
        {Opcode::PUSH_U8, "PUSH_U8"},
        {Opcode::PUSH_U16, "PUSH_U16"},
        {Opcode::PUSH_U32, "PUSH_U32"},
        {Opcode::PUSH_U64, "PUSH_U64"},
        {Opcode::PUSH_F32, "PUSH_F32"},
        {Opcode::PUSH_F64, "PUSH_F64"},
        {Opcode::PUSH_CHAR, "PUSH_CHAR"},
        {Opcode::PUSH_BOOL, "PUSH_BOOL"},

        {Opcode::LOAD_I8, "LOAD_I8"},
        {Opcode::LOAD_I16, "LOAD_I16"},
        {Opcode::LOAD_I32, "LOAD_I32"},
        {Opcode::LOAD_I64, "LOAD_I64"},
        {Opcode::LOAD_U8, "LOAD_U8"},
        {Opcode::LOAD_U16, "LOAD_U16"},
        {Opcode::LOAD_U32, "LOAD_U32"},
        {Opcode::LOAD_U64, "LOAD_U64"},
        {Opcode::LOAD_F32, "LOAD_F32"},
        {Opcode::LOAD_F64, "LOAD_F64"},
        {Opcode::LOAD_CHAR, "LOAD_CHAR"},
        {Opcode::LOAD_BOOL, "LOAD_BOOL"},

        {Opcode::STORE_I8, "STORE_I8"},
        {Opcode::STORE_I16, "STORE_I16"},
        {Opcode::STORE_I32, "STORE_I32"},
        {Opcode::STORE_I64, "STORE_I64"},
        {Opcode::STORE_U8, "STORE_U8"},
        {Opcode::STORE_U16, "STORE_U16"},
        {Opcode::STORE_U32, "STORE_U32"},
        {Opcode::STORE_U64, "STORE_U64"},
        {Opcode::STORE_F32, "STORE_F32"},
        {Opcode::STORE_F64, "STORE_F64"},
        {Opcode::STORE_CHAR, "STORE_CHAR"},
        {Opcode::STORE_BOOL, "STORE_BOOL"},

        {Opcode::JMP, "JMP"},
        {Opcode::JMP_IF_FALSE, "JMP_IF_FALSE"},
        {Opcode::JMP_IF_TRUE, "JMP_IF_TRUE"},
        {Opcode::CALL, "CALL"},
        {Opcode::RET, "RET"},
        {Opcode::POP, "POP"},
        {Opcode::DUP, "DUP"},
        {Opcode::SWAP, "SWAP"},
        {Opcode::HALT, "HALT"},

        {Opcode::ADD_I8, "ADD_I8"},
        {Opcode::ADD_I16, "ADD_I16"},
        {Opcode::ADD_I32, "ADD_I32"},
        {Opcode::ADD_I64, "ADD_I64"},
        {Opcode::ADD_U8, "ADD_U8"},
        {Opcode::ADD_U16, "ADD_U16"},
        {Opcode::ADD_U32, "ADD_U32"},
        {Opcode::ADD_U64, "ADD_U64"},
        {Opcode::ADD_F32, "ADD_F32"},
        {Opcode::ADD_F64, "ADD_F64"},

        {Opcode::SUB_I8, "SUB_I8"},
        {Opcode::SUB_I16, "SUB_I16"},
        {Opcode::SUB_I32, "SUB_I32"},
        {Opcode::SUB_I64, "SUB_I64"},
        {Opcode::SUB_U8, "SUB_U8"},
        {Opcode::SUB_U16, "SUB_U16"},
        {Opcode::SUB_U32, "SUB_U32"},
        {Opcode::SUB_U64, "SUB_U64"},
        {Opcode::SUB_F32, "SUB_F32"},
        {Opcode::SUB_F64, "SUB_F64"},

        {Opcode::MUL_I8, "MUL_I8"},
        {Opcode::MUL_I16, "MUL_I16"},
        {Opcode::MUL_I32, "MUL_I32"},
        {Opcode::MUL_I64, "MUL_I64"},
        {Opcode::MUL_U8, "MUL_U8"},
        {Opcode::MUL_U16, "MUL_U16"},
        {Opcode::MUL_U32, "MUL_U32"},
        {Opcode::MUL_U64, "MUL_U64"},
        {Opcode::MUL_F32, "MUL_F32"},
        {Opcode::MUL_F64, "MUL_F64"},

        {Opcode::DIV_I8, "DIV_I8"},
        {Opcode::DIV_I16, "DIV_I16"},
        {Opcode::DIV_I32, "DIV_I32"},
        {Opcode::DIV_I64, "DIV_I64"},
        {Opcode::DIV_U8, "DIV_U8"},
        {Opcode::DIV_U16, "DIV_U16"},
        {Opcode::DIV_U32, "DIV_U32"},
        {Opcode::DIV_U64, "DIV_U64"},
        {Opcode::DIV_F32, "DIV_F32"},
        {Opcode::DIV_F64, "DIV_F64"},

        {Opcode::MOD_I8, "MOD_I8"},
        {Opcode::MOD_I16, "MOD_I16"},
        {Opcode::MOD_I32, "MOD_I32"},
        {Opcode::MOD_I64, "MOD_I64"},
        {Opcode::MOD_U8, "MOD_U8"},
        {Opcode::MOD_U16, "MOD_U16"},
        {Opcode::MOD_U32, "MOD_U32"},
        {Opcode::MOD_U64, "MOD_U64"},

        {Opcode::NEG_I8, "NEG_I8"},
        {Opcode::NEG_I16, "NEG_I16"},
        {Opcode::NEG_I32, "NEG_I32"},
        {Opcode::NEG_I64, "NEG_I64"},
        {Opcode::NEG_F32, "NEG_F32"},
        {Opcode::NEG_F64, "NEG_F64"},

        {Opcode::EQ_I8, "EQ_I8"},
        {Opcode::EQ_I16, "EQ_I16"},
        {Opcode::EQ_I32, "EQ_I32"},
        {Opcode::EQ_I64, "EQ_I64"},
        {Opcode::EQ_U8, "EQ_U8"},
        {Opcode::EQ_U16, "EQ_U16"},
        {Opcode::EQ_U32, "EQ_U32"},
        {Opcode::EQ_U64, "EQ_U64"},
        {Opcode::EQ_F32, "EQ_F32"},
        {Opcode::EQ_F64, "EQ_F64"},
        {Opcode::EQ_CHAR, "EQ_CHAR"},
        {Opcode::EQ_BOOL, "EQ_BOOL"},

        {Opcode::NEQ_I8, "NEQ_I8"},
        {Opcode::NEQ_I16, "NEQ_I16"},
        {Opcode::NEQ_I32, "NEQ_I32"},
        {Opcode::NEQ_I64, "NEQ_I64"},
        {Opcode::NEQ_U8, "NEQ_U8"},
        {Opcode::NEQ_U16, "NEQ_U16"},
        {Opcode::NEQ_U32, "NEQ_U32"},
        {Opcode::NEQ_U64, "NEQ_U64"},
        {Opcode::NEQ_F32, "NEQ_F32"},
        {Opcode::NEQ_F64, "NEQ_F64"},
        {Opcode::NEQ_CHAR, "NEQ_CHAR"},
        {Opcode::NEQ_BOOL, "NEQ_BOOL"},

        {Opcode::GT_I8, "GT_I8"},
        {Opcode::GT_I16, "GT_I16"},
        {Opcode::GT_I32, "GT_I32"},
        {Opcode::GT_I64, "GT_I64"},
        {Opcode::GT_U8, "GT_U8"},
        {Opcode::GT_U16, "GT_U16"},
        {Opcode::GT_U32, "GT_U32"},
        {Opcode::GT_U64, "GT_U64"},
        {Opcode::GT_F32, "GT_F32"},
        {Opcode::GT_F64, "GT_F64"},
        {Opcode::GT_CHAR, "GT_CHAR"},

        {Opcode::GTE_I8, "GTE_I8"},
        {Opcode::GTE_I16, "GTE_I16"},
        {Opcode::GTE_I32, "GTE_I32"},
        {Opcode::GTE_I64, "GTE_I64"},
        {Opcode::GTE_U8, "GTE_U8"},
        {Opcode::GTE_U16, "GTE_U16"},
        {Opcode::GTE_U32, "GTE_U32"},
        {Opcode::GTE_U64, "GTE_U64"},
        {Opcode::GTE_F32, "GTE_F32"},
        {Opcode::GTE_F64, "GTE_F64"},
        {Opcode::GTE_CHAR, "GTE_CHAR"},

        {Opcode::LT_I8, "LT_I8"},
        {Opcode::LT_I16, "LT_I16"},
        {Opcode::LT_I32, "LT_I32"},
        {Opcode::LT_I64, "LT_I64"},
        {Opcode::LT_U8, "LT_U8"},
        {Opcode::LT_U16, "LT_U16"},
        {Opcode::LT_U32, "LT_U32"},
        {Opcode::LT_U64, "LT_U64"},
        {Opcode::LT_F32, "LT_F32"},
        {Opcode::LT_F64, "LT_F64"},
        {Opcode::LT_CHAR, "LT_CHAR"},

        {Opcode::LTE_I8, "LTE_I8"},
        {Opcode::LTE_I16, "LTE_I16"},
        {Opcode::LTE_I32, "LTE_I32"},
        {Opcode::LTE_I64, "LTE_I64"},
        {Opcode::LTE_U8, "LTE_U8"},
        {Opcode::LTE_U16, "LTE_U16"},
        {Opcode::LTE_U32, "LTE_U32"},
        {Opcode::LTE_U64, "LTE_U64"},
        {Opcode::LTE_F32, "LTE_F32"},
        {Opcode::LTE_F64, "LTE_F64"},
        {Opcode::LTE_CHAR, "LTE_CHAR"},

        {Opcode::LOGICAL_AND, "LOGICAL_AND"},
        {Opcode::LOGICAL_OR, "LOGICAL_OR"},
        {Opcode::LOGICAL_NOT, "LOGICAL_NOT"},

        { Opcode::INC_I8, "INC_I8"},
        { Opcode::INC_I16, "INC_I16" },
        { Opcode::INC_I32, "INC_I32" },
        { Opcode::INC_I64, "INC_I64" },
        { Opcode::INC_U8, "INC_U8" },
        { Opcode::INC_U16, "INC_U16" },
        { Opcode::INC_U32, "INC_U32" },
        { Opcode::INC_U64, "INC_U64" },

        { Opcode::NOT_I8, "NOT_I8" },
        { Opcode::NOT_I16, "NOT_I16" },
        { Opcode::NOT_I32, "NOT_I32" },
        { Opcode::NOT_I64, "NOT_I64" },
        { Opcode::NOT_U8, "NOT_U8" },
        { Opcode::NOT_U16, "NOT_U16" },
        { Opcode::NOT_U32, "NOT_U32" },
        { Opcode::NOT_U64, "NOT_U64" },

        { Opcode::AND_I8, "AND_I8" },
        { Opcode::AND_I16, "AND_I16" },
        { Opcode::AND_I32, "AND_I32" },
        { Opcode::AND_I64, "AND_I64" },
        { Opcode::AND_U8, "AND_U8" },
        { Opcode::AND_U16, "AND_U16" },
        { Opcode::AND_U32, "AND_U32" },
        { Opcode::AND_U64, "AND_U64" },

        { Opcode::OR_I8, "OR_I8" },
        { Opcode::OR_I16, "OR_I16" },
        { Opcode::OR_I32, "OR_I32" },
        { Opcode::OR_I64, "OR_I64" },
        { Opcode::OR_U8, "OR_U8" },
        { Opcode::OR_U16, "OR_U16" },
        { Opcode::OR_U32, "OR_U32" },
        { Opcode::OR_U64, "OR_U64" },

        { Opcode::XOR_I8, "XOR_I8" },
        { Opcode::XOR_I16, "XOR_I16" },
        { Opcode::XOR_I32, "XOR_I32" },
        { Opcode::XOR_I64, "XOR_I64" },
        { Opcode::XOR_U8, "XOR_U8" },
        { Opcode::XOR_U16, "XOR_U16" },
        { Opcode::XOR_U32, "XOR_U32" },
        { Opcode::XOR_U64, "XOR_U64" },

        { Opcode::SHL_I8, "SHL_I8" },
        { Opcode::SHL_I16, "SHL_I16" },
        { Opcode::SHL_I32, "SHL_I32" },
        { Opcode::SHL_I64, "SHL_I64" },
        { Opcode::SHL_U8, "SHL_U8" },
        { Opcode::SHL_U16, "SHL_U16" },
        { Opcode::SHL_U32, "SHL_U32" },
        { Opcode::SHL_U64, "SHL_U64" },

        { Opcode::SHR_I8, "SHR_I8" },
        { Opcode::SHR_I16, "SHR_I16" },
        { Opcode::SHR_I32, "SHR_I32" },
        { Opcode::SHR_I64, "SHR_I64" },
        { Opcode::SHR_U8, "SHR_U8" },
        { Opcode::SHR_U16, "SHR_U16" },
        { Opcode::SHR_U32, "SHR_U32" },
        { Opcode::SHR_U64, "SHR_U64" },

        { Opcode::DEC_I8, "DEC_I8" },
        { Opcode::DEC_I16, "DEC_I16" },
        { Opcode::DEC_I32, "DEC_I32" },
        { Opcode::DEC_I64, "DEC_I64" },
        { Opcode::DEC_U8, "DEC_U8" },
        { Opcode::DEC_U16, "DEC_U16" },
        { Opcode::DEC_U32, "DEC_U32" },
        { Opcode::DEC_U64, "DEC_U64" },

    };

    auto it = opcodeLookup.find(opcode);
    return (it != opcodeLookup.end()) ? it->second : "UNKNOWN_OPCODE";
}
