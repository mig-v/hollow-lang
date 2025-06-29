#pragma once

#include "type_info.h"

#include <unordered_map>
#include <string>

enum class ScopeKind
{
	Normal,
	Function
};

struct Symbol
{
	int scope;
	int slotIndex;
	TypeInfo* typeInfo;
};

class SymbolTable
{
public:
	SymbolTable(int parentScopeSlotOffset);
	~SymbolTable() = default;

	inline int getSlotOffset() const { return nextSlot + parentScopeSlotOffset; }

	int declareVar(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth);
	void declareFunc(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth);
	bool isDefined(const std::string& identifier);
	void dumpSymbolTable();
	Symbol* getSymbol(const std::string& identifier);

private:
	std::unordered_map<std::string, Symbol> symbols;
	int nextSlot;
	int parentScopeSlotOffset;
};