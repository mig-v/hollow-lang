//#include "archive/x86_64_emitter.h"
//
//#include "utils/semantic_utils.h"
//#include "utils/type_utils.h"
//#include "utils/debug_utils.h"
//
//#include <iostream>
//#include <iomanip>
//
//void x86_64Emitter::initTargetInfo()
//{
//	targetInfo.architecture = TargetArch::X86_64;
//	
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::rbx), false,  false});
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::rdi), false, false});
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::rsi), false, false });
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::r12), false, false });
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::r13), false, false });
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::r14), false, false });
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::r15), false, false });
//
//	targetInfo.gpRegisters.push_back({ static_cast<uint32_t>(X86GPRegister::rax), true, false });
//	targetInfo.gpRegisters.push_back({static_cast<uint32_t>(X86GPRegister::rcx), true, true });
//	targetInfo.gpRegisters.push_back({static_cast<uint32_t>(X86GPRegister::rdx), true, true });
//	targetInfo.gpRegisters.push_back({static_cast<uint32_t>(X86GPRegister::r8), true, true });
//	targetInfo.gpRegisters.push_back({static_cast<uint32_t>(X86GPRegister::r9), true, true });
//	targetInfo.gpRegisters.push_back({static_cast<uint32_t>(X86GPRegister::r10), true, false });
//
//	targetInfo.fpRegisters.push_back({ static_cast<uint32_t>(X86FPRegister::xmm6), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm8), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm9), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm10), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm11), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm12), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm13), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm14), false, false });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm15), false, false });
//
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm0), true, true });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm1), true, true });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm2), true, true });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm3), true, true });
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm4), true, false});
//	targetInfo.fpRegisters.push_back({static_cast<uint32_t>(X86FPRegister::xmm5), true, false});
//
//	targetInfo.registerConstraints[TacOpcode::Div].gpConstrainedRegisters = { {static_cast<uint32_t>(X86GPRegister::rax), static_cast<uint32_t>(X86GPRegister::rcx), static_cast<uint32_t>(X86GPRegister::rdx)} };
//	targetInfo.registerConstraints[TacOpcode::BitShiftLeft].gpConstrainedRegisters = { {static_cast<uint32_t>(X86GPRegister::rcx)} };
//	targetInfo.registerConstraints[TacOpcode::BitShiftRight].gpConstrainedRegisters = { {static_cast<uint32_t>(X86GPRegister::rcx)} };
//	targetInfo.registerConstraints[TacOpcode::Call].gpConstrainedRegisters = 
//	{{
//		static_cast<uint32_t>(X86GPRegister::rax), 
//		static_cast<uint32_t>(X86GPRegister::rcx),
//		static_cast<uint32_t>(X86GPRegister::rdx),
//		static_cast<uint32_t>(X86GPRegister::r8),
//		static_cast<uint32_t>(X86GPRegister::r9)
//	}};
//
//}
//
//void x86_64Emitter::emitCode(IRData* data, TypeTable* typeTable, ConstantPool* constantPool)
//{
//	initSubRegisters();
//	this->data = data;
//	this->typeTable = typeTable;
//	this->constantPool = constantPool;
//	this->currentFunction = nullptr;
//	this->needsEpilogueLabel = false;
//
//	out.open("hollow_out.asm");
//
//	// emit boiler plate entry point asm code
//	emitEntryPoint();
//
//	for (size_t i = 0; i < data->functionTable.size(); i++)
//	{
//		currentFunction = data->functionTable[i];
//		layoutFunctionMemory(data->functionTable[i]);
//		emitFunction(*data->functionTable[i]);
//	}
//
//	out.close();
//}
//
//void x86_64Emitter::initSubRegisters()
//{
//	// rax, rcx, rdx, r8, r9, r10,       // volatile (caller saved)
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rax) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "eax";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rax) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "ax";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rax) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "al";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rcx) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "ecx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rcx) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "cx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rcx) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "cl";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdx) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "edx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdx) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "dx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdx) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "dl";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r8) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r8d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r8) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r8w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r8) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r8b";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r9) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r9d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r9) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r9w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r9) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r9b";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r10) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r10d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r10) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r10w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r10) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r10b";
//
//	//rbx, rdi, rsi, r12, r13, r14, r15, // non volatile (callee saved)
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rbx) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "ebx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rbx) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "bx";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rbx) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "bl";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdi) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "edi";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdi) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "di";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rdi) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "dil";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rsi) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "esi";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rsi) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "si";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::rsi) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "sil";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r12) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r12d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r12) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r12w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r12) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r12b";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r13) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r13d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r13) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r13w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r13) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r13b";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r14) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r14d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r14) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r14w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r14) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r14b";
//
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r15) * 3) + static_cast<size_t>(SubRegisterAccess::dword)] = "r15d";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r15) * 3) + static_cast<size_t>(SubRegisterAccess::word)] = "r15w";
//	gpSubRegisters[(static_cast<size_t>(X86GPRegister::r15) * 3) + static_cast<size_t>(SubRegisterAccess::byte)] = "r15b";
//}
//
//void x86_64Emitter::emitEntryPoint()
//{
//	out << "extern ExitProcess\n\n";
//
//	emitConstantSection();
//
//	out << "section .text\n";
//	out << "global _start\n";
//	out << "global main\n\n";
//
//	out << "_start:\n";
//	out << "    sub rsp, 32\n";
//	out << "    call main\n";
//	out << "    add rsp, 32\n";
//	out << "    mov rcx, rax\n";
//	out << "    call ExitProcess\n\n";
//}
//
//void x86_64Emitter::emitConstantSection()
//{
//	out << "section .rodata\n";
//
//	// emit neg masks for negating f32 / f64 values
//	out << "    f32_neg_mask: dd 0x80000000\n";
//	out << "    f64_neg_mask: dq 0x8000000000000000\n";
//	for (size_t i = 0; i < constantPool->f32Pool.size(); i++)
//		out << "    f32_const_" << i << ": dd __float32__(" << std::setprecision(9) << std::fixed << constantPool->f32Pool[i] << ")\n";
//
//	out << "\n";
//	
//	for (size_t i = 0; i < constantPool->f64Pool.size(); i++)
//		out << "    f64_const_" << i << ": dq __float64__(" << std::setprecision(17) << std::fixed << constantPool->f64Pool[i] << ")\n";
//
//	out << "\n";
//
//	// reset io state to what it was
//	out.unsetf(std::ios::fixed);
//	out << std::setprecision(6); 
//}
//
//void x86_64Emitter::emitFunction(const IRFunction& function)
//{
//	needsEpilogueLabel = false;
//	out << function.name << ":\n";
//
//	emitFunctionPrologue(function);
//
//	// emit function body
//	for (const TacInstruction& instruction : function.body)
//		emitInstruction(instruction);
//
//	emitFunctionEpilogue(function);
//}
//
//void x86_64Emitter::emitFunctionPrologue(const IRFunction& function)
//{
//	out << "    push rbp\n";
//	out << "    mov rbp, rsp\n";
//
//	if (function.spillSpace > 0)
//		out << "    sub rsp, " << function.spillSpace << std::endl;
//}
//
//void x86_64Emitter::emitFunctionEpilogue(const IRFunction& function)
//{
//	if (needsEpilogueLabel)
//		out << ".L" + std::to_string(function.funcID) << "_Epilogue:\n";
//
//	if (function.spillSpace > 0)
//		out << "    add rsp, " << function.spillSpace << std::endl;
//	
//	out << "    pop rbp\n";
//	out << "    ret\n";
//}
//
//void x86_64Emitter::emitOperandSize(const IROperand& operand)
//{
//	if (operand.kind == ValueKind::Constant || operand.kind == ValueKind::Temporary)
//		emitTypeKindSize(operand.type);
//	else if (operand.kind == ValueKind::Variable)
//		emitTypeKindSize(data->values[operand.id].typeInfo->type);
//}
//
//void x86_64Emitter::emitTypeKindSize(TypeKind kind)
//{
//	switch (kind)
//	{
//		case TypeKind::u8:
//		case TypeKind::i8:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			out << "byte ";
//			break;
//
//		case TypeKind::u16:
//		case TypeKind::i16:
//			out << "word ";
//			break;
//
//		case TypeKind::u32:
//		case TypeKind::i32:
//		case TypeKind::f32:
//			out << "dword ";
//			break;
//
//		case TypeKind::u64:
//		case TypeKind::i64:
//		case TypeKind::f64:
//		case TypeKind::Pointer:
//			out << "qword ";
//			break;
//
//		default:
//			out << "unsupported type size";
//	}
//}
//
//void x86_64Emitter::layoutFunctionMemory(IRFunction* function)
//{
//	// at this point, function->stackFrameSize holds the amount of space needed for reg spills allocated in register allocation
//	// params and locals are slotted in below the spills
//	layoutFunctionParams(function);
//	layoutFunctionLocals(function);
//	function->stackFrameSize = TypeUtils::alignTo(function->stackFrameSize, 16);
//}
//
//void x86_64Emitter::layoutFunctionParams(IRFunction* function)
//{
//	uint32_t gpArgCount = 0;
//	uint32_t fpArgCount = 0;
//	uint32_t spilledCount = 0;
//
//	for (size_t i = 0; i < function->paramIDs.size(); i++)
//	{
//		TacValue& param = data->values[function->paramIDs[i]];
//		TypeKind paramType = getTypeFromTacValue(param);
//
//		if (typeGoesInGPRegister(paramType))
//		{
//			if (gpArgCount < RESERVED_GP_PARAM_REGISTERS)
//			{
//				param.regInfo.spilled = false;
//				param.regInfo.regClass = RegisterClass::GP;
//				param.regInfo.regID = static_cast<uint32_t>(gpParamRegisters[gpArgCount]);
//
//			}
//			else
//			{
//				param.regInfo.spilled = true;
//				param.regInfo.stackOffset = 48 + 8 * spilledCount;
//				spilledCount++;
//				// can add extra param space here if needed later on for structs or arrays or something
//				//function->extraParamSpace += 8;
//			}
//
//			gpArgCount++;
//
//		}
//		else
//		{
//			fpArgCount++;
//
//			if (fpArgCount > 4)
//			{
//				function->extraParamSpace += 8;
//				param.regInfo.stackOffset = function->extraParamSpace;
//			}
//		}
//	}
//
//	function->extraParamSpace = spilledCount * 8;
//}
//
//void x86_64Emitter::layoutFunctionLocals(IRFunction* function)
//{
//	// i can do this i think, just setting it equal to spill space which the register allocator allocates
//	//function->stackFrameSize = function->spillSpace;
//
//	for (size_t i = 0; i < function->localIDs.size(); i++)
//	{
//		TacValue& local = data->values[function->localIDs[i]];
//
//		if (SemanticUtils::typeIsPrimitive(local.typeInfo->type) || local.typeInfo->type == TypeKind::Pointer)
//		{
//			local.regInfo.spilled = true;
//			function->spillSpace += 8;
//			local.regInfo.stackOffset = function->spillSpace;
//		}
//	}
//}
//
//void x86_64Emitter::emitConstant(const IROperand& constant)
//{
//	if (SemanticUtils::isSigned(constant.type))
//		out << std::to_string(std::get<int64_t>(constant.data));
//	else if (SemanticUtils::isUnsigned(constant.type))
//		out << std::to_string(std::get<uint64_t>(constant.data));
//	else if (constant.type == TypeKind::f32)
//		out << "[f32_const_" << constant.constPoolID << "]";
//	else if (constant.type == TypeKind::f64)
//		out << "[f64_const_" << constant.constPoolID << "]";
//	else if (constant.type == TypeKind::Char)
//		out << std::to_string(std::get<char>(constant.data));
//	else if (constant.type == TypeKind::Bool)
//		out << std::to_string(std::get<bool>(constant.data));
//}
//
//void x86_64Emitter::emitRegisterSize(uint32_t regID, TypeKind type)
//{
//	switch (type)
//	{
//		case TypeKind::i8:
//		case TypeKind::u8:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			out << gpSubRegisters[(regID * static_cast<size_t>(SubRegisterAccess::subRegisterAccessCount)) + static_cast<size_t>(SubRegisterAccess::byte)];
//			break;
//		case TypeKind::i16:
//		case TypeKind::u16:
//			out << gpSubRegisters[(regID * static_cast<size_t>(SubRegisterAccess::subRegisterAccessCount)) + static_cast<size_t>(SubRegisterAccess::word)];
//			break;
//		case TypeKind::i32:
//		case TypeKind::u32:
//			out << gpSubRegisters[(regID * static_cast<size_t>(SubRegisterAccess::subRegisterAccessCount)) + static_cast<size_t>(SubRegisterAccess::dword)];
//			break;
//		case TypeKind::i64:
//		case TypeKind::u64:
//		case TypeKind::Pointer:
//			out << gpSubRegisters[(regID * static_cast<size_t>(SubRegisterAccess::subRegisterAccessCount))];
//			break;
//		case TypeKind::f32:
//		case TypeKind::f64:
//			out << fpRegisters[regID];
//			break;
//	}
//}
//
//void x86_64Emitter::emitRegister(TacValue& reg)
//{
//	// if reg info is not spilled, we can emit the register, otherwise we need to emit a stack offset
//	if (!reg.regInfo.spilled)
//		emitRegisterSize(reg.regInfo.regID, getTypeFromTacValue(reg));
//	else
//		out << "[rbp - " << reg.regInfo.stackOffset << "]";
//}
//
//void x86_64Emitter::emitGPParameter(uint32_t gpParamCount, uint32_t fpParamCount, uint32_t paramIndexForType, TypeKind paramType)
//{
//	// stack offset
//	if (gpParamCount >= RESERVED_GP_PARAM_REGISTERS)
//	{
//
//	}
//	else
//	{
//		switch (gpParamCount)
//		{
//			case 0: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rcx), paramType); break;
//			case 1: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rdx), paramType); break;
//			case 2: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::r8), paramType); break;
//			case 3: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::r9), paramType); break;
//		}
//	}
//}
//
//void x86_64Emitter::emitParameter(TacValue& param)
//{
//	TypeKind paramType = getTypeFromTacValue(param);
//
//	if (param.regInfo.spilled)
//	{
//		emitTypeKindSize(paramType);
//		out << "[rbp + " << param.regInfo.stackOffset << "]";
//	}
//	else
//	{
//		emitRegisterSize(param.regInfo.regID, paramType);
//	}
//	//IRFunction* function = data->functionTable[param.funcID];
//	//
//	//uint32_t gpParamCount = 0;
//	//uint32_t fpParamCount = 0;
//	//uint32_t paramIndexForType = 0;
//	//uint32_t spillOffset = 0;
//	//uint32_t spilledCount = 0;
//	//uint32_t snapshotSpillCount = 0;
//	//TypeKind paramType = getTypeFromTacValue(param);
//	//bool paramIsGP = typeGoesInGPRegister(paramType);
//	//for (size_t i = 0; i < function->paramIDs.size(); i++)
//	//{
//	//	if (param.paramIndex == i)
//	//	{
//	//		if (paramIsGP)
//	//			paramIndexForType = gpParamCount;
//	//		else
//	//			paramIndexForType = fpParamCount;
//
//	//		snapshotSpillCount = spilledCount;
//	//	}
//
//	//	if (typeGoesInGPRegister(getTypeFromTacValue(data->values[function->paramIDs[i]])))
//	//	{
//	//		gpParamCount++;
//	//		if (gpParamCount > RESERVED_GP_PARAM_REGISTERS)
//	//			spilledCount++;
//	//	}
//	//	else
//	//	{
//	//		fpParamCount++;
//	//		if (fpParamCount > RESERVED_FP_PARAM_REGISTERS)
//	//			spilledCount++;
//	//	}
//	//}
//
//	//if (paramIsGP)
//	//{
//	//	// stack offset
//	//	if (paramIndexForType >= RESERVED_GP_PARAM_REGISTERS)
//	//	{
//	//		//uint32_t offset = (snapshotSpillCount + 1) * 8
//
//	//		uint32_t offset = 32 + function->extraParamSpace;
//	//		offset = TypeUtils::alignTo(offset, 16);
//	//		emitTypeKindSize(paramType);
//	//		offset += (snapshotSpillCount) * 8;
//	//		out << "[rbp + " << offset << "]";
//
//	//		//out << "[rsp + " << 32 + (snapshotSpillCount * 8) << "]";
//	//	}
//	//	else
//	//	{
//	//		switch (paramIndexForType)
//	//		{
//	//			case 0: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rcx), paramType); break;
//	//			case 1: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rdx), paramType); break;
//	//			case 2: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::r8), paramType); break;
//	//			case 3: emitRegisterSize(static_cast<uint32_t>(X86GPRegister::r9), paramType); break;
//	//		}
//	//	}
//	//}
//}
//
//void x86_64Emitter::emitVariable(TacValue& var)
//{
//	// only emit the stack offset for variable access when it's not zero
//	if (var.regInfo.spilled)
//	{
//		if (var.regInfo.stackOffset == 0)
//			out << "[rbp]";
//		else
//			out << "[rbp - " << var.regInfo.stackOffset << "]";
//	}
//	else
//	{
//		emitRegister(var);
//	}
//}
//
//void x86_64Emitter::emitIROperand(const IROperand& operand)
//{
//	switch (operand.kind)
//	{
//		case ValueKind::Constant:
//			emitConstant(operand);
//			break;
//		case ValueKind::Temporary:
//			emitRegister(data->values[operand.id]);
//			break;
//		case ValueKind::Variable:
//			emitVariable(data->values[operand.id]);
//			break;
//		case ValueKind::Param:
//			std::cout << "emitting IR OPerand paramINDEX = " << data->values[operand.id].paramIndex << " func ID = " << data->values[operand.id].funcID << std::endl;
//			emitParameter(data->values[operand.id]);
//			break;
//	}
//}
//
//bool x86_64Emitter::operandIsMemLocation(const IROperand& operand)
//{
//	return ((operand.kind == ValueKind::Temporary || operand.kind == ValueKind::Variable) && (data->values[operand.id].regInfo.spilled));
//}
//
//bool x86_64Emitter::isRedundantMov(const IROperand& destination, const IROperand& src)
//{
//	// if moving a constant into a reg / mem location, the mov isn't redundant
//	if (src.kind == ValueKind::Constant)
//		return false;
//
//	RegisterInfo& destRegInfo = data->values[destination.id].regInfo;
//	RegisterInfo& srcRegInfo = data->values[src.id].regInfo;
//
//	// if either is spilled to the stack, the mov isn't redundant
//	if (destRegInfo.spilled || srcRegInfo.spilled)
//		return false;
//
//	// after early checks to return, check if the register ID's are the same, if they are return true
//	// this will eliminate redundant moves like mov ecx, ecx
//	return (destRegInfo.regID == srcRegInfo.regID);
//}
//
//bool x86_64Emitter::isMemToMemMov(const IROperand& destination, const IROperand& src)
//{
//	if (src.kind == ValueKind::Constant)
//		return false;
//
//	RegisterInfo& destRegInfo = data->values[destination.id].regInfo;
//	RegisterInfo& srcRegInfo = data->values[src.id].regInfo;
//
//	return (destRegInfo.spilled && srcRegInfo.spilled);
//}
//
//bool x86_64Emitter::isFloatImmToMemMov(const IROperand& destination, const IROperand& src)
//{
//	// if source is an f32 or f64 constant, it's guaranteed to be pooled, we must also check that the destination is a memory location and not a reg
//	return (src.kind == ValueKind::Constant 
//		&& data->values[destination.id].regInfo.spilled 
//		&& (src.type == TypeKind::f32 || src.type == TypeKind::f64));
//}
//
//bool x86_64Emitter::typeGoesInGPRegister(TypeKind type)
//{
//	return (SemanticUtils::isInteger(type) || type == TypeKind::Char || type == TypeKind::Bool || type == TypeKind::Pointer);
//}
//
//TypeKind x86_64Emitter::getTypeFromOperand(const IROperand& operand)
//{
//	TypeKind operandType = TypeKind::Unknown;
//	if (operand.kind == ValueKind::Constant)
//		operandType = operand.type;
//	else if (operand.kind == ValueKind::Temporary)
//		operandType = data->values[operand.id].temporaryType;
//	else if (operand.kind == ValueKind::Variable || operand.kind == ValueKind::Param)
//		operandType = data->values[operand.id].typeInfo->type;
//
//	return operandType;
//}
//
//TypeKind x86_64Emitter::getTypeFromTacValue(const TacValue& value)
//{
//	if (value.kind == ValueKind::Constant || value.kind == ValueKind::Temporary)
//		return value.temporaryType;
//	else if (value.kind == ValueKind::Variable || value.kind == ValueKind::Param)
//		return value.typeInfo->type;
//
//	return TypeKind::Unknown;
//}
//
//bool x86_64Emitter::instrNeedsSizeSpecifier(const IROperand& source)
//{
//	TypeKind sourceType = getTypeFromOperand(source);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			return true;
//		case TypeKind::f32:
//			return false;
//		case TypeKind::f64:
//			return false;
//		default:
//			return true;
//	}
//}
//
//bool x86_64Emitter::lastInstructionOfFunction(const TacInstruction& instruction)
//{
//	return (&instruction == &currentFunction->body.back());
//}
//
//const char* x86_64Emitter::getTypedMov(const IROperand& source)
//{
//	TypeKind sourceType = getTypeFromOperand(source);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			return "mov";
//		case TypeKind::f32:
//			return "movss";
//		case TypeKind::f64:
//			return "movsd";
//		default:
//			return "unknown type for mov";
//	}
//}
//
//const char* x86_64Emitter::getTypedAdd(const IROperand& lhs)
//{
//	TypeKind sourceType = getTypeFromOperand(lhs);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//		case TypeKind::Char:
//			return "add";
//		case TypeKind::f32:
//			return "addss";
//		case TypeKind::f64:
//			return "addsd";
//		default:
//			return "unknown type for add";
//	}
//}
//
//const char* x86_64Emitter::getTypedSub(const IROperand& lhs)
//{
//	TypeKind sourceType = getTypeFromOperand(lhs);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//		case TypeKind::Char:
//			return "sub";
//		case TypeKind::f32:
//			return "subss";
//		case TypeKind::f64:
//			return "subsd";
//		default:
//			return "unknown type for sub";
//	}
//}
//
//const char* x86_64Emitter::getTypedMul(const IROperand& lhs)
//{
//	TypeKind sourceType = getTypeFromOperand(lhs);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//			return "imul";
//		case TypeKind::f32:
//			return "mulss";
//		case TypeKind::f64:
//			return "mulsd";
//		default:
//			return "unknown type for mul";
//	}
//}
//
//const char* x86_64Emitter::getTypedCmp(const IROperand& lhs)
//{
//	TypeKind sourceType = getTypeFromOperand(lhs);
//
//	switch (sourceType)
//	{
//		case TypeKind::u8:
//		case TypeKind::u16:
//		case TypeKind::u32:
//		case TypeKind::u64:
//		case TypeKind::i8:
//		case TypeKind::i16:
//		case TypeKind::i32:
//		case TypeKind::i64:
//		case TypeKind::Pointer:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			return "cmp";
//		case TypeKind::f32:
//			return "ucomiss";
//		case TypeKind::f64:
//			return "ucomisd";
//		default:
//			return "unknown type for cmp";
//	}
//}
//
//const char* x86_64Emitter::getJumpKind(JumpKind kind, bool isFloatComparison)
//{
//	switch (kind)
//	{
//		case JumpKind::Je:  return "je";
//		case JumpKind::Jne: return "jne";
//		case JumpKind::Jg:  return isFloatComparison ? "ja" : "jg";
//		case JumpKind::Jge: return isFloatComparison ? "jae" : "jge";
//		case JumpKind::Jl:  return isFloatComparison ? "jb" : "jl";
//		case JumpKind::Jle: return isFloatComparison ? "jbe" : "jle";
//	}
//}
//
//const char* x86_64Emitter::getTypedSet(const IROperand& lhs, TacOpcode comparison)
//{
//	bool isSigned = SemanticUtils::isSigned(getTypeFromOperand(lhs));
//	switch (comparison)
//	{
//		// works for signed, unsigned, and float types
//		case TacOpcode::Eq:    return "sete";
//		case TacOpcode::NotEq: return "setne";
//
//		// these depend on signed or unsigned
//		case TacOpcode::Lt:    return isSigned ? "setl" : "setb";
//		case TacOpcode::Lte:   return isSigned ? "setle" : "setbe";
//		case TacOpcode::Gt:    return isSigned ? "setg" : "seta";
//		case TacOpcode::Gte:   return isSigned ? "setge" : "setae";
//
//		default: 
//			return "unsupported type for set flags";
//	}
//}
//
//void x86_64Emitter::emitHardCodedGPMov(uint32_t destID, const IROperand& source)
//{
//	// redundant
//	if (source.kind != ValueKind::Constant && !data->values[source.id].regInfo.spilled && data->values[source.id].regInfo.regID == destID)
//		return;
//
//	TypeKind type = getTypeFromOperand(source);
//	out << "    mov ";
//
//	emitRegisterSize(destID, type);
//	out << ", ";
//
//	if (instrNeedsSizeSpecifier(source) && operandIsMemLocation(source))
//		emitOperandSize(source);
//
//	emitIROperand(source);
//	out << "\n";
//}
//
//void x86_64Emitter::emitGPScratchRegister(TypeKind type)
//{
//	switch (type)
//	{
//		case TypeKind::i8:
//		case TypeKind::u8:
//		case TypeKind::Char:
//		case TypeKind::Bool:
//			out << scratchGPRegister << "b";
//			break;
//		case TypeKind::i16:
//		case TypeKind::u16:
//			out << scratchGPRegister << "w";
//			break;
//		case TypeKind::i32:
//		case TypeKind::u32:
//			out << scratchGPRegister << "d";
//			break;
//		case TypeKind::i64:
//		case TypeKind::u64:
//		case TypeKind::Pointer:
//			out << scratchGPRegister;
//			break;
//	}
//}
//
//void x86_64Emitter::emitGPScratchMov(const IROperand& destination, const IROperand& source)
//{
//	// mov scratch, source
//	// mov destination, scratch
//	TypeKind type = getTypeFromOperand(source);
//	out << "    mov ";
//	emitGPScratchRegister(type);
//	out << ", ";
//	emitIROperand(source);
//	out << std::endl;
//
//	out << "    mov ";
//	if (operandIsMemLocation(destination) && instrNeedsSizeSpecifier(source))
//		emitOperandSize(source);
//
//	emitIROperand(destination);
//	out << ", ";
//	emitGPScratchRegister(type);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitFPScratchMov(const IROperand& destination, const IROperand& source)
//{
//	// mov scratch, source
//	// mov destination, scratch
//	out << "    " << getTypedMov(source) << " " << scratchFPRegister << ", ";
//	emitIROperand(source);
//	out << std::endl;
//
//	out << "    " << getTypedMov(source) << " ";
//	emitIROperand(destination);
//	out << ", " << scratchFPRegister << "\n";
//}
//
//void x86_64Emitter::emitMov(const IROperand& destination, const IROperand& source)
//{
//	if (isRedundantMov(destination, source))
//		return;
//
//	if (isMemToMemMov(destination, source))
//	{
//		std::cout << "mem to mem mov\n";
//		emitGPScratchMov(destination, source);
//		return;
//	}
//
//	if (isFloatImmToMemMov(destination, source))
//	{
//		emitFPScratchMov(destination, source);
//		return;
//	}
//	
//	std::cout << "Regular x86 mov\n";
//	out << "    " << getTypedMov(source) << " ";
//
//	if (operandIsMemLocation(destination) && instrNeedsSizeSpecifier(source))
//		emitOperandSize(source);
//
//	emitIROperand(destination);
//	out << ", ";
//	emitIROperand(source);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitAdd(const IROperand& lhs, const IROperand& rhs)
//{
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	out << "    " << getTypedAdd(lhs) << " ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(lhs))
//		emitOperandSize(lhs);
//
//	emitIROperand(lhs);
//	out << ", ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(rhs))
//		emitOperandSize(rhs);
//
//	emitIROperand(rhs);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitSub(const IROperand& lhs, const IROperand& rhs)
//{
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	out << "    " << getTypedSub(lhs) << " ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(lhs))
//		emitOperandSize(lhs);
//
//	emitIROperand(lhs);
//	out << ", ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(rhs))
//		emitOperandSize(rhs);
//
//	emitIROperand(rhs);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitMul(const IROperand& lhs, const IROperand& rhs)
//{
//	// imul  -> integer / pointer multiplication
//	// mulss -> f32 multiplication
//	// mulsd -> f64 multiplication
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	out << "    " << getTypedMul(lhs) << " ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(lhs))
//		emitOperandSize(lhs);
//
//	emitIROperand(lhs);
//	out << ", ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(rhs))
//		emitOperandSize(rhs);
//
//	emitIROperand(rhs);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitSignedDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	TypeKind sourceType = getTypeFromOperand(lhs);
//	int width = SemanticUtils::getIntegerWidth(sourceType);
//
//	emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rax), lhs);
//	switch (width)
//	{
//		case 2: out << "    cwd\n"; break;
//		case 4: out << "    cdq\n"; break;
//		case 8: out << "    cqo\n"; break;
//	}
//
//	emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rcx), rhs);
//	out << "    idiv ";
//	emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rcx), getTypeFromOperand(rhs));
//	out << "\n";
//	out << "    mov ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(destination))
//		emitOperandSize(destination);
//
//	emitIROperand(destination);
//
//	out << ", rax\n";
//}
//
//void x86_64Emitter::emitUnsignedDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	TypeKind sourceType = getTypeFromOperand(lhs);
//	int width = SemanticUtils::getIntegerWidth(sourceType);
//
//	emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rax), lhs);
//	switch (width)
//	{
//		case 2: out << "    xor dx, dx\n"; break;
//		case 4: out << "    xor edx, edx\n"; break;
//		case 8: out << "    xor rdx, rdx\n"; break;
//	}
//
//	emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rcx), rhs);
//	out << "    div ";
//	emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rcx), getTypeFromOperand(rhs));
//	out << "\n";
//	out << "    mov ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(destination))
//		emitOperandSize(destination);
//
//	emitIROperand(destination);
//
//	out << ", rax\n";
//}
//
//void x86_64Emitter::emitFloatDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(lhs);
//	TypeKind sourceType = getTypeFromOperand(lhs);
//	int width = SemanticUtils::getFloatWidth(sourceType);
//	
//	switch (width)
//	{
//		case 4: out << "    divss "; break;
//		case 8: out << "    divsd "; break;
//	}
//
//	emitIROperand(lhs);
//	out << ", ";
//	emitIROperand(rhs);
//	out << "\n";
//
//	emitMov(destination, lhs);
//}
//
//void x86_64Emitter::emitDiv(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	// signed division   -> idiv  -> use rax as dividend, then src2 is moved into reg -> div src2
//	// unsigned division -> div
//	// f32 division      -> divss
//	// f64 division      -> divsd
//	TypeKind sourceType = getTypeFromOperand(lhs);
//
//	if (SemanticUtils::isSigned(sourceType))
//		emitSignedDiv(destination, lhs, rhs);
//	else if (SemanticUtils::isUnsigned(sourceType))
//		emitUnsignedDiv(destination, lhs, rhs);
//	else
//		emitFloatDiv(destination, lhs, rhs);
//}
//
//void x86_64Emitter::emitShl(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	// can emit directly shl lhs, const without moving the shift amount into rcx and emitting shl lhs, cl
//	if (rhs.kind == ValueKind::Constant)
//	{
//		emitMov(destination, lhs);
//		out << "    shl ";
//		emitIROperand(destination);
//		out << ", ";
//		emitIROperand(rhs);
//		out << "\n";
//	}
//
//	// if shift amount is not a constant, we need to mov it into rcx and shift by cl via -> shl lhs, cl
//	else
//	{
//		emitMov(destination, lhs);
//		emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rcx), rhs);
//		out << "    shl ";
//		emitIROperand(destination);
//		out << ", cl\n";
//	}
//}
//
//void x86_64Emitter::emitShr(const IROperand& destination, const IROperand& lhs, const IROperand& rhs)
//{
//	// same operand logic as shl, just splitting it up because shr for signed integers should emit sar
//	bool signedShift = SemanticUtils::isSigned(getTypeFromOperand(lhs));
//	if (rhs.kind == ValueKind::Constant)
//	{
//		emitMov(destination, lhs);
//
//		if (signedShift)
//			out << "    sar ";
//		else
//			out << "    shr ";
//
//		emitIROperand(destination);
//		out << ", ";
//		emitIROperand(rhs);
//		out << "\n";
//	}
//
//	else
//	{
//		emitMov(destination, lhs);
//		emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rcx), rhs);
//
//		if (signedShift)
//			out << "    sar ";
//		else
//			out << "    shr ";
//
//		emitIROperand(destination);
//		out << ", cl\n";
//	}
//}
//
//void x86_64Emitter::emitRet(const IROperand& retVal, bool lastInstruction)
//{
//	emitHardCodedGPMov(static_cast<uint32_t>(X86GPRegister::rax), retVal);
//
//	// only emit jumps to the function epilogue label for returns that aren't that last instruction of the function
//	if (!lastInstruction)
//	{
//		out << "    jmp " << ".L" + std::to_string(currentFunction->funcID) << "_Epilogue\n";
//		needsEpilogueLabel = true;
//	}
//}
//
//void x86_64Emitter::emitBitOp(const TacInstruction& instruction)
//{
//	emitMov(instruction.dest, instruction.src1);
//
//	bool needsSizeSpecifier = instrNeedsSizeSpecifier(instruction.src1);
//
//	switch (instruction.op)
//	{
//		case TacOpcode::BitAnd: out << "    and "; break;
//		case TacOpcode::BitOr:  out << "    or "; break;
//		case TacOpcode::BitXor: out << "    xor "; break;
//	}
//
//	if (needsSizeSpecifier && operandIsMemLocation(instruction.dest))
//		emitOperandSize(instruction.dest);
//
//	emitIROperand(instruction.dest);
//	out << ", ";
//
//	if (needsSizeSpecifier && operandIsMemLocation(instruction.src2))
//		emitOperandSize(instruction.src2);
//
//	emitIROperand(instruction.src2);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitLabel(const IROperand& label)
//{
//	out << ".L" << label.id << ":\n";
//}
//
//void x86_64Emitter::emitCmpJmp(const IROperand& label, const IROperand& lhs, const IROperand& rhs, JumpKind jumpKind)
//{
//	// cond src1, src2, label, JumpKind -> cmp src1, src2 -> JumpKind label
//	TypeKind type = getTypeFromOperand(lhs);
//	if (SemanticUtils::isInteger(type) || type == TypeKind::Pointer || type == TypeKind::Char || type == TypeKind::Bool)
//	{
//		out << "    " << getTypedCmp(lhs) << " ";
//		emitIROperand(lhs);
//		out << ", ";
//		emitIROperand(rhs);
//		out << "\n";
//
//		out << "    " << getJumpKind(jumpKind, false) << " .L" << label.id << std::endl;
//	}
//	else if (SemanticUtils::isFloat(type))
//	{
//		out << "    " << getTypedCmp(lhs) << " ";
//		emitIROperand(lhs);
//		out << ", ";
//		emitIROperand(rhs);
//		out << "\n";
//
//		out << "    " << getJumpKind(jumpKind, true) << " .L" << label.id << std::endl;
//	}
//}
//
//void x86_64Emitter::emitJmp(const IROperand& label)
//{
//	out << "    jmp .L" << label.id << std::endl;
//}
//
//void x86_64Emitter::emitCmp(const TacInstruction& instruction)
//{
//	// cmp lhs, rhs
//	out << "    " << getTypedCmp(instruction.src1) << " "; 
//	emitIROperand(instruction.src1);
//	out << ", ";
//	emitIROperand(instruction.src2);
//	out << std::endl;
//
//	// set flag
//	out << "    " << getTypedSet(instruction.src1, instruction.op) << " ";
//
//	// the setcc result will be put into the destination register -> setne 'dest';
//	emitIROperand(instruction.dest);
//	out << std::endl;
//	// the result should be 1 byte, since its a boolean result, so we can pass in boolean to get the correct subregister access like r11b
//	//emitGPScratchRegister(TypeKind::Bool);
//}
//
//void x86_64Emitter::emitNeg(const IROperand& value)
//{
//	TypeKind type = getTypeFromOperand(value);
//
//	// signed / unsigned ints -> neg reg
//	if (SemanticUtils::isInteger(type))
//	{
//		out << "    neg ";
//		emitIROperand(value);
//		out << std::endl;
//	}
//
//	// neg isn't valid for float types, but we can flip the sign bit using xor and bitmask for negation
//	else if (SemanticUtils::isFloat(type))
//	{
//		out << "    xorps ";
//		emitIROperand(value);
//
//		if (type == TypeKind::f32)
//			out << ", [rip + f32_neg_mask]\n";
//		else
//			out << ", [rip + f64_neg_mask]\n";
//	}
//}
//
//void x86_64Emitter::emitBitNot(const IROperand& value)
//{
//	// only defined for signed / unsigned integers, this is already guaranteed by earlier compiler phases so no need to check here
//	out << "    not ";
//	emitIROperand(value);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitLogicalNot(const IROperand& value)
//{
//	out << "    cmp ";
//	emitIROperand(value);
//	out << ", 0";
//	out << "    sete ";
//	emitIROperand(value);
//	out << std::endl;
//}
//
//void x86_64Emitter::emitCondJmp(const IROperand& value, const IROperand& label, TacOpcode opcode)
//{
//	JumpKind jumpKind = opcode == TacOpcode::JmpIfFalse ? JumpKind::Je : JumpKind::Jne;
//
//	// cmp bool, 0
//	out << "    cmp ";
//	emitIROperand(value);
//	out << ", 0\n";
//	
//	out << "    " << getJumpKind(jumpKind, false) << " .L" << label.id << std::endl;
//}
//
//void x86_64Emitter::emitArg(const IROperand& arg)
//{
//	funcArgs.push_back(arg);
//	//out << "    push ";
//	//emitIROperand(arg);
//	//out << std::endl;
//}
//
//void x86_64Emitter::emitGPArg(const IROperand& arg, uint32_t argCount)
//{
//	// need to spill to stack
//	if (argCount >= RESERVED_GP_PARAM_REGISTERS)
//	{
//		uint32_t stackOffset = (8 * argCount);
//
//		// if arg is a memory location (like a variable), then we need to emit it into the scratch register then do the mov
//		// since we know that destination is a memory location since we're spilling args here
//		if (operandIsMemLocation(arg))
//		{
//			// mov arg into scratch reg
//			TypeKind argType = getTypeFromOperand(arg);
//			out << "    mov ";
//			emitGPScratchRegister(argType);
//			out << ", ";
//			emitIROperand(arg);
//			out << std::endl;
//
//			// mov scratch reg into stack offset
//			out << "    mov ";
//			emitOperandSize(arg);
//			out << "[rsp + " << stackOffset << "], ";
//			emitGPScratchRegister(argType);
//			out << std::endl;
//		}
//
//		// otherwise, if arg is a register, we can directly move it into the stack offset
//		else
//		{
//			out << "    mov ";
//			emitOperandSize(arg);
//			out << "[rsp + " << stackOffset << "], ";
//			emitIROperand(arg);
//			out << std::endl;
//		}
//		//out << "    mov ";
//		//emitOperandSize(arg);
//		//out << "[rsp + " << stackOffset << "], ";
//		//emitIROperand(arg);
//		//out << std::endl;
//	}
//
//	// push into gp register order of args -> rcx, rdx, r8, r9
//	else
//	{
//		switch (argCount)
//		{
//			
//			case 0: emitHardCodedGPMov(static_cast<size_t>(X86GPRegister::rcx), arg); break;
//			case 1: emitHardCodedGPMov(static_cast<size_t>(X86GPRegister::rdx), arg); break;
//			case 2: emitHardCodedGPMov(static_cast<size_t>(X86GPRegister::r8), arg); break;
//			case 3: emitHardCodedGPMov(static_cast<size_t>(X86GPRegister::r9), arg); break;
//		}
//	}
//}
//
//void x86_64Emitter::emitCall(const IROperand& ret, const IROperand& func)
//{
//	IRFunction* function = data->functionTable[func.id];
//
//	size_t gpArgCount = 0;
//	size_t fpArgCount = 0;
//
//	// 32 by default for shadow space
//	std::cout << "extra param space at time of call site = " << function->extraParamSpace << std::endl;
//	uint32_t stackSpace = TypeUtils::alignTo(32 + function->extraParamSpace, 16);
//
//	out << "    sub rsp, " << stackSpace << std::endl;
//
//	gpArgCount = 0;
//	fpArgCount = 0;
//
//	// now move args into registers / put on stack if needed
//	for (size_t i = 0; i < funcArgs.size(); i++)
//	{
//		TypeKind argType = getTypeFromOperand(funcArgs[i]);
//
//		if (typeGoesInGPRegister(argType))
//		{
//			emitGPArg(funcArgs[i], gpArgCount);
//			gpArgCount++;
//		}
//		else
//		{
//			fpArgCount++;
//		}
//	}
//
//	out << "    call " << function->name << std::endl;
//	out << "    add rsp, " << stackSpace << std::endl;
//
//	out << "    mov ";
//	if (operandIsMemLocation(ret) && instrNeedsSizeSpecifier(ret))
//		emitOperandSize(ret);
//
//	emitIROperand(ret);
//
//	out << ", ";
//	emitRegisterSize(static_cast<uint32_t>(X86GPRegister::rax), getTypeFromOperand(ret));
//	out << std::endl;
//}
//
//void x86_64Emitter::emitInstruction(const TacInstruction& instruction)
//{
//	switch (instruction.op)
//	{
//		// Not needed
//		//case TacOpcode::LogicalAnd:
//		//case TacOpcode::LogicalOr:
//
//		case TacOpcode::Address:      
//		case TacOpcode::Dereference:   
//		case TacOpcode::FieldAddr:    
//		case TacOpcode::CallVoid:     
//		case TacOpcode::LoadArrElem:  
//		case TacOpcode::StoreArrElem: 
//		case TacOpcode::Mod:           
//		{
//			out << "    unimplemented instruction " << DebugUtils::tacOpcodeToString(instruction.op) << std::endl;
//			break;
//		}
//
//		// this might be able to be commented out
//		case TacOpcode::Constant:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			break;
//		}
//		case TacOpcode::Move:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			break;
//		}
//		case TacOpcode::Store:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			break;
//		}
//
//		case TacOpcode::Load:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			break;
//		}
//
//		case TacOpcode::Add:
//		{
//			// move src1 into destination, then add src2 to destination
//			emitMov(instruction.dest, instruction.src1);
//			emitAdd(instruction.dest, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::Sub:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			emitSub(instruction.dest, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::Mul:
//		{
//			emitMov(instruction.dest, instruction.src1);
//			emitMul(instruction.dest, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::Div:
//		{
//			emitDiv(instruction.dest, instruction.src1, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::BitOr:
//		case TacOpcode::BitXor:
//		case TacOpcode::BitAnd:
//		{
//			emitBitOp(instruction);
//			break;
//		}
//
//		case TacOpcode::BitShiftLeft:
//		{
//			emitShl(instruction.dest, instruction.src1, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::BitShiftRight:
//		{
//			emitShr(instruction.dest, instruction.src1, instruction.src2);
//			break;
//		}
//
//		case TacOpcode::Label:
//		{
//			emitLabel(instruction.dest);
//			break;
//		}
//
//		case TacOpcode::LoweredCmpJmp:
//		{
//			emitCmpJmp(instruction.dest, instruction.src1, instruction.src2, instruction.jumpKind);
//			break;
//		}
//
//		case TacOpcode::Jmp:
//		{
//			emitJmp(instruction.dest);
//			break;
//		}
//
//		case TacOpcode::Eq:
//		case TacOpcode::Gt:
//		case TacOpcode::Gte:
//		case TacOpcode::Lt:
//		case TacOpcode::Lte:
//		case TacOpcode::NotEq:
//		{
//			emitCmp(instruction);
//			break;
//		}
//
//		case TacOpcode::Neg:
//		{
//			emitNeg(instruction.src1);
//			break;
//		}
//
//		case TacOpcode::BitNot:
//		{
//			emitBitNot(instruction.src1);
//			break;
//		}
//
//		case TacOpcode::Not:
//		{
//			emitBitNot(instruction.src1);
//			break;
//		}
//
//		case TacOpcode::JmpIfTrue:
//		case TacOpcode::JmpIfFalse:
//		{
//			emitCondJmp(instruction.src1, instruction.dest, instruction.op);
//			break;
//		}
//
//		case TacOpcode::FuncArg:
//		{
//			emitArg(instruction.dest);
//			break;
//		}
//
//		case TacOpcode::Call:
//		{
//			emitCall(instruction.dest, instruction.src1);
//			break;
//		}
//
//		case TacOpcode::Return:
//		{
//			emitRet(instruction.dest, lastInstructionOfFunction(instruction));
//			break;
//		}
//	}
//}