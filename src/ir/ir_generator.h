#pragma once

#include "parser/ast_node.h"
#include "parser/ast_expr.h"
#include "parser/ast_stmt.h"

#include "core/memory_arena.h"

#include "semantics/type_table.h"

#include "ir/ir_environment.h"
#include "ir/ir_core.h"

class IRGenerator
{
public:
	IRGenerator();
	void generate(std::vector<ASTNode*>& ast, MemoryArena* typeArena, TypeTable* typeTable);
	inline IRData* getIRData() { return &irData; }

	IROperand visitIntLiteral(ASTIntLiteral& node);
	IROperand visitDoubleLiteral(ASTDoubleLiteral& node);
	IROperand visitCharLiteral(ASTCharLiteral& node);
	IROperand visitBoolLiteral(ASTBoolLiteral& node);
	IROperand visitVarDecl(ASTVarDecl& node);
	IROperand visitFuncDecl(ASTFuncDecl& node);
	IROperand visitIdentifier(ASTIdentifier& node);
	IROperand visitExprStmt(ASTExprStmt& node);
	IROperand visitAssign(ASTAssign& node);
	IROperand visitReturn(ASTReturn& node);
	IROperand visitBlock(ASTBlock& node);
	IROperand visitForLoop(ASTForLoop& node);
	IROperand visitWhileLoop(ASTWhileLoop& node);
	IROperand visitIfStatement(ASTIfStatement& node);
	IROperand visitLogical(ASTLogical& node);
	IROperand visitBinaryExpr(ASTBinaryExpr& node);
	IROperand visitUnaryExpr(ASTUnaryExpr& node);
	IROperand visitCall(ASTCall& node);
	IROperand visitGroupExpr(ASTGroupExpr& node);
	IROperand visitPostfix(ASTPostfix& node);
	IROperand visitParameter(ASTParameter& node);
	IROperand visitArgument(ASTArgument& node);
	IROperand visitParamList(ASTParamList& node);
	IROperand visitArgList(ASTArgList& node);
	IROperand visitArrayAccess(ASTArrayAccess& node);
	IROperand visitCast(ASTCast& node);
	IROperand visitStructDecl(ASTStructDecl& node);
	IROperand visitFieldAccess(ASTFieldAccess& node);
private:
	uint32_t newID();
	uint32_t newFuncID();
	uint32_t newLabelID();

	void pushValue(const TacValue& value);
	void setValue(uint32_t id, const TacValue& value);

	void pushEvalMode(EvalMode mode);
	void popEvalMode();
	EvalMode getEvalMode();

	uint32_t makeTemporary(TypeKind type);                    // makes a temporary, pushes it into value table, and returns its ID
	IROperand makeIntConstant(TypeKind type);                 // makes a constant for inc / dec instructions, defaults to 1
	IROperand makeNullOperand();
	IROperand makeTemporaryOperand(TypeKind type);
	IROperand makeLabelOperand();

	void emitInstruction(const TacInstruction& instruction);
	void emitShortCircuitOr(ASTLogical& node, IROperand trueLabel, IROperand falseLabel);
	void emitShortCircuitAnd(ASTLogical& node, IROperand trueLabel, IROperand falseLabel);
	IROperand emitArrayIndexCalculation(ASTExpr* base);

	IREnvironment env;
	TypeInfo* implicitCastCtx;
	IRFunction* currentFunction;
	MemoryArena* typeArena;
	MemoryArena funcArena;
	TypeTable* typeTable;
	IRData irData;
	IROperand nullOperand;

	bool shortCircuitCtx;
	uint32_t currID;
	uint32_t funcID;
	uint32_t labelID;

	//std::vector<TacInstruction> instructions;
	//std::vector<TacValue> values;
	std::vector<IRConditionContext> conditionCtxStack;
	std::vector<EvalMode> evalModeStack;

	//// kinda overly complicated, but I want the functions to be in order for printing purposes when debugging, so I want to store them
	//// in order in a vector but still be able to look them up in a map by name. So this works as func identifier -> func id -> IRFunction
	//std::unordered_map<std::string, uint32_t> funcToId;
	//std::vector<IRFunction*> functionTable;
};