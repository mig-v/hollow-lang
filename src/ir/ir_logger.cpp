#include "ir/ir_logger.h"

#include "utils/debug_utils.h"

#include <filesystem>

void IRLogger::log(IRData* data, const std::string& path)
{
	this->data = data;

	std::filesystem::path logPath(path);
	std::filesystem::create_directories(logPath.parent_path());
	out.open(logPath);

	if (!out.is_open())
		return;

	for (IRFunction* function : data->functionTable)
		logIRFunction(function);

	out.close();
}

void IRLogger::logIRFunction(IRFunction* function)
{
	out << "function " << function->name << ": (param count = " << function->paramIDs.size() << " local count = " << function->localIDs.size() << ")\n";
	if (function->body.size() == 0)
		out << "    <empty body>\n";

	for (const TacInstruction& instruction : function->body)
		logIRInstruction(instruction);

	out << std::endl;
}

void IRLogger::logIRInstruction(const TacInstruction& instruction)
{
	switch (instruction.op)
	{
		case TacOpcode::Move:
			out << "    move " << irOperandToString(instruction.dest) << ", " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Store:
			out << "    store " << irOperandToString(instruction.dest) << ", " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Constant:
			out << "    " << (std::string("t") + std::to_string(instruction.dest.id)) << " = " << irOperandToString(instruction.dest) << std::endl;
			break;
		case TacOpcode::Load:
			out << "    " << irOperandToString(instruction.dest) << " = load " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Address:
			out << "    " << irOperandToString(instruction.dest) << " = addr " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Dereference:
			out << "    " << irOperandToString(instruction.dest) << " = deref " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::FieldAddr:
			out << "    " << irOperandToString(instruction.dest) << " = field_addr " << irOperandToString(instruction.src1) << ", offset = " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Add:
			out << "    " << irOperandToString(instruction.dest) << " = add " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Sub:
			out << "    " << irOperandToString(instruction.dest) << " = sub " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Mul:
			out << "    " << irOperandToString(instruction.dest) << " = mul " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Div:
			out << "    " << irOperandToString(instruction.dest) << " = div " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::BitAnd:
			out << "    " << irOperandToString(instruction.dest) << " = bitand " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::BitOr:
			out << "    " << irOperandToString(instruction.dest) << " = bitor " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::BitXor:
			out << "    " << irOperandToString(instruction.dest) << " = bitxor " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::BitShiftLeft:
			out << "    " << irOperandToString(instruction.dest) << " = shl " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::BitShiftRight:
			out << "    " << irOperandToString(instruction.dest) << " = shr " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::NotEq:
			out << "    " << irOperandToString(instruction.dest) << " = neq " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Eq:
			out << "    " << irOperandToString(instruction.dest) << " = eq " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Gt:
			out << "    " << irOperandToString(instruction.dest) << " = gt " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Gte:
			out << "    " << irOperandToString(instruction.dest) << " = gte " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Lt:
			out << "    " << irOperandToString(instruction.dest) << " = lt " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Lte:
			out << "    " << irOperandToString(instruction.dest) << " = lte " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Label:
			out << (std::string(".L" + std::to_string(instruction.dest.id))) << ":" << std::endl;
			break;
		case TacOpcode::JmpIfFalse:
			out << "    jmp_if_false " << (std::string(".L_" + std::to_string(instruction.dest.id))) << ", " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::JmpIfTrue:
			out << "    jmp_if_true " << (std::string(".L_" + std::to_string(instruction.dest.id))) << ", " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Jmp:
			out << "    jmp " << (std::string(".L_" + std::to_string(instruction.dest.id))) << std::endl;
			break;
		case TacOpcode::LoweredCmpJmp:
			out << "    cond " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << " " << DebugUtils::jumpKindToString(instruction.jumpKind) << " " << (std::string(".L_" + std::to_string(instruction.dest.id))) << std::endl;
			break;
		case TacOpcode::Call:
			out << "    " << (std::string("t") + std::to_string(instruction.dest.id)) << " = call " << data->functionTable[instruction.src1.id]->name << std::endl;
			break;
		case TacOpcode::CallVoid:
			out << "    call " << data->functionTable[instruction.dest.id]->name << std::endl;
			break;
		case TacOpcode::FuncArg:
			out << "    arg " << irOperandToString(instruction.dest) << std::endl;
			break;
		case TacOpcode::Return:
			out << "    return " << irOperandToString(instruction.dest) << std::endl;
			break;
		case TacOpcode::LoadArrElem:
			out << "    " << (std::string("t") + std::to_string(instruction.dest.id)) << " = load elem " << (std::string("t") + std::to_string(instruction.src1.id)) << "[" << (std::string("t") + std::to_string(instruction.src2.id)) << "]" << std::endl;
			break;
		case TacOpcode::Neg:
			out << "    " << irOperandToString(instruction.dest) << " = " << "neg " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::Not:
			out << "    " << irOperandToString(instruction.dest) << " = " << "not " << irOperandToString(instruction.src1) << std::endl;
			break;
		case TacOpcode::LogicalAnd:
			out << "    " << irOperandToString(instruction.dest) << " = logical_and " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::LogicalOr:
			out << "    " << irOperandToString(instruction.dest) << " = logical_or " << irOperandToString(instruction.src1) << ", " << irOperandToString(instruction.src2) << std::endl;
			break;
		case TacOpcode::Dead:
			out << "    dead" << std::endl;
			break;
		default:
			out << "Unsupported Tac Opcode\n";
	}
}

std::string IRLogger::irOperandToString(const IROperand& value)
{
	switch (value.kind)
	{
	case ValueKind::Constant:
		return  "const " + irConstToString(value);
	case ValueKind::Temporary:
		return "t" + std::to_string(value.id);
	case ValueKind::Variable:
		return std::get<std::string>(data->values[value.id].data);
	default:
		return "Unimplemented ValueKind " + std::to_string(static_cast<int>(value.kind));
	}
}

std::string IRLogger::irConstToString(const IROperand& value)
{
	switch (value.type)
	{
	case TypeKind::i8:
	case TypeKind::i16:
	case TypeKind::i32:
	case TypeKind::i64:
		return std::to_string(std::get<int64_t>(value.data));
	case TypeKind::u8:
	case TypeKind::u16:
	case TypeKind::u32:
	case TypeKind::u64:
		return std::to_string(std::get<uint64_t>(value.data));
	case TypeKind::Bool:
		return std::to_string(std::get<bool>(value.data));
	case TypeKind::f32:
	case TypeKind::f64:
		return std::to_string(std::get<double>(value.data));
	default:
		return "Unsupported Tac Value Type";
	}
}