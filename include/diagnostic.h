#pragma once

#include <string>
#include <vector>

#include "token.h"

enum class DiagnosticLevel
{
	None,
	Warning,
	Error
};

enum class ErrorPhase
{
	None,
	Parser,
	Semantic,
	TypeChecker
};

enum class DiagnosticType
{
	None,

	// warnings
	NarrowingConversion,

	// errors
	DuplicateIdentifier,
	IncompatibleType,
	UndefinedIdentifier,
	IncompatibleOperands,
	IncompatibleReturnType,
	UnknownOperator,
	UnresolvedFunctionCall,
	IncompatibleArgCount,
	ReturnOutsideFunction,
	ParseError
};

struct Diagnostic
{
	DiagnosticLevel level;      // severity level
	DiagnosticType  type;       // type of warning / error
	ErrorPhase      phase;      // which phase of the compiler does the error originate from
	std::string     message;    // error / warning message
	uint32_t        line;       // error line
	uint32_t        col;        // error column

	Diagnostic() : level(DiagnosticLevel::None), type(DiagnosticType::None), phase(ErrorPhase::None), message(std::string()), line(1), col(1) {}
};

class DiagnosticReporter
{
public:
	DiagnosticReporter() = default;
	void reportDiagnostic(const std::string& message, DiagnosticLevel level, DiagnosticType type, ErrorPhase phase, uint32_t line, uint32_t col);
	void displayDiagnostics(bool verbose);
	bool hasErrors();
private:
	const char* diagnosticLevelToString(DiagnosticLevel level);
	const char* errorPhaseToString(ErrorPhase phase);
	const char* diagnosticTypeToString(DiagnosticType type);

	std::vector<Diagnostic> diagnostics;
};