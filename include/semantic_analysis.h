#pragma once

#include "ast_visitor.h"
#include "environment.h"
#include "memory_arena.h"
#include "diagnostic.h"

class SemanticAnalysis : public ASTVisitor
{
public:
	SemanticAnalysis();
	void analyze(std::vector<ASTNode*>& ast, MemoryArena* typeArena, DiagnosticReporter* diagnosticReporter, Environment* env);
	void printInfo();
	int getGlobalVarCount();

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
	void checkForMainFunction();
	void visitASTType(ASTNode& rootNode, TypeInfo* typeInfo, ASTType* type);
	uint64_t calculateElementSizeInBytes(TypeInfo* elementType);

	Environment* env;
	MemoryArena* typeArena;
	DiagnosticReporter* diagnosticReporter;
	std::vector<TypeInfo*> functionCtxStack; // used for nested calls like add(10, getNum()); we need a stack to deal with this
};