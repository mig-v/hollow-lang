#pragma once

#include "type_info.h"
#include "token.h"
#include "diagnostic.h"

namespace DebugUtils
{
	const char* typeKindToString(TypeKind type);
	const char* tokenTypeToString(const Token& token);
}