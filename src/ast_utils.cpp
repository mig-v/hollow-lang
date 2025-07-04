#include "ast_utils.h"

namespace ASTUtils
{
	ASTExpr* unwrapGroupExpr(ASTExpr* expr)
	{
		while (expr->isGroupExpr)
		{
			ASTGroupExpr* groupExpr = static_cast<ASTGroupExpr*>(expr);
			expr = groupExpr->expr;
		}

		return expr;
	}
}