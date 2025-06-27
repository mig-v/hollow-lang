#pragma once

#include <string>
#include <vector>

struct StructMember;

enum class TypeKind
{
	// built ins
	u8, u16, u32, u64, i8, i16, i32, i64, f32, f64, Char, Bool, Void, Function,
	
	// user defined structs
	Struct,

	// for error handling
	Unknown,
	TypeCount
};

struct TypeInfo
{
	TypeKind type;		// store the type kind
	std::string name;	// store the name of the type (only really used for function / variables since this'll store their identifiers)

	// struct data
	std::vector<StructMember> structMembers;

	// function data
	std::vector<TypeInfo*> paramTypes;
	TypeInfo* returnType;
};

struct StructMember
{
	std::string name;	// name of the member
	TypeInfo* type;		// type info of the member
};