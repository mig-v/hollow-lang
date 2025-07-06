#pragma once

#include <vector>
#include <string>
#include <map>
#include <fstream>

#include "parser.h"
#include "lexer.h"
#include "semantic_analysis.h"
#include "type_checker.h"
#include "bytecode_emitter.h"
#include "symbol_resolution.h"

struct BytecodeEmitterTestInfo
{
	std::string testname;
	std::vector<uint8_t> expectedBytecode;
};

class BytecodeEmitterTests
{
public:
	BytecodeEmitterTests();
	~BytecodeEmitterTests();
	void runAll();

private:
	void runSpecific(const std::string& filename);

	Lexer* lexer;
	Parser* parser;
	SymbolResolution* symbolResolution;
	SemanticAnalysis* semanticAnalysis;
	TypeChecker* typeChecker;
	BytecodeEmitter* emitter;
	MemoryArena* nodeArena;
	MemoryArena* typeArena;
	DiagnosticReporter* reporter;

	std::map<std::string, BytecodeEmitterTestInfo> testSuite;
	std::ofstream logFile;
};