#include "ir/ir_environment.h"

IREnvironment::IREnvironment()
{
	// push the global scope of the environment, always will be index 0 and will never be popped
	pushScope();
}

void IREnvironment::pushScope()
{
	uint32_t accumulatedStackOffset = stackOffsets.size() > 0 ? stackOffsets.back() : 0;
	scopes.push_back(std::unordered_map<std::string, IRSymbol>());
	stackOffsets.push_back(accumulatedStackOffset);
}

void IREnvironment::popScope()
{
	if (!scopes.empty())
	{
		scopes.pop_back();
		stackOffsets.pop_back();
	}
}

IRSymbol* IREnvironment::set(const std::string& identifier, uint32_t id)
{
	IRSymbol& symbol = scopes.back()[identifier];
	symbol.id = id;
	
	// this needs to be more sophisticated later on for arrays and structs, but for now this will work for primitive
	symbol.stackOffset = stackOffsets.back();
	stackOffsets.back() += 8;
	return &symbol;
}

IRSymbol* IREnvironment::get(const std::string& identifier)
{
	// iterate through the scopes in reverse, innermost scopes take priority when looking for identifiers
	for (auto it = scopes.rbegin(); it != scopes.rend(); it++)
	{
		auto entry = it->find(identifier);
		if (entry != it->end())
			return &entry->second;
	}

	// logically, this should never be reached since semantic analysis guarantees every symbol referenced exists
	return 0;
}
