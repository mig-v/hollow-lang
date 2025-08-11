#include "core/diagnostic.h"

#include "utils/debug_utils.h"

#include <iostream>

void DiagnosticReporter::reportDiagnostic(const std::string& message, DiagnosticLevel level, DiagnosticType type, ErrorPhase phase, uint32_t line, uint32_t col)
{
	Diagnostic diagnostic;
	diagnostic.message = message;
	diagnostic.level = level;
	diagnostic.type = type;
	diagnostic.phase = phase;
	diagnostic.line = line;
	diagnostic.col = col;
	diagnostics.emplace_back(diagnostic);
}

void DiagnosticReporter::displayDiagnostics(bool verbose)
{
	for (const Diagnostic& diagnostic : diagnostics)
	{
		if (verbose)
			std::cout << "[" << errorPhaseToString(diagnostic.phase) << " " << diagnosticLevelToString(diagnostic.level)
			<< ": " << diagnosticTypeToString(diagnostic.type) << "\nline " << diagnostic.line << " col " << diagnostic.col
			<< " " << diagnostic.message << std::endl;
		else
			std::cout << "[" << diagnosticLevelToString(diagnostic.level) << "] line "
			<< diagnostic.line << " col " << diagnostic.col << " " << diagnostic.message << std::endl;
	}
}

const char* DiagnosticReporter::diagnosticLevelToString(DiagnosticLevel level)
{
	switch (level)
	{
		case DiagnosticLevel::Warning: return "Warning";
		case DiagnosticLevel::Error:   return " Error ";
		case DiagnosticLevel::None:    return "None";
	}
}

const char* DiagnosticReporter::errorPhaseToString(ErrorPhase phase)
{
	switch (phase)
	{
		case ErrorPhase::None:             return "None";
		case ErrorPhase::Parser:           return "Parse";
		case ErrorPhase::Semantic:         return "Semantic";
		case ErrorPhase::TypeChecker:      return "Type Checker";
		case ErrorPhase::SymbolResolution: return "Symbol Resolution";
	}
}

const char* DiagnosticReporter::diagnosticTypeToString(DiagnosticType type)
{
	switch (type)
	{
		case DiagnosticType::None:                   return "None";
		case DiagnosticType::NarrowingConversion:    return "Narrowing Conversion";
		case DiagnosticType::DuplicateIdentifier:    return "Duplicate Identifier";
		case DiagnosticType::IncompatibleType:       return "Incompatible Type";
		case DiagnosticType::UndefinedIdentifier:    return "Undefined Identifier";
		case DiagnosticType::IncompatibleOperands:   return "Incompatible Operands";
		case DiagnosticType::IncompatibleReturnType: return "Incompatible Return Type";
		case DiagnosticType::UnresolvedFunctionCall: return "Unresolved Function";
		case DiagnosticType::IncompatibleArgCount:   return "Incompatible Argument Count";
		case DiagnosticType::ReturnOutsideFunction:  return "Return Outside Function";
		case DiagnosticType::ParseError:             return "Parse Error";
		case DiagnosticType::CyclicStructDependency: return "Cyclic Struct Dependency";
		case DiagnosticType::UnknownOperator:        return "Unknown Operator";
		case DiagnosticType::InvalidFieldName:       return "Invalid Field Name";
	}
}

bool DiagnosticReporter::hasErrors()
{
	for (const Diagnostic& diagnostic : diagnostics)
	{
		if (diagnostic.level == DiagnosticLevel::Error)
			return true;
	}

	return false;
}