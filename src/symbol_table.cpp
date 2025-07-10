#include "symbol_table.h"
#include "debug_utils.h"

#include <iostream>

SymbolTable::SymbolTable()
{
	this->nextSlot = 0;
	this->parentScopeSlotOffset = parentScopeSlotOffset;
}

void SymbolTable::setSlotOffset(int offset)
{
	this->parentScopeSlotOffset = offset;
}

int SymbolTable::getSlotCountForType(TypeInfo* typeInfo)
{
	switch (typeInfo->type)
	{
	case TypeKind::u8:
	case TypeKind::u16:
	case TypeKind::u32:
	case TypeKind::u64:
	case TypeKind::i8:
	case TypeKind::i16:
	case TypeKind::i32:
	case TypeKind::i64:
	case TypeKind::f32:
	case TypeKind::f64:
	case TypeKind::Bool:
	case TypeKind::Char:
		return 1;
	case TypeKind::Array:
		return typeInfo->arrayLength * getSlotCountForType(typeInfo->elementType);
	}
}

int SymbolTable::declareVar(const std::string& identifier, TypeInfo* typeInfo, int scopeDepth)
{
	int slot = nextSlot;
	nextSlot += getSlotCountForType(typeInfo);
	slot += parentScopeSlotOffset;
	Symbol symbol = { scopeDepth, slot, typeInfo, false };
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

		if (sym.typeInfo->type == TypeKind::Function)
		{
			std::cout << symbol.first << " : " << DebugUtils::typeKindToString(sym.typeInfo->type) << " [scope = " << sym.scope << ", parameters -> [";

			for (size_t i = 0; i < sym.typeInfo->paramTypes.size(); i++)
			{
				std::cout << sym.typeInfo->paramTypes[i]->name << " : " << DebugUtils::typeKindToString(sym.typeInfo->paramTypes[i]->type);
				if (i + 1 < sym.typeInfo->paramTypes.size())
					std::cout << ", ";
			}

			std::cout << "]]\n";
		}
		else
			std::cout << symbol.first << " : " << DebugUtils::typeKindToString(sym.typeInfo->type) << " [scope = " << sym.scope << ", slot index = " << sym.slotIndex << "]\n";
	}
}