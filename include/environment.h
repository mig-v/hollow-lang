#pragma once

#include "symbol_table.h"
#include <vector>

class Environment
{
public:
	Environment();
	inline SymbolTable& getCurrentScope() { return scopes.back(); }
	inline int getScopeDepth() const { return scopeDepth; }
	void pushScope();
	void popScope();
	void dumpEnvironment();

private:

	std::vector<SymbolTable> scopes;
	int scopeDepth;


};