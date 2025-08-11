#pragma once

#include "lexer/lexer.h"

#include "parser/parser.h"

#include "semantics/semantic_analysis.h"
#include "semantics/type_checker.h"
#include "semantics/symbol_resolution.h"

#include "ir/ir_generator.h"

#include "core/memory_arena.h"
#include "core/diagnostic.h"

#include "codegen/code_emitter.h"

class Compiler
{
public:
	Compiler();
	~Compiler();

	void compile(const std::string& file);
	void shutdown();

private:
	MemoryArena typeArena;
	MemoryArena nodeArena;

	DiagnosticReporter diagnosticReporter;

	Lexer* lexer;
	Parser* parser;
	SymbolResolution* symbolResolution;
	SemanticAnalysis* semanticAnalysis;
	TypeChecker* typeChecker;
	BytecodeEmitter* bytecodeEmitter;
	IRGenerator* irGenerator;
	CodeEmitter* codeEmitter;
};