#pragma once

#include "core/type_info.h"

namespace TypeUtils
{
	uint64_t calculateSlotCountForElement(TypeInfo* elementType);
	size_t alignTo(size_t offset, size_t alignment);
}