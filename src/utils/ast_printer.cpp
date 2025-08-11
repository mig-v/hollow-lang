#include "parser/ast_type.h"

#include "utils/ast_printer.h"
#include "utils/debug_utils.h"

#include <iostream>

namespace { constexpr uint32_t SPACES_PER_DEPTH = 4; }

void ASTPrinter::visitIntLiteral(ASTIntLiteral& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' '); 
	std::cout << indent << "[Int Literal Node] " << node.value << std::endl;
	printTypeInfo(depth + 1, node.typeInfo);
}

void ASTPrinter::visitDoubleLiteral(ASTDoubleLiteral& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Double Literal Node] " << node.value << std::endl;
	printTypeInfo(depth + 1, node.typeInfo);
}

void ASTPrinter::visitCharLiteral(ASTCharLiteral& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Char Literal Node] " << node.value << std::endl;
	printTypeInfo(depth + 1, node.typeInfo);
}

void ASTPrinter::visitBoolLiteral(ASTBoolLiteral& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Bool Literal Node] " << node.value << std::endl;
	printTypeInfo(depth + 1, node.typeInfo);
}

void ASTPrinter::printNull(uint32_t depth, const std::string& message)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << message << std::endl;
}

void ASTPrinter::printTypeInfo(uint32_t depth, TypeInfo* info)
{
	if (!info)
	{
		printNull(depth + 1, "(no type information)");
		return;
	}

	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Type Info] " << DebugUtils::typeKindToString(info->type) << std::endl;

	switch (info->type)
	{
		case TypeKind::Array:
		{
			std::cout << indent << "[Arr Element Type]\n";
			printTypeInfo(depth + 1, info->elementType);
			std::cout << indent << "[Array Length] " << info->arrayLength << std::endl;
			std::cout << indent << "[Array Element Size] " << info->elementSize << std::endl;
			std::cout << indent << "[Array Slot Count Per Element] " << info->slotCountPerElement << std::endl;
			break;
		}
		case TypeKind::Pointer:
		{
			std::cout << indent << "[Base Ptr Type]\n";
			printTypeInfo(depth + 1, info->ptrBaseType);
			break;
		}
		case TypeKind::Struct:
		{
			std::cout << indent << "[Struct Identifier] " << info->typeName << std::endl;
			break;
		}
	}
}

void ASTPrinter::printASTTypeInfo(uint32_t depth, ASTType* astType)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::string nextIndent(SPACES_PER_DEPTH * (depth + 1), ' ');

	std::cout << indent << "[AST Type Info] ";

	switch (astType->astType)
	{
		case ASTTypeKind::Primitive:
		{
			ASTPrimitiveType* type = static_cast<ASTPrimitiveType*>(astType);
			std::cout << "Primitive " << DebugUtils::tokenTypeToString(type->primitiveType) << std::endl;
			break;
		}
		case ASTTypeKind::Array:
		{
			ASTArrayType* type = static_cast<ASTArrayType*>(astType);
			std::cout << "Array: dimension count = " << type->dimensionCount << std::endl;
			printASTTypeInfo(depth + 1, type->elementType);
			type->size->accept(*this, depth + 1);
			break;
		}
		case ASTTypeKind::Pointer:
		{
			ASTPointerType* type = static_cast<ASTPointerType*>(astType);
			std::cout << "Pointer: indirection level = " << type->indirectionLevel << std::endl;
			printASTTypeInfo(depth + 1, type->baseType);
			break;
		}
		case ASTTypeKind::Struct:
		{
			ASTStructType* type = static_cast<ASTStructType*>(astType);
			std::cout << "Struct " << std::get<std::string>(type->identifier.value) << std::endl;
		}
	}
}

void ASTPrinter::visitVarDecl(ASTVarDecl& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Var Decl Node] " << std::get<std::string>(node.varIdentifier.value) << "\n";
	printTypeInfo(depth + 1, node.typeInfo);
	printASTTypeInfo(depth + 1, node.type);

	if (node.initialization)
		node.initialization->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(null variable initializer)");
}

void ASTPrinter::visitFuncDecl(ASTFuncDecl& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Func Decl Node] " << std::get<std::string>(node.funcIdentifier.value) << std::endl;
	node.params->accept(*this, depth + 1);
	node.body->accept(*this, depth + 1);
	std::cout << indent << "[Return Type]\n";
	printTypeInfo(depth + 1, node.typeInfo->returnType);
}

