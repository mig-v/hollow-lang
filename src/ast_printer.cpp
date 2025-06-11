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