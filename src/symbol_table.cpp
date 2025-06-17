#include "symbol_table.h"
#include "debug_utils.h"

#include <iostream>

SymbolTable::SymbolTable()
{
	this->nextSlot = 0;
}

int SymbolTable::declare(const std::string identifier, TypeInfo* typeInfo, int scopeDepth)
{
	Symbol symbol = { scopeDepth, nextSlot++, typeInfo };
	symbols[identifier] = symbol;
	return symbol.slotIndex;
}

bool SymbolTable::isDefined(const std::string identifier)
{
	return (symbols.find(identifier) != symbols.end());
}

void SymbolTable::dumpSymbolTable()
{
	for (auto& symbol : symbols)
	{
		Symbol& sym = symbol.second;
		std::cout << symbol.first << " : " << DebugUtils::typeKindToString(sym.type->type) << " [scope = " << sym.scope << ", slot index = " << sym.slotIndex << "]\n";
	}
}