void ASTPrinter::visitIdentifier(ASTIdentifier& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Identifier Node] " << std::get<std::string>(node.identifier.value) << std::endl;
	printTypeInfo(depth + 1, node.typeInfo);
}

void ASTPrinter::visitExprStmt(ASTExprStmt& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Expr Stmt Node]\n";
	node.expression->accept(*this, depth + 1);
}

void ASTPrinter::visitAssign(ASTAssign& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Assign Node] ";

	switch (node.op.type)
	{
		case TokenType::Assign:                  std::cout << "=\n";   break;
		case TokenType::PlusEquals:              std::cout << "+=\n";  break;
		case TokenType::MinusEquals:             std::cout << "-=\n";  break;
		case TokenType::TimesEquals:             std::cout << "*=\n";  break;
		case TokenType::DividedEquals:           std::cout << "/=\n";  break;
		case TokenType::BitwiseAndEquals:        std::cout << "&=\n";  break;
		case TokenType::BitwiseOrEquals:         std::cout << "|=\n";  break;
		case TokenType::BitwiseXorEquals:        std::cout << "^=\n";  break;
		case TokenType::BitwiseLeftShiftEquals:  std::cout << "<<=\n"; break;
		case TokenType::BitwiseRightShiftEquals: std::cout << ">>=\n"; break;
	}

	node.assignee->accept(*this, depth + 1);
	node.value->accept(*this, depth + 1);
}

void ASTPrinter::visitReturn(ASTReturn& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Return Node]\n";

	// the return value can be null if the function returns void
	if (node.returnVal)
		node.returnVal->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(void)");
}

void ASTPrinter::visitBlock(ASTBlock& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Block Node Start]\n";

	for (ASTNode* statement : node.statements)
		statement->accept(*this, depth + 1);

	std::cout << indent << "[Block Node End]\n";
}

void ASTPrinter::visitForLoop(ASTForLoop& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[For Loop Node]\n";

	if (node.initializer)
		node.initializer->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(null initializer)");

	if (node.condition)
		node.condition->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(null condition)");

	if (node.increment)
		node.increment->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(null increment)");

	node.body->accept(*this, depth + 1);
}

void ASTPrinter::visitWhileLoop(ASTWhileLoop& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[While Loop Node]\n";
	node.condition->accept(*this, depth + 1);
	node.body->accept(*this, depth + 1);
}

void ASTPrinter::visitIfStatement(ASTIfStatement& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[If Statement Node]\n";
	node.condition->accept(*this, depth + 1);

	node.trueBranch->accept(*this, depth + 1);

	if (node.falseBranch)
		node.falseBranch->accept(*this, depth + 1);
	else
		printNull(depth + 1, "(no false branch)");
}

void ASTPrinter::visitLogical(ASTLogical& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Logical Node] ";

	switch (node.logicalOperator.type)
	{
		case TokenType::LogicalAnd: std::cout << "&&\n"; break;
		case TokenType::LogicalOr:  std::cout << "||\n"; break;
	}

	node.lhs->accept(*this, depth + 1);
	if (node.rhs)
		node.rhs->accept(*this, depth + 1);
	else
		std::cout << "(no rhs)\n";
}

void ASTPrinter::visitBinaryExpr(ASTBinaryExpr& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Binary Expr Node] ";

	switch (node.op.type)
	{
		case TokenType::Plus:              std::cout << "+\n"; break;
		case TokenType::Minus:             std::cout << "-\n"; break;
		case TokenType::Asterisk:          std::cout << "*\n"; break;
		case TokenType::ForwardSlash:      std::cout << "/\n"; break;
		case TokenType::NotEq:             std::cout << "!=\n"; break;
		case TokenType::Equality:          std::cout << "==\n"; break;
		case TokenType::GreaterThan:       std::cout << ">\n"; break;
		case TokenType::GreaterThanEq:     std::cout << ">=\n"; break;
		case TokenType::LessThan:          std::cout << "<\n"; break;
		case TokenType::LessThanEq:        std::cout << "<=\n"; break;
		case TokenType::BitwiseAnd:        std::cout << "&\n"; break;
		case TokenType::BitwiseOr:         std::cout << "|\n"; break;
		case TokenType::BitwiseXor:        std::cout << "^\n"; break;
		case TokenType::BitwiseLeftShift:  std::cout << "<<\n"; break;
		case TokenType::BitwiseRightShift: std::cout << ">>\n"; break;
	}

	printTypeInfo(depth + 1, node.typeInfo);

	node.lhs->accept(*this, depth + 1);
	node.rhs->accept(*this, depth + 1);
}

