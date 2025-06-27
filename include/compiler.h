#pragma once

#include "lexer.h"
#include "parser.h"
#include "semantic_analysis.h"
#include "type_checker.h"
#include "bytecode_emitter.h"
#include "bytecode_disassembler.h"
#include "memory_arena.h"
#include "diagnostic.h"

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
	SemanticAnalysis* semanticAnalysis;
	TypeChecker* typeChecker;
	BytecodeEmitter* bytecodeEmitter;
	BytecodeDisassembler* bytecodeDisassembler;
};