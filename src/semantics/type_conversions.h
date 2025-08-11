#pragma once

#include "core/type_info.h"

#include <array>

enum class ConversionType
{
	SameType,		// types are the same, so no conversion needs to take place
	ExplicitOnly,	// conversion is only allowed explicitly
	Widening,		// conversion is widening (implicitly)
	Narrowing,		// conversion is narrowing (implicitly)
	NotAllowed		// conversion is not allowed implicitly or explicitly
};

struct ConversionInfo
{
	ConversionType type;
	const char* message;
};

class TypeConversions
{
public:
	TypeConversions();
	ConversionInfo getConversion(TypeKind from, TypeKind to);
private:
	void addConversion(TypeKind from, TypeKind to, ConversionType type, const char* message = "");
	// looks scary, just a 2D array of ConversionInfo
	// essentially just --> ConversionInfo conversionTable[TypeKind::TypeCount][TypeKind::TypeCount];
	std::array<std::array<ConversionInfo, static_cast<size_t>(TypeKind::TypeCount)>, static_cast<size_t>(TypeKind::TypeCount)> conversionTable;
};