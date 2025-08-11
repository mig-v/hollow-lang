#pragma once

#include "core/type_info.h"

#include "archive/register.h"

#include <variant>
#include <unordered_map>

enum class TacOpcode
{
	Constant,	 // solely used for constants
	Move,		 // used for temporaries
	Store,		 // used for variables, or anything with a memory address
	Load,		 // for loading the value of a variable into a temporary
	Address,	 // taking the address of a variable
	Dereference, // dereferencing a variable
	FieldAddr,   // address of a field -> foo.x, evals to address of x -> [TacOpcode::FieldAddr, fieldID, structID, offset]

	FuncArg, Call, CallVoid, Return, LoadArrElem, StoreArrElem,
	
	// binary ops
	Add, Sub, Mul, Div, Mod, BitAnd, BitOr, BitXor, BitShiftLeft, BitShiftRight, NotEq, Eq, Gt, Gte, Lt, Lte,

	// logical ops
	LogicalAnd, LogicalOr,
	
	// control flow ops
	Jmp, JmpIfTrue, JmpIfFalse, Label, LoweredCmpJmp,

	// unary ops
	Neg, Not, BitNot,

	// optimization passes may mark instructions as dead by changing their opcode, this lets DCE cleanly remove dead code
	Dead,
	None
};

enum class ValueKind
{
	Constant, Temporary, Variable, Label, FuncID, Param, None
};

enum class EvalMode
{
	LValue, RValue
};

enum class JumpKind
{
	Je, Jne, Jg, Jge, Jl, Jle
};

struct IRSymbol
{
	uint32_t stackOffset;
	uint32_t id;
};

struct IROperand
{
	ValueKind kind;
	TypeKind type;

	// if kind == Temporary or Variable, id is used, then the value is accessed via this ID
	uint32_t id;

	// certain constants are pooled and stored in the asm read only section such as f32 and f64 since they cant be used as immediates in asm
	uint32_t constPoolID;

	// if kind == Constant, we don't use the values table, but instead immediately store the constant value on the operand
	std::variant<std::string, int64_t, uint64_t, double, char, bool> data;
};

struct TacValue
{
	TypeInfo* typeInfo;		// full type info for variables
	TypeKind temporaryType;	// type of a temporary, since they dont need a full blown typeInfo
	ValueKind kind;
	uint32_t id;

	// std::string holds identifiers for variables, all other data types store the value for constants
	std::variant<std::string, int64_t, uint64_t, double, char, bool> data;
};

struct TacInstruction
{
	TacOpcode op;
	IROperand dest;
	IROperand src1;
	IROperand src2;
	JumpKind jumpKind;	// only used in code emission after lowering conditionals
};

struct IRConditionContext
{
	IROperand falseLabel;	// where to jump to if condition is false
	IROperand trueLabel;	    // where to jump to if condition is true
	IROperand endLabel;	    // end of condition construct, instruction immediately after while loop, if statement, etc.
};

struct IRFunction
{
	std::string name;
	std::vector<uint32_t> paramIDs;
	std::vector<uint32_t> localIDs;
	std::vector<TacInstruction> body;
	TypeInfo* returnType;
	uint32_t funcID;
};

struct IRData
{
	std::vector<TacValue> values;

	// kinda overly complicated, but I want the functions to be in order for printing purposes when debugging, so I want to store them
	// in order in a vector but still be able to look them up in a map by name. So this works as func identifier -> func id -> IRFunction
	std::unordered_map<std::string, uint32_t> funcToId;
	std::vector<IRFunction*> functionTable;
};