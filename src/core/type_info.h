#pragma once

#include <string>
#include <vector>

enum class TypeKind
{
	// built ins
	u8, u16, u32, u64, i8, i16, i32, i64, f32, f64, Char, Bool, Void, Function, Array, Pointer,
	
	// user defined structs
	Struct,

	// for error handling
	Unknown,
	TypeCount
};

struct TypeInfo
{
	TypeKind type;		// store the type kind
	std::string name;	// store the name of the type (variables -> var identifier, function -> func identifier)

	// struct data
	std::string typeName;

	// pointer data
	TypeInfo* ptrBaseType;
	uint8_t indirectionLevel = 0;

	// function data
	std::vector<TypeInfo*> paramTypes;
	TypeInfo* returnType;
	uint32_t localsCount;

	// array data
	TypeInfo* elementType; 
	uint64_t arrayLength;  
	uint64_t elementSize;         // size of each element of the array in bytes
	uint64_t slotCountPerElement; // the number of slots each element takes up
	uint8_t dimensionCount = 0;
};
