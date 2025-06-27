#include "compiler.h"
#include <iostream>

Compiler::Compiler()
{
	this->lexer = nullptr;
	this->parser = nullptr;
	this->semanticAnalysis = nullptr;
	this->typeChecker = nullptr;
	this->bytecodeEmitter = nullptr;
	this->bytecodeDisassembler = nullptr;
}

Compiler::~Compiler()
{
	shutdown();
}

void Compiler::shutdown()
{
	delete lexer;
	delete parser;
	delete semanticAnalysis;
	delete typeChecker;
	delete bytecodeEmitter;
	delete bytecodeDisassembler;
	diagnosticReporter.displayDiagnostics(false);
}

void Compiler::compile(const std::string& file)
{
	lexer = new Lexer();
	parser = new Parser();
	semanticAnalysis = new SemanticAnalysis();
	bytecodeEmitter = new BytecodeEmitter();
	bytecodeDisassembler = new BytecodeDisassembler();
	typeChecker = new TypeChecker();

	if (!lexer->lexFile(TEST_PATH"/parser_tests/temp.hollow"))
		return;

	parser->parse(lexer->getTokens(), &nodeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	semanticAnalysis->analyze(parser->getAst(), &typeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;
	
	std::cout << "calling typeCheck()\n";
	typeChecker->typeCheck(parser->getAst(), &nodeArena, &typeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	bytecodeEmitter->generateBytecode(parser->getAst());
	parser->printAST();
	bytecodeDisassembler->setBytecode(bytecodeEmitter->getBytecode());
	bytecodeDisassembler->disassemble();
}