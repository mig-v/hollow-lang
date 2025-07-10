#pragma once

#include "opcodes.h"
#include "vm_function_table.h"

#include <array>
#include <vector>

class VM;

typedef void(VM::*VMFuncPtr)();

inline constexpr size_t INITIAL_STACK_SIZE = 4096;

struct StackFrame
{
	size_t retAddress;  // return address to jump back to when we pop this stack frame
	size_t basePointer; // base pointer of the stack frame, locals are indexed as stack[basePointer + slotIndex]
	size_t retSP;       // the sp value to return to, i think i need this, not sure
};

class VM
{
public:
	VM();
	void execute(std::vector<uint8_t>* code, VMFunctionTable* functionTable, int globalVarCount);
private:
	VMFunctionTable* functionTable;
	std::array<VMFuncPtr, static_cast<size_t>(Opcode::NUM_OPCODES)> opcodeTable;
	std::vector<uint64_t> stack;
	std::vector<uint64_t> globals;
	std::vector<StackFrame> callStack;
	std::vector<uint8_t> bytecode;

	size_t ip;
	size_t sp;	// not really needed unless i switch to an array for the stack

	uint8_t  read8();
	uint16_t read16();
	uint32_t read32();
	uint64_t read64();

	void fillOpcodeTable();
	void debugViewState();

	// loads / stores values in the current stack frame
	uint64_t loadLocal(uint16_t slot);
	void storeLocal(uint16_t slot, uint64_t value);

	// loads / stores values in the globals vector at 'slot'
	uint64_t loadGlobal(uint16_t slot);
	void storeGlobal(uint16_t slot, uint64_t value);

	// loads / stores value in the globals vector at the absolute address 'addr'
	uint64_t loadGlobalInd(uint64_t addr);
	void storeGlobalInd(uint64_t addr, uint64_t value);

	// loads / stores values on the stack at the absolute address 'addr'
	uint64_t loadLocalInd(uint64_t addr);
	void storeLocalInd(uint64_t addr, uint64_t value);

	void pushStackFrame(size_t basePointer, size_t returnAddress, size_t retSP);
	void popStackFrame();

	// special instructions never emitted by BytecodeEmitter but implicitly used by the VM to manipulate the stack
	uint64_t POP(); void PUSH(uint64_t value); void SWAP(); uint64_t PEEK();

	// regular instructions the BytecodeEmitter can emit
	void PUSH_I8(); void PUSH_I16(); void PUSH_I32(); void PUSH_I64();
	void PUSH_U8(); void PUSH_U16(); void PUSH_U32(); void PUSH_U64();
	void PUSH_F32(); void PUSH_F64(); void PUSH_CHAR(); void PUSH_BOOL();

	void LDL_I8(); void LDL_I16(); void LDL_I32(); void LDL_I64();
	void LDL_U8(); void LDL_U16(); void LDL_U32(); void LDL_U64();
	void LDL_F32(); void LDL_F64(); void LDL_CHAR(); void LDL_BOOL();

	void STL_I8(); void STL_I16(); void STL_I32(); void STL_I64();
	void STL_U8(); void STL_U16(); void STL_U32(); void STL_U64();
	void STL_F32(); void STL_F64(); void STL_CHAR(); void STL_BOOL();

	void LDG_I8(); void LDG_I16(); void LDG_I32(); void LDG_I64();
	void LDG_U8(); void LDG_U16(); void LDG_U32(); void LDG_U64();
	void LDG_F32(); void LDG_F64(); void LDG_CHAR(); void LDG_BOOL();

	void STG_I8(); void STG_I16(); void STG_I32(); void STG_I64();
	void STG_U8(); void STG_U16(); void STG_U32(); void STG_U64();
	void STG_F32(); void STG_F64(); void STG_CHAR(); void STG_BOOL();

	void JMP(); void JMP_IF_FALSE(); void JMP_IF_TRUE(); void CALL(); 
	void RET(); void DUP();

	void ADD_I8(); void ADD_I16(); void ADD_I32(); void ADD_I64();
	void ADD_U8(); void ADD_U16(); void ADD_U32(); void ADD_U64();
	void ADD_F32(); void ADD_F64();

	void SUB_I8(); void SUB_I16(); void SUB_I32(); void SUB_I64();
	void SUB_U8(); void SUB_U16(); void SUB_U32(); void SUB_U64();
	void SUB_F32(); void SUB_F64();

	void MUL_I8(); void MUL_I16(); void MUL_I32(); void MUL_I64();
	void MUL_U8(); void MUL_U16(); void MUL_U32(); void MUL_U64();
	void MUL_F32(); void MUL_F64();

	void DIV_I8(); void DIV_I16(); void DIV_I32(); void DIV_I64();
	void DIV_U8(); void DIV_U16(); void DIV_U32(); void DIV_U64();
	void DIV_F32(); void DIV_F64();

