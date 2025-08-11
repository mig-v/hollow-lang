#include "core/compiler.h"

#include "archive/x86_64_emitter.h"
#include "archive/register_allocation.h"
#include "archive/constant_pool.h"

#include "optimization/optimizer.h"

#include "ir/ir_lowering.h"
#include "ir/ir_logger.h"

#include <iostream>

Compiler::Compiler()
{
	this->lexer = nullptr;
	this->parser = nullptr;
	this->symbolResolution = nullptr;
	this->semanticAnalysis = nullptr;
	this->typeChecker = nullptr;
	this->irGenerator = nullptr;
	this->bytecodeEmitter = nullptr;
	this->codeEmitter = nullptr;
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
	delete irGenerator;
	delete bytecodeEmitter;
	diagnosticReporter.displayDiagnostics(true);
}

void Compiler::compile(const std::string& file)
{
	lexer = new Lexer();
	parser = new Parser();
	symbolResolution = new SymbolResolution();
	semanticAnalysis = new SemanticAnalysis();
	typeChecker = new TypeChecker();
	irGenerator = new IRGenerator();
	//codeEmitter = new x86_64Emitter();

	//if (!lexer->lexFile(TEST_PATH"/parser_tests/temp.hollow"))
	if (!lexer->lexFile(TEST_PATH"/asm_test.hollow"))
		return;

	parser->parse(lexer->getTokens(), &nodeArena, &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	symbolResolution->resolve(parser->getAst(), &diagnosticReporter);
	if (diagnosticReporter.hasErrors())
		return;

	semanticAnalysis->analyze(parser->getAst(), &typeArena, &diagnosticReporter, symbolResolution->getEnv(), symbolResolution->getTypeTable());
	int globalVarCount = semanticAnalysis->getGlobalVarCount();
	if (diagnosticReporter.hasErrors())
		return;

	TypeTable* typeTable = symbolResolution->getTypeTable();

	typeChecker->typeCheck(parser->getAst(), &nodeArena, &typeArena, &diagnosticReporter, symbolResolution->getTypeTable());
	if (diagnosticReporter.hasErrors())
		return;

	irGenerator->generate(parser->getAst(), &typeArena, symbolResolution->getTypeTable());

	parser->printAST();
	typeTable->dumpTypeTable();

	IRLogger irLogger;
	irLogger.log(irGenerator->getIRData(), "logs/ir_logs/unoptimized_ir.txt");

	Optimizer optimizer;
	optimizer.optimize(irGenerator->getIRData(), OptimizationLevel::O1);
	irLogger.log(irGenerator->getIRData(), "logs/ir_logs/optimized_ir.txt");

	IRLowering irLowering;
	irLowering.lower(irGenerator->getIRData());
	irLogger.log(irGenerator->getIRData(), "logs/ir_logs/lowered_ir.txt");

	ConstantPool constantPool;
	constantPool.allocate(irGenerator->getIRData());
	constantPool.dumpConstants();


	//codeEmitter->emitCode(irGenerator->getIRData(), symbolResolution->getTypeTable(), &constantPool);
}