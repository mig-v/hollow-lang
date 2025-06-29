#include "environment.h"

#include <iostream>

Environment::Environment()
{
	scopeDepth = 0;
	scopes.emplace_back(SymbolTable{0});	// emplace the 'global' scope, which is always the scope at index 0 in the scopes vector
}

void Environment::pushScope(ScopeKind kind)
{
	// slot offset has to be the parent scopes slotIndex + ITS parent scope slot offset
	// but only if the scope is NORMAL, if it's a FUNCTION, then we need to reset slot counting from 0,
	// so we pass in an offset of 0. This is because when a function is called, a stack frame is pushed
	// resetting slot indices back to 0. This will not interfere with prev scopes / global scope
	int parentScopeSlotOffset = kind == ScopeKind::Normal ? scopes.back().getSlotOffset() : 0;
	scopes.emplace_back(SymbolTable{parentScopeSlotOffset});
	scopeDepth++;
}

void Environment::popScope()
{
	if (scopeDepth == 0)
		return;

	scopes.pop_back();
	scopeDepth--;
}

Symbol* Environment::findSymbol(const std::string& identifier)
{
	// iterate through the scopes in reverse, innermost scopes take priority when looking for identifiers
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		if (it->isDefined(identifier))
			return it->getSymbol(identifier);
	}

	return nullptr;
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