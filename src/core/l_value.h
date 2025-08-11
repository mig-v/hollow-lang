#pragma once

#include "semantics/symbol_table.h"

enum class LValueKind
{
	Slot,        // simple variables, like x : i32, etc.
	Indirect,    // indirect accesses like arr[0] or *ptr
	StructField, // accessing a structs field
	Invalid
};

struct LValue
{
	LValueKind kind = LValueKind::Invalid;

	// for slot, holds scope / slot index information
	Symbol* symbol = nullptr;

	// add more as needed
};