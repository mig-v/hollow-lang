#pragma once

#include "core/type_info.h"

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
	bool isParameter;
};

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable() = default;

	inline int getSlotOffset() const { return nextSlot + parentScopeSlotOffset; }
	void setSlotOffset(int offset);

	int declareVar(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth);
	void declareFunc(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth);
	bool isDefined(const std::string& identifier);
	void dumpSymbolTable();
	Symbol* getSymbol(const std::string& identifier);

private:
	int getSlotCountForType(TypeInfo* typeInfo);

	std::unordered_map<std::string, Symbol> symbols;
	int nextSlot;
	int parentScopeSlotOffset;
};