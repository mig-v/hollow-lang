#pragma once

#include "semantics/symbol_table.h"

#include <vector>

class Environment
{
public:
	Environment(SymbolTable* globalScope);
	inline SymbolTable* getCurrentScope() { return scopes.back(); }
	inline SymbolTable* getGlobalScope() { return globalScope; }
	inline int getScopeDepth() const { return scopeDepth; }

	void pushScope(SymbolTable* table, ScopeKind kind);
	void popScope();

	Symbol* findSymbol(const std::string& identifier);
	void dumpEnvironment();

private:

	std::vector<SymbolTable*> scopes;
	SymbolTable* globalScope;
	int scopeDepth;
};