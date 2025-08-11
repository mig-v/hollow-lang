#pragma once

#include "archive/register_allocation_core.h"

#include "core/type_info.h"
#include "core/diagnostic.h"
#include "core/l_value.h"

#include "ir/ir_core.h"

#include "lexer/token.h"

namespace DebugUtils
{
	const char* typeKindToString(TypeKind type);
	const char* tokenTypeToString(TokenType type);
	const char* lValueKindToString(LValueKind kind);
	const char* tacOpcodeToString(TacOpcode opcode);
	const char* jumpKindToString(JumpKind kind);
}