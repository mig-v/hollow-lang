#pragma once

#include "ast_visitor.h"
#include "environment.h"
#include "memory_arena.h"
#include "diagnostic.h"

class ASTType;

class SymbolResolution : public ASTVisitor
{
public:
	SymbolResolution();
	~SymbolResolution();
	void resolve(std::vector<ASTNode*>& ast, DiagnosticReporter* diagnosticReporter);
	inline Environment* getEnv() { return env; }

	void visitFuncDecl(ASTFuncDecl& node) override;
	void visitVarDecl(ASTVarDecl& node) override;
	void visitIdentifier(ASTIdentifier& node) override;  // identifier references MUST be resolved when they're encountered, this is unlike functions
	void visitBlock(ASTBlock& node) override;
	void visitForLoop(ASTForLoop& node) override;
	void visitParamList(ASTParamList& node) override;
	void visitParameter(ASTParameter& node) override;
	void visitWhileLoop(ASTWhileLoop& node) override;
	void visitIfStatement(ASTIfStatement& node) override;
	void visitCall(ASTCall& node) override;
	void visitBinaryExpr(ASTBinaryExpr& node) override;
	void visitExprStmt(ASTExprStmt& node) override;
	void visitPostfix(ASTPostfix& node) override;
	void visitUnaryExpr(ASTUnaryExpr& node) override;
	void visitGroupExpr(ASTGroupExpr& node) override;
	void visitArgList(ASTArgList& node) override;
	void visitArgument(ASTArgument& node) override;
	void visitAssign(ASTAssign& node) override;
	void visitReturn(ASTReturn& node) override;
	void visitLogical(ASTLogical& node) override;
	void visitArrayAccess(ASTArrayAccess& node) override;
private:
	uint64_t calculateElementSizeInBytes(TypeInfo* elementType);
	uint64_t calculateSlotCountForElement(TypeInfo* elementType);
	void visitASTType(ASTNode& rootNode, TypeInfo* typeInfo, ASTType* type);

	Environment* env;
	MemoryArena symbolArena;
	MemoryArena typeArena;
	DiagnosticReporter* diagnosticReporter;

	// i can get away with storing a raw pointer to the function context rather than a stack because we will only be in 1 function at max
	// since I won't allow nested functions. If for some reason I add nested function support, this will need to become a stack
	TypeInfo* functionCtx;	
};