	void MOD_I8(); void MOD_I16(); void MOD_I32(); void MOD_I64();
	void MOD_U8(); void MOD_U16(); void MOD_U32(); void MOD_U64();

	void NEG_I8(); void NEG_I16(); void NEG_I32(); void NEG_I64();
	void NEG_F32(); void NEG_F64();

	void EQ_I8(); void EQ_I16(); void EQ_I32(); void EQ_I64();
	void EQ_U8(); void EQ_U16(); void EQ_U32(); void EQ_U64();
	void EQ_F32(); void EQ_F64(); void EQ_CHAR(); void EQ_BOOL();

	void NEQ_I8(); void NEQ_I16(); void NEQ_I32(); void NEQ_I64();
	void NEQ_U8(); void NEQ_U16(); void NEQ_U32(); void NEQ_U64();
	void NEQ_F32(); void NEQ_F64(); void NEQ_CHAR(); void NEQ_BOOL();

	void GT_I8(); void GT_I16(); void GT_I32(); void GT_I64();
	void GT_U8(); void GT_U16(); void GT_U32(); void GT_U64();
	void GT_F32(); void GT_F64(); void GT_CHAR();

	void GTE_I8(); void GTE_I16(); void GTE_I32(); void GTE_I64();
	void GTE_U8(); void GTE_U16(); void GTE_U32(); void GTE_U64();
	void GTE_F32(); void GTE_F64(); void GTE_CHAR();

	void LT_I8(); void LT_I16(); void LT_I32(); void LT_I64();
	void LT_U8(); void LT_U16(); void LT_U32(); void LT_U64();
	void LT_F32(); void LT_F64(); void LT_CHAR();

	void LTE_I8(); void LTE_I16(); void LTE_I32(); void LTE_I64();
	void LTE_U8(); void LTE_U16(); void LTE_U32(); void LTE_U64();
	void LTE_F32(); void LTE_F64(); void LTE_CHAR();

	void LOGICAL_AND(); void LOGICAL_OR(); void LOGICAL_NOT();

	void INC_I8(); void INC_I16(); void INC_I32(); void INC_I64();
	void INC_U8(); void INC_U16(); void INC_U32(); void INC_U64();
	
	void DEC_I8(); void DEC_I16(); void DEC_I32(); void DEC_I64();
	void DEC_U8(); void DEC_U16(); void DEC_U32(); void DEC_U64();

	void NOT_I8(); void NOT_I16(); void NOT_I32(); void NOT_I64();
	void NOT_U8(); void NOT_U16(); void NOT_U32(); void NOT_U64();

	void AND_I8(); void AND_I16(); void AND_I32(); void AND_I64();
	void AND_U8(); void AND_U16(); void AND_U32(); void AND_U64();

	void OR_I8(); void OR_I16(); void OR_I32(); void OR_I64();
	void OR_U8(); void OR_U16(); void OR_U32(); void OR_U64();

	void XOR_I8(); void XOR_I16(); void XOR_I32(); void XOR_I64();
	void XOR_U8(); void XOR_U16(); void XOR_U32(); void XOR_U64();

	void SHL_I8(); void SHL_I16(); void SHL_I32(); void SHL_I64();
	void SHL_U8(); void SHL_U16(); void SHL_U32(); void SHL_U64();

	void SHR_I8(); void SHR_I16(); void SHR_I32(); void SHR_I64();
	void SHR_U8(); void SHR_U16(); void SHR_U32(); void SHR_U64();

	void LDL_IND_I8(); void LDL_IND_I16(); void LDL_IND_I32(); void LDL_IND_I64();
	void LDL_IND_U8(); void LDL_IND_U16(); void LDL_IND_U32(); void LDL_IND_U64();
	void LDL_IND_F32(); void LDL_IND_F64(); void LDL_IND_CHAR(); void LDL_IND_BOOL();

	void LDG_IND_I8(); void LDG_IND_I16(); void LDG_IND_I32(); void LDG_IND_I64();
	void LDG_IND_U8(); void LDG_IND_U16(); void LDG_IND_U32(); void LDG_IND_U64();
	void LDG_IND_F32(); void LDG_IND_F64(); void LDG_IND_CHAR(); void LDG_IND_BOOL();

	void STL_IND_I8(); void STL_IND_I16(); void STL_IND_I32(); void STL_IND_I64();
	void STL_IND_U8(); void STL_IND_U16(); void STL_IND_U32(); void STL_IND_U64();
	void STL_IND_F32(); void STL_IND_F64(); void STL_IND_CHAR(); void STL_IND_BOOL();

	void STG_IND_I8(); void STG_IND_I16(); void STG_IND_I32(); void STG_IND_I64();
	void STG_IND_U8(); void STG_IND_U16(); void STG_IND_U32(); void STG_IND_U64();
	void STG_IND_F32(); void STG_IND_F64(); void STG_IND_CHAR(); void STG_IND_BOOL();

	void NOP();
};