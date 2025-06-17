#pragma once

#include "type_info.h"

#include <unordered_map>
#include <string>

struct Symbol
{
	int scope;
	int slotIndex;
	TypeInfo* type;
};

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable() = default;

	int declare(const std::string identifier, TypeInfo* typeInfo, int scopeDepth);
	bool isDefined(const std::string identifier);
	void dumpSymbolTable();

private:
	std::unordered_map<std::string, Symbol> symbols;
	int nextSlot;
};