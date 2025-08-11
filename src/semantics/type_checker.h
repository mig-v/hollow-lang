#pragma once

#include "core/ast_visitor.h"
#include "core/memory_arena.h"
#include "core/diagnostic.h"
#include "core/l_value.h"

#include "semantics/type_conversions.h"
#include "semantics/type_table.h"

class TypeChecker : public ASTVisitor
{
public:
	TypeChecker();
	~TypeChecker();
	void typeCheck(std::vector<ASTNode*>& ast, MemoryArena* nodeArena, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter, TypeTable* typeTable);

	void visitIntLiteral(ASTIntLiteral& node) override;
	void visitDoubleLiteral(ASTDoubleLiteral& node) override;
	void visitCharLiteral(ASTCharLiteral& node) override;
	void visitBoolLiteral(ASTBoolLiteral& node) override;
	void visitVarDecl(ASTVarDecl& node) override;
	void visitFuncDecl(ASTFuncDecl& node) override;
	void visitIdentifier(ASTIdentifier& node) override;
	void visitExprStmt(ASTExprStmt& node) override;
	void visitAssign(ASTAssign& node) override;
	void visitReturn(ASTReturn& node) override;
	void visitBlock(ASTBlock& node) override;
	void visitForLoop(ASTForLoop& node) override;
	void visitWhileLoop(ASTWhileLoop& node) override;
	void visitIfStatement(ASTIfStatement& node) override;
	void visitLogical(ASTLogical& node) override;
	void visitBinaryExpr(ASTBinaryExpr& node) override;
	void visitUnaryExpr(ASTUnaryExpr& node) override;
	void visitCall(ASTCall& node) override;
	void visitGroupExpr(ASTGroupExpr& node) override;
	void visitPostfix(ASTPostfix& node) override;
	void visitParameter(ASTParameter& node) override;
	void visitArgument(ASTArgument& node) override;
	void visitParamList(ASTParamList& node) override;
	void visitArgList(ASTArgList& node) override;
	void visitArrayAccess(ASTArrayAccess& node) override;
private:
	bool conversionIsLegal(const ConversionInfo& conversion, ASTNode* nodeCtx);
	bool pointerAssignmentIsLegal(TypeInfo* lhs, TypeInfo* rhs);
	bool arrayAssignmentIsLegal(TypeInfo* lhs, TypeInfo* rhs);
	bool typesLegalForComparison(TypeKind lhs, TypeKind rhs);
	bool typesLegalForArithmetic(TypeKind lhs, TypeKind rhs);
	bool typesLegalForBitwise(TypeKind lhs, TypeKind rhs);

	void unifyBinaryOperands(ASTBinaryExpr& node, bool isComparisonOp);
	ASTExpr* unwrapExpr(ASTExpr* expr);

	MemoryArena* nodeArena;
	MemoryArena* typeArena;
	DiagnosticReporter* diagnosticReporter;
	std::vector<TypeInfo*> functionCtxStack;
	TypeConversions* typeConversions;
	TypeTable* typeTable;
};