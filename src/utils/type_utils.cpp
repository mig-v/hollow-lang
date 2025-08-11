#include "utils/type_utils.h"

namespace TypeUtils
{
	uint64_t calculateSlotCountForElement(TypeInfo* elementType)
	{
		switch (elementType->type)
		{
			case TypeKind::Void:
				return 0;
			case TypeKind::i8:
			case TypeKind::u8:
			case TypeKind::Char:
			case TypeKind::Bool:
				return 1;
			case TypeKind::i16:
			case TypeKind::u16:
				return 1;
			case TypeKind::i32:
			case TypeKind::u32:
			case TypeKind::f32:
				return 1;
			case TypeKind::i64:
			case TypeKind::u64:
			case TypeKind::f64:
				return 1;
			case TypeKind::Array:
				return elementType->arrayLength * calculateSlotCountForElement(elementType->elementType);
			case TypeKind::Pointer:
				return 1;
		}
	}

	size_t alignTo(size_t offset, size_t alignment)
	{
		return (offset + alignment - 1) & ~(alignment - 1);
	}
}