#pragma once

#include "ast_node.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "memory_arena.h"
#include "diagnostic.h"
#include "type_conversions.h"
#include "l_value.h"

class TypeChecker
{
public:
	TypeChecker();
	~TypeChecker();
	void typeCheck(std::vector<ASTNode*>& ast, MemoryArena* nodeArena, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter);

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