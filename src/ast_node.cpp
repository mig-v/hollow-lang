#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "ast_printer.h"
#include "semantic_analysis.h"

#include <iostream>

ASTParameter::ASTParameter(Token paramIdentifier, Token paramType)
{
	this->paramIdentifier = paramIdentifier;
	this->paramType = paramType;
	this->typeInfo = nullptr;
}

bool ASTParameter::operator==(const ASTNode& other) const
{
	if (const ASTParameter* node = dynamic_cast<const ASTParameter*>(&other))
	{
		// NOTE: This .type comparison will only work for built in types. Once user types are added their token string variant value will need
		//       to be compared
		return ((node->paramType.type == this->paramType.type)
			&& (std::get<std::string>(node->paramIdentifier.value) == std::get<std::string>(this->paramIdentifier.value)));
	}

	return false;
}

void ASTParameter::accept(ASTPrinter visitor)
{
	visitor.visitParameter(*this);
}

void ASTParameter::accept(SemanticAnalysis& visitor)
{
	visitor.visitParameter(*this);
}

bool ASTParamList::operator==(const ASTNode& other) const
{
	if (const ASTParamList* node = dynamic_cast<const ASTParamList*>(&other))
	{
		if (node->params.size() != this->params.size())
			return false;

		for (size_t i = 0; i < this->params.size(); i++)
		{
			if (!(astEqual(node->params[i], this->params[i])))
				return false;
		}

		return true;
	}
	
	return false;
}

void ASTParamList::accept(ASTPrinter visitor)
{
	visitor.visitParamList(*this);
}

void ASTParamList::accept(SemanticAnalysis& visitor)
{
	visitor.visitParamList(*this);
}

bool ASTArgList::operator==(const ASTNode& other) const
{
	if (const ASTArgList* node = dynamic_cast<const ASTArgList*>(&other))
	{
		if (node->args.size() != this->args.size())
			return false;

		for (size_t i = 0; i < this->args.size(); i++)
		{
			if (!(astEqual(node->args[i], this->args[i])))
				return false;
		}

		return true;
	}

	return false;
}

void ASTArgList::accept(ASTPrinter visitor)
{
	visitor.visitArgList(*this);
}

void ASTArgList::accept(SemanticAnalysis& visitor)
{
	visitor.visitArgList(*this);
}