void ASTPrinter::visitUnaryExpr(ASTUnaryExpr& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Unary Expr Node] ";
	
	switch (node.op.type)
	{
		case TokenType::LogicalNot: std::cout << "!\n"; break;
		case TokenType::Minus:      std::cout << "-\n"; break;
		case TokenType::BitwiseNot: std::cout << "~\n"; break;
		case TokenType::Increment:  std::cout << "++\n"; break;
		case TokenType::Decrement:  std::cout << "--\n"; break;
		case TokenType::BitwiseAnd: std::cout << "&\n"; break;
		case TokenType::Asterisk:   std::cout << "*\n"; break;
	}

	node.expr->accept(*this, depth + 1);
}

void ASTPrinter::visitCall(ASTCall& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Call Node]\n";
	node.callee->accept(*this, depth + 1);
	node.args->accept(*this, depth + 1);
}

void ASTPrinter::visitGroupExpr(ASTGroupExpr& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Group Expr Node]\n";
	node.expr->accept(*this, depth + 1);
}

void ASTPrinter::visitPostfix(ASTPostfix& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Postfix Node] ";

	switch (node.op.type)
	{
		case TokenType::Increment: std::cout << "++\n"; break;
		case TokenType::Decrement: std::cout << "--\n"; break;
	}

	node.expr->accept(*this, depth + 1);
}

void ASTPrinter::visitParameter(ASTParameter& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Parameter Node] " << std::get<std::string>(node.paramIdentifier.value);
	printASTTypeInfo(depth + 1, node.type);
	//switch (node.paramType.type)
	//{
	//	case TokenType::u8Keyword:   std::cout << " : u8\n";   break;
	//	case TokenType::u16Keyword:  std::cout << " : u16\n";  break;
	//	case TokenType::u32Keyword:  std::cout << " : u32\n";  break;
	//	case TokenType::u64Keyword:  std::cout << " : u64\n";  break;
	//	case TokenType::i8Keyword:   std::cout << " : i8\n";   break;
	//	case TokenType::i16Keyword:  std::cout << " : i16\n";  break;
	//	case TokenType::i32Keyword:  std::cout << " : i32\n";  break;
	//	case TokenType::i64Keyword:  std::cout << " : i64\n";  break;
	//	case TokenType::f32Keyword:  std::cout << " : f32\n";  break;
	//	case TokenType::f64Keyword:  std::cout << " : f64\n";  break;
	//	case TokenType::charKeyword: std::cout << " : char\n"; break;
	//	case TokenType::boolKeyword: std::cout << " : bool\n"; break;

	//	// if the type token isn't a built in type, then its a user defined type so it will be stored in the tokens std::variant<std::string> value
	//	default:
	//		std::cout << " : " << std::get<std::string>(node.paramType.value) << std::endl;
	//}
}

void ASTPrinter::visitArgument(ASTArgument& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Argument Node]\n";
	node.value->accept(*this, depth + 1);
}

void ASTPrinter::visitParamList(ASTParamList& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Params Start]\n";

	for (ASTParameter* param : node.params)
		param->accept(*this, depth + 1);

	std::cout << indent << "[Params End]\n";
}

void ASTPrinter::visitArgList(ASTArgList& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Args Start]\n";

	for (ASTArgument* arg: node.args)
		arg->accept(*this, depth + 1);

	std::cout << indent << "[Args End]\n";
}

void ASTPrinter::visitCast(ASTCast& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Cast Node]\n";
	printTypeInfo(depth + 1, node.typeInfo);
	node.expr->accept(*this, depth + 1);
}

void ASTPrinter::visitArrayAccess(ASTArrayAccess& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Array Access Node]\n";
	printTypeInfo(depth + 1, node.typeInfo);
	node.arrayExpr->accept(*this, depth + 1);
	node.indexExpr->accept(*this, depth + 1);
}

void ASTPrinter::visitStructDecl(ASTStructDecl& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');

	std::cout << indent << "[Struct Declaration Node] " << std::get<std::string>(node.type->identifier.value) << std::endl;
	std::cout << indent << "[Fields]\n";

	for (ASTVarDecl* field : node.fields)
		field->accept(*this, depth + 1);
}

void ASTPrinter::visitFieldAccess(ASTFieldAccess& node, uint32_t depth)
{
	std::string indent(SPACES_PER_DEPTH * depth, ' ');
	std::cout << indent << "[Field Access Node] " << node.typeInfo->typeName << "." << std::get<std::string>(node.fieldIdentifier.value) << std::endl;
	node.structExpr->accept(*this, depth + 1);
}
