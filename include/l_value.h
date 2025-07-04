#pragma once

enum class LValueKind
{
	Slot,
	Pointer,
	ArrayElement,
	StructField,
	Invalid
};

struct LValue
{
	LValueKind kind = LValueKind::Invalid;
	int slotIndex   = 0;  // for slot
	int scope       = -1; 
	int fieldOffset = 0;  // for structs
	int elementSize = 0;  // for arrays
};