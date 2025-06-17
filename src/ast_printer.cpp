#include <iostream>

#include "ast_printer.h"
#include "debug_utils.h"

void ASTPrinter::visitIntLiteral(ASTIntLiteral& node)
{
	std::cout << "[Int Literal Node] = " << node.value << std::endl;
}

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
	std::cout << "[Var Decl Node] : " << std::get<std::string>(node.varIdentifier.value) << "\n\tinitializer: ";

	if (node.initialization)
		node.initialization->accept(*this);
	else
		std::cout << "(null initializer)\n";

	if (node.typeInfo)
		std::cout << "\ttype: " << DebugUtils::typeKindToString(node.typeInfo->type) << " scope: " << node.scope << " slot index : " << node.slotIndex << std::endl;
}

void ASTPrinter::visitFuncDecl(ASTFuncDecl& node)
{
	std::cout << "[Func Decl Node] Function name: " << std::get<std::string>(node.funcIdentifier.value) << std::endl;
	node.params->accept(*this);
}

void ASTPrinter::visitVariable(ASTVariable& node)
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
		case TokenType::Plus:          std::cout << "\toperator: +\n"; break;
		case TokenType::Minus:         std::cout << "\toperator: -\n"; break;
		case TokenType::Asterisk:      std::cout << "\toperator: *\n"; break;
		case TokenType::ForwardSlash:  std::cout << "\toperator: /\n"; break;
		case TokenType::NotEq:         std::cout << "\toperator: !=\n"; break;
		case TokenType::Equality:      std::cout << "\toperator: ==\n"; break;
		case TokenType::GreaterThan:   std::cout << "\toperator: >\n"; break;
		case TokenType::GreaterThanEq: std::cout << "\toperator: >=\n"; break;
		case TokenType::LessThan:      std::cout << "\toperator: <\n"; break;
		case TokenType::LessThanEq:    std::cout << "\toperator: <=\n"; break;
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

	for (ASTArgument* arg : node.args->args)
		arg->accept(*this);

	std::cout << "[Args End]\n";
}

void ASTPrinter::visitGroupExpr(ASTGroupExpr& node)
{
	std::cout << "[Group Expr Node] --> ";
	node.expr->accept(*this);
}

void ASTPrinter::visitPostfix(ASTPostfix& node)
{
	std::cout << "[Postfix Node] --> ";

	switch (node.op.type)
	{
		case TokenType::Increment: std::cout << "op = ++ "; break;
		case TokenType::Decrement: std::cout << "op = -- "; break;
	}

	std::cout << " expr: ";
	node.expr->accept(*this);
}

void ASTPrinter::visitParameter(ASTParameter& node)
{
	std::cout << "[Parameter Node] Identifier : "
		<< std::get<std::string>(node.paramIdentifier.value);
		
	switch (node.paramType.type)
	{
		case TokenType::u8Keyword:   std::cout << " Type: u8\n";   break;
		case TokenType::u16Keyword:  std::cout << " Type: u16\n";  break;
		case TokenType::u32Keyword:  std::cout << " Type: u32\n";  break;
		case TokenType::u64Keyword:  std::cout << " Type: u64\n";  break;
		case TokenType::i8Keyword:   std::cout << " Type: i8\n";   break;
		case TokenType::i16Keyword:  std::cout << " Type: i16\n";  break;
		case TokenType::i32Keyword:  std::cout << " Type: i32\n";  break;
		case TokenType::i64Keyword:  std::cout << " Type: i64\n";  break;
		case TokenType::f32Keyword:  std::cout << " Type: f32\n";  break;
		case TokenType::f64Keyword:  std::cout << " Type: f64\n";  break;
		case TokenType::charKeyword: std::cout << " Type: char\n"; break;
		case TokenType::boolKeyword: std::cout << " Type: bool\n"; break;

		// if the type token isn't a built in type, then its a user defined type so it will be stored in the tokens std::variant<std::string> value
		default:
			std::cout << " Type : " << std::get<std::string>(node.paramType.value) << std::endl;
	}
}

void ASTPrinter::visitArgument(ASTArgument& node)
{
	std::cout << "[Argument Node] value : ";
	node.value->accept(*this);
}

void ASTPrinter::visitParamList(ASTParamList& node)
{
	std::cout << "[Params Start]\n";

	for (ASTParameter* param : node.params)
		param->accept(*this);

	std::cout << "[Params End]\n";
}

void ASTPrinter::visitArgList(ASTArgList& node)
{
	std::cout << "[Args Start]\n";

	for (ASTArgument* arg: node.args)
		arg->accept(*this);

	std::cout << "[Args End]\n";
}