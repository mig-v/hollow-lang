#pragma once

#include "core/type_info.h"
#include "core/diagnostic.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

struct StructField
{
	std::string fieldName;
	TypeInfo* typeInfo;
	size_t offset;
};

struct TypeEntry
{
	std::string typeName;

	// fast lookup for field names, but doesn't preserve source order
	std::unordered_map<std::string, StructField> structFields;

	// preserves source order
	std::vector<StructField*> fieldOrder;

	size_t typeSize;  // size of the type in bytes, 1 by default
	size_t typeAlignment;
	bool layoutComplete;
};

class TypeTable
{
public:
	bool declareType(const std::string& typeName);        // returns false if the type is already declared, true if successfully added type to table

	bool addFieldToType(const std::string& typeName, const std::string& fieldName, TypeInfo* fieldType);
	bool typeHasField(const std::string& typeName, const std::string& fieldName);
	void generateStructLayouts(DiagnosticReporter* reporter);
	void dumpTypeTable();

	TypeEntry* lookupType(const std::string& typeName);

	size_t getAlignment(TypeInfo* typeInfo);
	size_t getSize(TypeInfo* typeInfo);

private:
	void finalizeType(TypeEntry* type, DiagnosticReporter* reporter);


	std::unordered_map<std::string, TypeEntry> types;
	std::unordered_set<std::string> activeTypes;
};