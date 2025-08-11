//#pragma once
//
//#include "archive/opcodes.h"
//#include "archive/vm_function_table.h"
//
//#include "core/ast_visitor.h"
//
//struct JumpLabel
//{
//	uint16_t jumpAddress = 0;			// target jump location
//	std::vector<uint16_t> jumpPatches;	// vector of addresses that need to be patched to jump to the labels 'jumpAddress'
//};
//
//// used to track conditional contexts as a whole like the whole condition of if, while, for expressions
//// if ({ConditionContext tracker})
//struct ConditionContext
//{
//	JumpLabel falseJump;	// where to jump to if condition is false
//	JumpLabel trueJump;	    // where to jump to if condition is true
//	JumpLabel end;		    // end of condition construct, instruction immediately after while loop, if statement, etc.
//};
//
//struct DeferredCall
//{
//	ASTCall* call;
//	size_t callAddress;
//};
//
//class BytecodeEmitter : public ASTVisitor
//{
//public:
//	BytecodeEmitter();
//	void generateBytecode(std::vector<ASTNode*>& ast);
//	void rawDumpBytecode();
//	std::vector<uint8_t>* getBytecode() { return &bytecode; }
//	VMFunctionTable* getFunctionTable() { return &functionTable; }
//
//	void visitIntLiteral(ASTIntLiteral& node) override;
//	void visitDoubleLiteral(ASTDoubleLiteral& node) override;
//	void visitCharLiteral(ASTCharLiteral& node) override;
//	void visitBoolLiteral(ASTBoolLiteral& node) override;
//	void visitVarDecl(ASTVarDecl& node) override;
//	void visitFuncDecl(ASTFuncDecl& node) override;
//	void visitIdentifier(ASTIdentifier& node) override;
//	void visitExprStmt(ASTExprStmt& node) override;
//	void visitAssign(ASTAssign& node) override;
//	void visitReturn(ASTReturn& node) override;
//	void visitBlock(ASTBlock& node) override;
//	void visitForLoop(ASTForLoop& node) override;
//	void visitWhileLoop(ASTWhileLoop& node) override;
//	void visitIfStatement(ASTIfStatement& node) override;
//	void visitLogical(ASTLogical& node) override;
//	void visitBinaryExpr(ASTBinaryExpr& node) override;
//	void visitUnaryExpr(ASTUnaryExpr& node) override;
//	void visitCall(ASTCall& node) override;
//	void visitGroupExpr(ASTGroupExpr& node) override;
//	void visitPostfix(ASTPostfix& node) override;
//	void visitArgument(ASTArgument& node) override;
//	void visitArgList(ASTArgList& node) override;
//	void visitCast(ASTCast& node) override;
//	void visitArrayAccess(ASTArrayAccess& node) override;
//
//private:
//	void emit(Opcode opcode);
//	void emit8(uint8_t value);
//	void emit16(uint16_t value);
//	void emit32(uint32_t value);
//	void emit64(uint64_t value);
//	void emitDefaultValue(TypeKind type);
//	void insert16(uint16_t value, size_t offset);	// writes a 2 bytes value into bytecode at bytecode[offset], bytecode[offset + 1]
//	void insert8(uint8_t value, size_t offset);    // writes 1 byte 'value' into bytecode at bytecode[offset]
//
//	void patchLabelJumps(std::vector<uint16_t>& patchSites, uint16_t address);
//	void resolveDeferredFunction(ASTFuncDecl* function);
//	void resolveDeferredCall(DeferredCall& deferredCall);
//	void emitCallToMain();
//	void emitIndirectAddress(ASTExpr* base);
//
//	void emitShortCircuitOr(ASTLogical& node, JumpLabel* trueTarget, JumpLabel* falseTarget);
//	void emitShortCircuitAnd(ASTLogical& node, JumpLabel* trueTarget, JumpLabel* falseTarget);
//	void patchLabel(JumpLabel& label);
//	bool tryOverwriteLastJump(Opcode newJump);
//	bool lastInstructionWasConditionalJmp();
//
//	Opcode getTypeSpecificAddOpcode(TypeKind type);
//	Opcode getTypeSpecificSubOpcode(TypeKind type);
//	Opcode getTypeSpecificMulOpcode(TypeKind type);
//	Opcode getTypeSpecificDivOpcode(TypeKind type);
//	Opcode getTypeSpecificStoreLocalOpcode(TypeKind type);
//	Opcode getTypeSpecificStoreGlobalOpcode(TypeKind type);
//	Opcode getTypeSpecificNegOpcode(TypeKind type);
//	Opcode getTypeSpecificNotOpcode(TypeKind type);
//	Opcode getTypeSpecificAndOpcode(TypeKind type);
//	Opcode getTypeSpecificOrOpcode(TypeKind type);
//	Opcode getTypeSpecificXorOpcode(TypeKind type);
//	Opcode getTypeSpecificShlOpcode(TypeKind type);
//	Opcode getTypeSpecificShrOpcode(TypeKind type);
//	Opcode getTypeSpecificEqOpcode(TypeKind type);
//	Opcode getTypeSpecificNeqOpcode(TypeKind type);
//	Opcode getTypeSpecificGtOpcode(TypeKind type);
//	Opcode getTypeSpecificGteOpcode(TypeKind type);
//	Opcode getTypeSpecificLtOpcode(TypeKind type);
//	Opcode getTypeSpecificLteOpcode(TypeKind type);
//	Opcode getTypeSpecificLoadLocalOpcode(TypeKind type);
//	Opcode getTypeSpecificLoadGlobalOpcode(TypeKind type);
//	Opcode getTypeSpecificIncOpcode(TypeKind type);
//	Opcode getTypeSpecificDecOpcode(TypeKind type);
//	Opcode getTypeSpecificLoadLocalIndOpcode(TypeKind type);
//	Opcode getTypeSpecificLoadGlobalIndOpcode(TypeKind type);
//	Opcode getTypeSpecificStoreLocalIndOpcode(TypeKind type);
//	Opcode getTypeSpecificStoreGlobalIndOpcode(TypeKind type);
//
//	TypeInfo* implicitCastCtx;
//	VMFunctionTable functionTable;
//	bool shortCircuitCtx;
//
//	std::vector<uint8_t> bytecode;
//	std::vector<ConditionContext> conditionCtxStack;
//	std::vector<ASTFuncDecl*> deferredFunctions;
//	std::vector<DeferredCall> deferredCalls;
//};