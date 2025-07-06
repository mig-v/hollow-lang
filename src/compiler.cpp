#include "compiler.h"
#include <iostream>

Compiler::Compiler()
{
	this->lexer = nullptr;
	this->parser = nullptr;
	this->symbolResolution = nullptr;
	this->semanticAnalysis = nullptr;
	this->typeChecker = nullptr;
	this->bytecodeEmitter = nullptr;
	this->bytecodeDisassembler = nullptr;
	this->vm = nullptr;
}

Compiler::~Compiler()
{
	shutdown();
}

void Compiler::shutdown()
{
	delete lexer;
	delete parser;
	delete symbolResolution;
	delete semanticAnalysis;
	delete typeChecker;
	delete bytecodeEmitter;
	delete bytecodeDisassembler;
	delete vm;
	diagnosticReporter.displayDiagnostics(false);
}

void Compiler::compile(const std::string& file)
{
	lexer = new Lexer();
	parser = new Parser();
	symbolResolution = new SymbolResolution();
	semanticAnalysis = new SemanticAnalysis();
	bytecodeEmitter = new BytecodeEmitter();
	bytecodeDisassembler = new BytecodeDisassembler();
	typeChecker = new TypeChecker();
	vm = new VM();

	if (!lexer->lexFile(TEST_PATH"/bytecode_emitter_tests/short_circuit_test.hollow"))
		return;

	parser->parse(lexer->getTokens(), &nodeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	symbolResolution->resolve(parser->getAst(), &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	semanticAnalysis->analyze(parser->getAst(), &typeArena, &diagnosticReporter, symbolResolution->getEnv());
	int globalVarCount = semanticAnalysis->getGlobalVarCount();
	if (diagnosticReporter.hasErrors())
		return;

	typeChecker->typeCheck(parser->getAst(), &nodeArena, &typeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	bytecodeEmitter->generateBytecode(parser->getAst());
	//parser->printAST();
	//bytecodeDisassembler->setBytecode(bytecodeEmitter->getBytecode());
	//bytecodeDisassembler->disassemble();
	vm->execute(bytecodeEmitter->getBytecode(), bytecodeEmitter->getFunctionTable(), globalVarCount);
	bytecodeEmitter->rawDumpBytecode();
}