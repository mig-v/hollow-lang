#pragma once

#include "ir/ir_core.h"

#include "semantics/type_table.h"

#include "archive/register_allocation_core.h"
#include "archive/constant_pool.h"

#include "codegen/target_info.h"

class CodeEmitter
{
public:
	inline TargetInfo* getTargetInfo() { return &targetInfo; }

	virtual void emitCode(IRData* data, TypeTable* typeTable, ConstantPool* constantPool) = 0;
	virtual void emitFunction(const IRFunction& function) = 0;
	virtual void emitInstruction(const TacInstruction& instruction) = 0;
	virtual void initTargetInfo() = 0;
	virtual ~CodeEmitter() = default;

protected:
	TargetInfo targetInfo;
};