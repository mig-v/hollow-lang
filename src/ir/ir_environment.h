#pragma once

#include "ir/ir_core.h"

#include <vector>
#include <unordered_map>
#include <string>

class IREnvironment
{
public:
	IREnvironment();

	void pushScope();
	void popScope();

	IRSymbol* set(const std::string& identifier, uint32_t id); // creates and returns an IRSymbol
	IRSymbol* get(const std::string& identifier);
private:
	std::vector<std::unordered_map<std::string, IRSymbol>> scopes;
	std::vector<uint32_t> stackOffsets;
};