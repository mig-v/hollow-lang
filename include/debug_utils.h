#pragma once

#include "type_info.h"
#include "token.h"
#include "diagnostic.h"
#include "l_value.h"

namespace DebugUtils
{
	const char* typeKindToString(TypeKind type);
	const char* tokenTypeToString(TokenType type);
	const char* lValueKindToString(LValueKind kind);
}