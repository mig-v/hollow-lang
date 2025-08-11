#include "semantics/type_table.h"

#include "utils/type_utils.h"
#include "utils/debug_utils.h"

#include <iostream>

bool TypeTable::declareType(const std::string& typeName)
{
	if (types.find(typeName) != types.end())
		return false;

	TypeEntry entry;
	entry.typeName = typeName;
	entry.typeSize = 0;
	entry.typeAlignment = 0;
	entry.layoutComplete = false;
	types[typeName] = entry;

	return true;
}

void TypeTable::finalizeType(TypeEntry* type, DiagnosticReporter* reporter)
{
	if (type->layoutComplete)
		return;

	if (activeTypes.count(type->typeName))
	{
		std::string message = "cyclic struct dependency for " + type->typeName;
		reporter->reportDiagnostic(message, DiagnosticLevel::Error, DiagnosticType::CyclicStructDependency, ErrorPhase::SymbolResolution, 0, 0);
		return;
	}

	activeTypes.insert(type->typeName);

	size_t offset = 0;
	size_t maxAlignment = 1;

	for (StructField* field : type->fieldOrder)
	{
		if (field->typeInfo->type == TypeKind::Struct)
		{
			// make sure struct is laid out first before, type entries are guaranteed to be in the type table at this point
			// so no need to check if fieldType is a valid pointer
			TypeEntry* fieldType = lookupType(field->typeInfo->typeName);
			finalizeType(fieldType, reporter);
		}

		size_t fieldAlignment = getAlignment(field->typeInfo);
		size_t fieldSize = getSize(field->typeInfo);

		offset = TypeUtils::alignTo(offset, fieldAlignment);
		field->offset = offset;
		offset += fieldSize;

		maxAlignment = std::max(fieldAlignment, maxAlignment);
	}

	type->typeSize = TypeUtils::alignTo(offset, maxAlignment);
	type->typeAlignment = maxAlignment;
	type->layoutComplete = true;

	activeTypes.erase(type->typeName);
}

void TypeTable::generateStructLayouts(DiagnosticReporter* reporter)
{
	for (auto& [key, value] : types)
		finalizeType(&value, reporter);
}

bool TypeTable::addFieldToType(const std::string& typeName, const std::string& fieldName, TypeInfo* fieldType)
{
	TypeEntry* entry = lookupType(typeName);

	if (!entry)
		return false;

	if (entry->structFields.find(fieldName) != entry->structFields.end())
		return false;

	entry->structFields[fieldName] = {fieldName, fieldType, 0};
	entry->fieldOrder.push_back(&entry->structFields[fieldName]);
	return true;
}

bool TypeTable::typeHasField(const std::string& typeName, const std::string& fieldName)
{
	TypeEntry* type = lookupType(typeName);
	if (!type)
		return false;

	return (type->structFields.find(fieldName) != type->structFields.end());
}

TypeEntry* TypeTable::lookupType(const std::string& typeName)
{
	auto entry = types.find(typeName);
	if (entry != types.end())
		return &entry->second;

	return nullptr;
}

size_t TypeTable::getAlignment(TypeInfo* typeInfo)
{
	switch (typeInfo->type)
	{
		case TypeKind::i8:
		case TypeKind::u8:
		case TypeKind::Char:
		case TypeKind::Bool:
			return 1;
		case TypeKind::i16:
		case TypeKind::u16:
			return 2;
		case TypeKind::i32:
		case TypeKind::u32:
		case TypeKind::f32:
			return 4;
		case TypeKind::i64:
		case TypeKind::u64:
		case TypeKind::f64:
			return 8;
		case TypeKind::Array:
			return getAlignment(typeInfo->elementType);
		case TypeKind::Pointer:
			return 8;
		case TypeKind::Struct:
			return types[typeInfo->typeName].typeAlignment;
		default:
			return 8;
	}
}

size_t TypeTable::getSize(TypeInfo* typeInfo)
{
	switch (typeInfo->type)
	{
		case TypeKind::Void:
			return 0;
		case TypeKind::i8:
		case TypeKind::u8:
		case TypeKind::Char:
		case TypeKind::Bool:
			return 1;
		case TypeKind::i16:
		case TypeKind::u16:
			return 2;
		case TypeKind::i32:
		case TypeKind::u32:
		case TypeKind::f32:
			return 4;
		case TypeKind::i64:
		case TypeKind::u64:
		case TypeKind::f64:
			return 8;
		case TypeKind::Array:
			return typeInfo->arrayLength * getSize(typeInfo->elementType);
		case TypeKind::Pointer:
			return 8;
		case TypeKind::Struct:
			return types[typeInfo->typeName].typeSize;
	}
}

void TypeTable::dumpTypeTable()
{
	for (const auto& [key, value] : types)
	{
		std::cout << "struct " << key << " (size = " << value.typeSize << ", alignment = " << value.typeAlignment << "):" << std::endl;

		for (StructField* field : value.fieldOrder)
		{
			if (field->typeInfo->type == TypeKind::Struct)
				std::cout << "    " << field->fieldName << " : " << DebugUtils::typeKindToString(field->typeInfo->type) << " " << field->typeInfo->typeName <<  " offset = " << field->offset << std::endl;
			else
				std::cout << "    " << field->fieldName << " : " << DebugUtils::typeKindToString(field->typeInfo->type) << " offset = " << field->offset << std::endl;
		}
	}
}