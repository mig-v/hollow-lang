//#pragma once
//
//#include "ir/ir_core.h"
//
//#include "codegen/code_emitter.h"
//
//#include "archive/register.h"
//#include "archive/constant_pool.h"
//
//#include <fstream>
//#include <array>
//
//enum class X86GPRegister
//{
//	// NOTE: r11 is reserved as scratch, can also reserve r10 as a scratch if needed
//	rax, rcx, rdx, r8, r9, r10,       // volatile (caller saved)
//	rbx, rdi, rsi, r12, r13, r14, r15, // non volatile (callee saved)
//	numGpRegisters
//};
//
//enum class X86FPRegister
//{
//	// NOTE: xmm7 is reserved as an fp scratch
//	xmm0, xmm1, xmm2, xmm3, xmm4, xmm5,                         // volatile (caller saved)
//	xmm6, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15, // non volatile (callee saved)
//	numFpRegisters
//};
//
//enum class SubRegisterAccess
//{
//	dword, word, byte, subRegisterAccessCount
//};
//
//class x86_64Emitter : public CodeEmitter
//{
//public:
//	void emitCode(IRData* data, TypeTable* typeTable, ConstantPool* constantPool) override;
//	void emitFunction(const IRFunction& function) override;
//	void emitInstruction(const TacInstruction& instruction) override;
//	void initTargetInfo() override;
//
//private:
//	void initSubRegisters();
//	void layoutFunctionMemory(IRFunction* function);
//	void layoutFunctionParams(IRFunction* function);
//	void layoutFunctionLocals(IRFunction* function);
//	void emitEntryPoint();
//	void emitConstantSection();
//	void emitConstant(const IROperand& constant);
//	void emitRegister(TacValue& reg);
//	void emitParameter(TacValue& param);
//	void emitGPParameter(uint32_t gpParamCount, uint32_t fpParamCount, uint32_t paramIndexForType, TypeKind paramType);
//	void emitRegisterSize(uint32_t regID, TypeKind type);
//	void emitVariable(TacValue& var);
//	void emitIROperand(const IROperand& val);
//	void emitFunctionPrologue(const IRFunction& function);
//	void emitFunctionEpilogue(const IRFunction& function);
//	void emitOperandSize(const IROperand& operand);
//	void emitTypeKindSize(TypeKind kind);
//
//	void emitHardCodedGPMov(uint32_t destID, const IROperand& source);
//	void emitGPScratchRegister(TypeKind type);
//	void emitGPScratchMov(const IROperand& destination, const IROperand& source);
//	void emitFPScratchMov(const IROperand& destination, const IROperand& source);
//	void emitSignedDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitUnsignedDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitFloatDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitMov(const IROperand& destination, const IROperand& source);
//	void emitAdd(const IROperand& lhs, const IROperand& rhs);
//	void emitSub(const IROperand& lhs, const IROperand& rhs);
//	void emitMul(const IROperand& lhs, const IROperand& rhs);
//	void emitDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitRet(const IROperand& retVal, bool lastInstruction);
//	void emitShl(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitShr(const IROperand& destination, const IROperand& lhs, const IROperand& rhs);
//	void emitBitOp(const TacInstruction& instruction);
//	void emitLabel(const IROperand& label);
//	void emitCmpJmp(const IROperand& label, const IROperand& lhs, const IROperand& rhs, JumpKind jumpKind);
//	void emitJmp(const IROperand& label);
//	void emitCmp(const TacInstruction& instruction);
//	void emitNeg(const IROperand& value);
//	void emitBitNot(const IROperand& value);
//	void emitLogicalNot(const IROperand& value);
//	void emitCondJmp(const IROperand& value, const IROperand& label, TacOpcode opcode);
//	void emitArg(const IROperand& arg);
//	void emitCall(const IROperand& ret, const IROperand& func);
//	void emitGPArg(const IROperand& arg, uint32_t argCount);
//
//	bool instrNeedsSizeSpecifier(const IROperand& source);
//	bool lastInstructionOfFunction(const TacInstruction& instruction);
//	bool operandIsMemLocation(const IROperand& operand);
//	bool isRedundantMov(const IROperand& destination, const IROperand& src);
//	bool isMemToMemMov(const IROperand& destination, const IROperand& src);
//	bool isFloatImmToMemMov(const IROperand& destination, const IROperand& src);
//	bool typeGoesInGPRegister(TypeKind type);
//
//	const char* getTypedMov(const IROperand& source);
//	const char* getTypedAdd(const IROperand& lhs);
//	const char* getTypedSub(const IROperand& lhs);
//	const char* getTypedMul(const IROperand& lhs);
//	const char* getTypedCmp(const IROperand& lhs);
//	const char* getTypedSet(const IROperand& lhs, TacOpcode comparison);
//	const char* getJumpKind(JumpKind kind, bool isFloatComparison);
//
//	TypeKind getTypeFromOperand(const IROperand& operand);
//	TypeKind getTypeFromTacValue(const TacValue& value);
//
//	const char* scratchGPRegister = "r11";
//	const char* scratchFPRegister = "xmm7";
//
//	const size_t RESERVED_GP_PARAM_REGISTERS = 4;
//	const size_t RESERVED_FP_PARAM_REGISTERS = 4;
//
//	const std::array<const char*, static_cast<size_t>(X86GPRegister::numGpRegisters)> gpRegisters = 
//	{ "rax", "rcx", "rdx", "r8", "r9", "r10", "rbx", "rdi" , "rsi" , "r12" , "r13" , "r14" , "r15" };
//
//	const std::array<const char*, static_cast<size_t>(X86FPRegister::numFpRegisters)> fpRegisters =
//	{ "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15" };
//
//	const std::array<X86GPRegister, 4> gpParamRegisters = { X86GPRegister::rcx, X86GPRegister::rdx, X86GPRegister::r8, X86GPRegister::r9 };
//
//	// array of 3 strings for each gp register for sub register access, each gp register has 3 sub register aliases
//	// eg: rax -> dword = eax, word = ax, and byte = al
//	std::array<std::string, static_cast<size_t>(SubRegisterAccess::subRegisterAccessCount) * static_cast<size_t>(X86GPRegister::numGpRegisters)> gpSubRegisters;
//	std::vector<IROperand> funcArgs;
//	std::ofstream out;
//	IRData* data;
//	ConstantPool* constantPool;
//	IRFunction* currentFunction;
//	TypeTable* typeTable;
//	bool needsEpilogueLabel;
//};