#include "environment.h"

#include <iostream>

Environment::Environment()
{
	scopeDepth = 0;
	scopes.emplace_back();	// emplace the 'global' scope, which is always the scope at index 0 in the scopes vector
}

void Environment::pushScope()
{
	scopes.emplace_back();
	scopeDepth++;
}

void Environment::popScope()
{
	if (scopeDepth == 0)
		return;

	scopes.pop_back();
	scopeDepth--;
}

void Environment::dumpEnvironment()
{
	std::cout << "\n\n===== Dumping Environment =====\n";
	for (size_t i = 0; i < scopes.size(); i++)
	{
		std::cout << "Scope depth " << i << std::endl;
		scopes[i].dumpSymbolTable();
	}
}