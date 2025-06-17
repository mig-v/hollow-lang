#include "debug_utils.h"

namespace DebugUtils
{
	const char* typeKindToString(TypeKind type)
	{
		switch (type)
		{
			case TypeKind::u8:   return "u8";
			case TypeKind::u16:  return "u16";
			case TypeKind::u32:  return "u32";
			case TypeKind::u64:  return "u64";
			case TypeKind::i8:   return "i8";
			case TypeKind::i16:  return "i16";
			case TypeKind::i32:  return "i32";
			case TypeKind::i64:  return "i64";
			case TypeKind::f32:  return "f32";
			case TypeKind::f64:  return "f64";
			case TypeKind::Char: return "char";
			case TypeKind::Bool: return "bool";
			case TypeKind::Void: return "void";
			default:
				return "unknown";
		}
	}
}