#pragma once

#include "opcodes.h"

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"

struct JumpLabel
{
	uint16_t jumpAddress = 0;
	std::vector<uint16_t> jumpPatches;
};

struct ConditionContext
{
	JumpLabel falseBranch;	// where to jump to if condition is false
	JumpLabel trueBranch;	// where to jump to if condition is true
	JumpLabel end;			// end of condition construct, instruction immediately after while loop, if statement, etc.
};

class BytecodeEmitter
{
public:
	BytecodeEmitter();
	void generateBytecode(std::vector<ASTNode*>& ast);
	const std::vector<uint8_t>* getBytecode() { return &bytecode; }

	void visitIntLiteral(ASTIntLiteral& node);
	void visitDoubleLiteral(ASTDoubleLiteral& node);
	void visitCharLiteral(ASTCharLiteral& node);
	void visitBoolLiteral(ASTBoolLiteral& node);
	void visitVarDecl(ASTVarDecl& node);
	void visitFuncDecl(ASTFuncDecl& node);
	void visitIdentifier(ASTIdentifier& node);
	void visitExprStmt(ASTExprStmt& node);
	void visitAssign(ASTAssign& node);
	void visitReturn(ASTReturn& node);
	void visitBlock(ASTBlock& node);
	void visitForLoop(ASTForLoop& node);
	void visitWhileLoop(ASTWhileLoop& node);
	void visitIfStatement(ASTIfStatement& node);
	void visitLogical(ASTLogical& node);
	void visitBinaryExpr(ASTBinaryExpr& node);
	void visitUnaryExpr(ASTUnaryExpr& node);
	void visitCall(ASTCall& node);
	void visitGroupExpr(ASTGroupExpr& node);
	void visitPostfix(ASTPostfix& node);
	void visitParameter(ASTParameter& node);
	void visitArgument(ASTArgument& node);
	void visitParamList(ASTParamList& node);
	void visitArgList(ASTArgList& node);
	void visitCast(ASTCast& node);

private:
	void emit(Opcode opcode);
	void emit8(uint8_t value);
	void emit16(uint16_t value);
	void emit32(uint32_t value);
	void emit64(uint64_t value);
	void emitDefaultValue(TypeKind type);
	void insert16(uint16_t value, size_t offset);	// writes a 2 bytes value into bytecode at bytecode[offset], bytecode[offset + 1]

	void patchLabelJumps(std::vector<uint16_t>& patchSites, uint16_t address);

	Opcode getTypeSpecificAddOpcode(TypeKind type);
	Opcode getTypeSpecificSubOpcode(TypeKind type);
	Opcode getTypeSpecificMulOpcode(TypeKind type);
	Opcode getTypeSpecificDivOpcode(TypeKind type);
	Opcode getTypeSpecificStoreOpcode(TypeKind type);
	Opcode getTypeSpecificNegOpcode(TypeKind type);
	Opcode getTypeSpecificNotOpcode(TypeKind type);
	Opcode getTypeSpecificAndOpcode(TypeKind type);
	Opcode getTypeSpecificOrOpcode(TypeKind type);
	Opcode getTypeSpecificXorOpcode(TypeKind type);
	Opcode getTypeSpecificShlOpcode(TypeKind type);
	Opcode getTypeSpecificShrOpcode(TypeKind type);
	Opcode getTypeSpecificEqOpcode(TypeKind type);
	Opcode getTypeSpecificNeqOpcode(TypeKind type);
	Opcode getTypeSpecificGtOpcode(TypeKind type);
	Opcode getTypeSpecificGteOpcode(TypeKind type);
	Opcode getTypeSpecificLtOpcode(TypeKind type);
	Opcode getTypeSpecificLteOpcode(TypeKind type);


	TypeInfo* implicitCastCtx;
	bool shortCircuitCtx;

	std::vector<uint8_t> bytecode;
	std::vector<ConditionContext> conditionCtxStack;
};