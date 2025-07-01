#pragma once

#include "ast_visitor.h"
#include "memory_arena.h"
#include "diagnostic.h"
#include "type_conversions.h"
#include "l_value.h"

class TypeChecker : public ASTVisitor
{
public:
	TypeChecker();
	~TypeChecker();
	void typeCheck(std::vector<ASTNode*>& ast, MemoryArena* nodeArena, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter);

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
	void visitCast(ASTCast& node) override;
private:
	bool conversionIsLegal(const ConversionInfo& conversion, ASTNode* nodeCtx);
	bool typesLegalForComparison(TypeKind lhs, TypeKind rhs);
	bool typesLegalForArithmetic(TypeKind lhs, TypeKind rhs);
	bool typesLegalForBitwise(TypeKind lhs, TypeKind rhs);
	LValue isLValue(ASTExpr* expr);

	void unifyBinaryOperands(ASTBinaryExpr& node, bool isComparisonOp);
	ASTExpr* unwrapExpr(ASTExpr* expr);

	MemoryArena* nodeArena;
	MemoryArena* typeArena;
	DiagnosticReporter* diagnosticReporter;
	std::vector<TypeInfo*> functionCtxStack;
	TypeConversions* typeConversions;
};