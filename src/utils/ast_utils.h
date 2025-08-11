#pragma once

#include "parser/ast_node.h"
#include "parser/ast_expr.h"
#include "parser/ast_stmt.h"

namespace ASTUtils
{
	ASTExpr* unwrapGroupExpr(ASTExpr* expr);
}