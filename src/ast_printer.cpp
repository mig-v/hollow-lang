#include <iostream>

#include "ast_printer.h"

//void ASTPrinter::visitUnsignedLiteral(ASTUnsignedLiteral& node)
//{
//	switch (node.type)
//	{
//		case TokenType::u8Keyword:
//			std::cout << "[Unsigned Literal Node] u8 = ";
//			break;
//		case TokenType::u16Keyword:
//			std::cout << "[Unsigned Literal Node] u16 = ";
//			break;
//		case TokenType::u32Keyword:
//			std::cout << "[Unsigned Literal Node] u32 = ";
//			break;
//		case TokenType::u64Keyword:
//			std::cout << "[Unsigned Literal Node] u64 = ";
//			break;
//	}
//
//	std::cout << node.value << std::endl;
//}
//
//void ASTPrinter::visitSignedLiteral(ASTSignedLiteral& node)
//{
//	switch (node.type)
//	{
//		case TokenType::i8Keyword:
//			std::cout << "[Signed Literal Node] i8 = ";
//			break;
//		case TokenType::i16Keyword:
//			std::cout << "[Signed Literal Node] i16 = ";
//			break;
//		case TokenType::i32Keyword:
//			std::cout << "[Signed Literal Node] i32 = ";
//			break;
//		case TokenType::i64Keyword:
//			std::cout << "[Signed Literal Node] i64 = ";
//			break;
//	}
//
//	std::cout << node.value << std::endl;
//}

void ASTPrinter::visitIntLiteral(ASTIntLiteral& node)
{
	std::cout << "[Int Literal Node] = " << node.value << std::endl;
}

//void ASTPrinter::visitFloatLiteral(ASTFloatLiteral& node)
//{
//	std::cout << "[Float Literal Node] f32 = " << node.value << std::endl;
//}

void ASTPrinter::visitDoubleLiteral(ASTDoubleLiteral& node)
{
	std::cout << "[Double Literal Node] f64 = " << node.value << std::endl;
}

void ASTPrinter::visitCharLiteral(ASTCharLiteral& node)
{
	std::cout << "[Char Literal Node] char = " << node.value << std::endl;
}

void ASTPrinter::visitBoolLiteral(ASTBoolLiteral& node)
{
	std::cout << "[Bool Literal Node] = " << node.value << std::endl;
}

void ASTPrinter::visitVarDecl(ASTVarDecl& node)
{
	std::cout << "[Var Decl Node] " << std::get<std::string>(node.varIdentifier.value) << " = ";

	if (node.initialization)
		node.initialization->accept(*this);
	else
		std::cout << "(null initializer)\n";
}

void ASTPrinter::visitFuncDecl(ASTFuncDecl& node)
{
	std::cout << "[Func Decl Node] Function name: " << std::get<std::string>(node.funcIdentifier.value) << std::endl;
}

void ASTPrinter::visitVarAccess(ASTVarAccess& node)
{
	std::cout << "[Var Access Node] Variable name: " << std::get<std::string>(node.identifier.value) << std::endl;
}

void ASTPrinter::visitExprStmt(ASTExprStmt& node)
{
	std::cout << "[Expr Stmt Node] --> ";
	node.expression->accept(*this);
}

void ASTPrinter::visitAssign(ASTAssign& node)
{
	std::cout << "[Assign Node] " << std::get<std::string>(node.identifier.value) << " = ";
	node.value->accept(*this);
}

void ASTPrinter::visitReturn(ASTReturn& node)
{
	std::cout << "[Return Node] return value = ";

	// the return value can be null if the function returns void
	if (node.returnVal)
		node.returnVal->accept(*this);
	else
		std::cout << "void\n";
}

void ASTPrinter::visitBlock(ASTBlock& node)
{
	std::cout << "[Block Node Start]\n";

	for (ASTNode* statement : node.statements)
		statement->accept(*this);

	std::cout << "[Block Node End]\n";
}

void ASTPrinter::visitForLoop(ASTForLoop& node)
{
	std::cout << "[For Loop Node]\ninitializer = ";

	if (node.initializer)
		node.initializer->accept(*this);
	else
		std::cout << "(null)\n";

	std::cout << "condition = ";
	if (node.condition)
		node.condition->accept(*this);
	else
		std::cout << "(null)\n";

	std::cout << "increment = ";
	if (node.increment)
		node.increment->accept(*this);
	else
		std::cout << "(null)\n";

	node.body->accept(*this);
}

void ASTPrinter::visitIfStatement(ASTIfStatement& node)
{
	std::cout << "[If Statement Node]\n\tcondition = ";
	node.condition->accept(*this);

	std::cout << "\tTrue Branch = ";
	node.trueBranch->accept(*this);

	std::cout << "\tFalse Branch = ";
	if (node.falseBranch)
		node.falseBranch->accept(*this);
	else
		std::cout << "(no false branch)\n";
}

void ASTPrinter::visitLogical(ASTLogical& node)
{
	std::cout << "[Logical Node]\n\tlhs = ";
	node.lhs->accept(*this);

	std::cout << "\trhs = ";
	if (node.rhs)
		node.rhs->accept(*this);
	else
		std::cout << "(no rhs)\n";
}

void ASTPrinter::visitBinaryExpr(ASTBinaryExpr& node)
{
	std::cout << "[Binary Expr Node]\n\tlhs = ";
	node.lhs->accept(*this);

	switch (node.op.type)
	{
		case TokenType::Plus:         std::cout << "\toperator = +\n"; break;
		case TokenType::Minus:        std::cout << "\toperator = -\n"; break;
		case TokenType::Asterisk:     std::cout << "\toperator = *\n"; break;
		case TokenType::ForwardSlash: std::cout << "\toperator = /\n"; break;
	}

	std::cout << "\trhs = ";
	node.rhs->accept(*this);
}

void ASTPrinter::visitUnaryExpr(ASTUnaryExpr& node)
{
	std::cout << "[Unary Expr Node]\n";
	
	switch (node.op.type)
	{
		case TokenType::LogicalNot: std::cout << "\top = !\n"; break;
		case TokenType::Minus:      std::cout << "\top = -\n"; break;
	}

	std::cout << "\texpression = ";
	node.expr->accept(*this);
}

void ASTPrinter::visitCall(ASTCall& node)
{
	std::cout << "[Call Node]\n\tcallee = ";
	node.callee->accept(*this);
	
	std::cout << "[Args Start]\n";

	for (ASTNode* arg : node.args)
		arg->accept(*this);

	std::cout << "[Args End]\n";
}

void ASTPrinter::visitGroupExpr(ASTGroupExpr& node)
{
	std::cout << "[Group Expr Node] --> ";
	node.expr->accept(*this);
}