#include "symbol_table.h"
#include "debug_utils.h"

#include <iostream>

SymbolTable::SymbolTable(int parentScopeSlotOffset)
{
	std::cout << "making symbol table with parent offset = " << parentScopeSlotOffset << std::endl;
	this->nextSlot = 0;
	this->parentScopeSlotOffset = parentScopeSlotOffset;
}

int SymbolTable::declareVar(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth)
{
	int slot = nextSlot++;
	slot += parentScopeSlotOffset;
	Symbol symbol = { scopeDepth, slot, typeInfo };
	symbols[identifier] = symbol;
	return symbol.slotIndex;
}

void SymbolTable::declareFunc(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth)
{
	Symbol symbol{ scopeDepth, -1, typeInfo };
	symbols[identifier] = symbol;
}

bool SymbolTable::isDefined(const std::string& identifier)
{
	return (symbols.find(identifier) != symbols.end());
}

Symbol* SymbolTable::getSymbol(const std::string& identifier)
{
	return &symbols[identifier];
}

void SymbolTable::dumpSymbolTable()
{
	for (auto& symbol : symbols)
	{
		Symbol& sym = symbol.second;
		std::cout << symbol.first << " : " << DebugUtils::typeKindToString(sym.typeInfo->type) << " [scope = " << sym.scope << ", slot index = " << sym.slotIndex << "]\n";
	}
}