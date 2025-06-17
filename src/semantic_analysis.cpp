#include "semantic_analysis.h"
#include "debug_utils.h"

#include <iostream>
#include <limits>

TypeKind SemanticAnalysis::getTypeFromToken(TokenType token)
{
	switch (token)
	{
		case TokenType::u8Keyword:   return TypeKind::u8;     
		case TokenType::u16Keyword:  return TypeKind::u16;    
		case TokenType::u32Keyword:  return TypeKind::u32;    
		case TokenType::u64Keyword:  return TypeKind::u64;    
		case TokenType::i8Keyword:   return TypeKind::i8;     
		case TokenType::i16Keyword:  return TypeKind::i16;    
		case TokenType::i32Keyword:  return TypeKind::i32;    
		case TokenType::i64Keyword:  return TypeKind::i64;    
		case TokenType::f32Keyword:  return TypeKind::f32;    
		case TokenType::f64Keyword:  return TypeKind::f64;    
		case TokenType::charKeyword: return TypeKind::Char;   
		case TokenType::boolKeyword: return TypeKind::Bool;   
		case TokenType::typeKeyword: return TypeKind::Struct; 
		default:
			return TypeKind::Unknown;
	}
}

bool SemanticAnalysis::fitsInIntegerType(uint64_t value, TypeKind type)
{
	switch (type)
	{
		case TypeKind::i8:  return value <= static_cast<uint64_t>(std::numeric_limits<int8_t>::max());
		case TypeKind::i16: return value <= static_cast<uint64_t>(std::numeric_limits<int16_t>::max());
		case TypeKind::i32: return value <= static_cast<uint64_t>(std::numeric_limits<int32_t>::max());
		case TypeKind::i64: return value <= static_cast<uint64_t>(std::numeric_limits<int64_t>::max());
		case TypeKind::u8:  return value <= std::numeric_limits<uint8_t>::max();
		case TypeKind::u16: return value <= std::numeric_limits<uint16_t>::max();
		case TypeKind::u32: return value <= std::numeric_limits<uint32_t>::max();
		case TypeKind::u64: return value <= std::numeric_limits<uint64_t>::max();
		default: 
			return false;
	}
}

bool SemanticAnalysis::isAssignable(TypeKind lhs, TypeKind rhs)
{
	/*

	any declared type basically has to match with a type of equal wideness or smaller wideness
	basically checking if type is smaller than or equal to
	cases:
		u8 matches with i8
		u16 matches with i8 or i16
		u32 matches with i8, i16, or i32
		u64 matches with any

		i8 matches with i8
		i16 matches with i8 or 16
		i32 matches with i8, i16, or i32
		i64 matches with any

	*/
	
	switch (lhs)
	{
		case TypeKind::u8:
		case TypeKind::i8:
		{
			return (rhs == TypeKind::u8 || rhs == TypeKind::i8);
		}

		case TypeKind::u16:
		case TypeKind::i16:
		{
			return (rhs == TypeKind::u8 || rhs == TypeKind::i8 
				|| rhs == TypeKind::u16 || rhs == TypeKind::i16);
		}

		case TypeKind::u32:
		case TypeKind::i32:
		{
			return (rhs == TypeKind::u8 || rhs == TypeKind::i8 
				|| rhs == TypeKind::u16 || rhs == TypeKind::i16
				|| rhs == TypeKind::u32 || rhs == TypeKind::i32);
		}

		case TypeKind::u64:
		case TypeKind::i64:
		{
			return (rhs == TypeKind::u8 || rhs == TypeKind::i8
				|| rhs == TypeKind::u16 || rhs == TypeKind::i16
				|| rhs == TypeKind::u32 || rhs == TypeKind::i32
				|| rhs == TypeKind::u64 || rhs == TypeKind::i64);
		}

		case TypeKind::f32:
			return (rhs == TypeKind::f32);
		case TypeKind::f64:
			return (rhs == TypeKind::f32 || rhs == TypeKind::f64);

		case TypeKind::Bool:
			return (rhs == TypeKind::Bool);

		case TypeKind::Char:
			return (rhs == TypeKind::Char);

		default:
			return lhs == rhs;
	}
}

void SemanticAnalysis::printInfo()
{
	env.dumpEnvironment();
}

void SemanticAnalysis::visitIntLiteral(ASTIntLiteral& node)
{
	/*
	
	Idea is to have int literals default to the smallest container possible that fits the value
	and assignment / variable declarations can widen the type if needed
	
	*/

	node.typeInfo = arena.alloc<TypeInfo>();
	if (node.value <= std::numeric_limits<int8_t>::max()) 
		node.typeInfo->type = TypeKind::i8;
	else if (node.value <= std::numeric_limits<int16_t>::max())
		node.typeInfo->type = TypeKind::i16;
	else if (node.value <= std::numeric_limits<int32_t>::max())
		node.typeInfo->type = TypeKind::i32;
	else
		node.typeInfo->type = TypeKind::i64;
}

void SemanticAnalysis::visitDoubleLiteral(ASTDoubleLiteral& node)
{
	node.typeInfo = arena.alloc<TypeInfo>();
	float castedFloat = static_cast<float>(node.value);
	double castedBack = static_cast<double>(castedFloat);

	if (std::fabs(node.value - castedBack) <= (std::numeric_limits<float>::epsilon() * std::fabs(node.value)))
		node.typeInfo->type = TypeKind::f32;
	else
		node.typeInfo->type = TypeKind::f64;
}

void SemanticAnalysis::visitCharLiteral(ASTCharLiteral& node)
{
	node.typeInfo = arena.alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Char;
}

void SemanticAnalysis::visitBoolLiteral(ASTBoolLiteral& node)
{
	node.typeInfo = arena.alloc<TypeInfo>();
	node.typeInfo->type = TypeKind::Bool;
}

void SemanticAnalysis::visitVarDecl(ASTVarDecl& node)
{
	SymbolTable& table = env.getCurrentScope();
	std::string& identifier = std::get<std::string>(node.varIdentifier.value);
	
	// error, defining a variable that is already defined, swap to better error reporting system eventually
	if (table.isDefined(identifier))
	{
		std::cout << "Error, defining duplicate variables with name: " << identifier << std::endl;
		return;
	}

	node.typeInfo = arena.alloc<TypeInfo>();
	node.typeInfo->name = identifier;
	node.typeInfo->type = getTypeFromToken(node.varType);
	
	// if the node has an initialization, make sure the type of this variable is compatible with the type of the initialization expression
	if (node.initialization)
	{
		// now we know what type is declared for the variable, and what type was inferred for the initialization. Now we need to check
		// if they are compatible
		node.initialization->accept(*this);

		if (!isAssignable(node.typeInfo->type, node.initialization->typeInfo->type))
		{
			std::cout << "Error, incompatible types for: " << identifier << std::endl;
		}
	}

	node.scope = env.getScopeDepth();
	node.slotIndex = table.declare(identifier, node.typeInfo, node.scope);
}

void SemanticAnalysis::visitFuncDecl(ASTFuncDecl& node)
{
	SymbolTable& table = env.getCurrentScope();
	std::string& funcIdentifier = std::get<std::string>(node.funcIdentifier.value);

	if (table.isDefined(funcIdentifier))
	{
		std::cout << "Error, defining duplicate functions with name: " << funcIdentifier << std::endl;
		return;
	}

	env.pushScope();
	SymbolTable& funcScope = env.getCurrentScope();

	node.typeInfo = arena.alloc<TypeInfo>();
	node.typeInfo->returnType = arena.alloc<TypeInfo>();
	node.typeInfo->name = funcIdentifier;
	node.typeInfo->type = TypeKind::Function;
	node.typeInfo->returnType->type = getTypeFromToken(node.returnType.type);
	funcScope.declare(funcIdentifier, node.typeInfo, env.getScopeDepth());

	// set the semantic analysis functionCtx to the current ASTFuncDecl's type information
	functionCtx = node.typeInfo;
	visitParamList(*node.params);

	// when visiting the body, ASTBlock should not create a scope since the functionDecl node itself is responsible for creating the scope
	node.body->createScope = false;
	visitBlock(*node.body);
	
	// after visiting param list / body / whatever other functionCtx related information, reset the ctx back to nullptr
	functionCtx = nullptr;
	std::cout << "DUMPING ENV FROM FUNCTION DECLARATION NODE\n";
	env.dumpEnvironment();
	env.popScope();
}

void SemanticAnalysis::visitVariable(ASTVariable& node)
{

}

void SemanticAnalysis::visitExprStmt(ASTExprStmt& node)
{

}

void SemanticAnalysis::visitAssign(ASTAssign& node)
{
	
}

void SemanticAnalysis::visitReturn(ASTReturn& node)
{
	// if there's no functionCtx, then we're visiting a return statement outside of a function body which is not allowed
	if (!functionCtx)
	{
		std::cout << "Error, return statements must be inside function body\n";
		return;
	}

	// if functionCtx is valid, we need to resolve the type of the return value and make sure it matches the function signatures return type
	node.returnVal->accept(*this);
	if (node.returnVal->typeInfo->type != functionCtx->returnType->type)
	{
		std::cout << "Error, return type does not match function declaration. Returning "
			<< DebugUtils::typeKindToString(node.returnVal->typeInfo->type)
			<< " when " << DebugUtils::typeKindToString(functionCtx->returnType->type)
			<< " is expected\n";
		return;
	}
}

void SemanticAnalysis::visitBlock(ASTBlock& node)
{
	std::cout << "visit block during semantic analysis, create scope flag = " << node.createScope << std::endl;
	if (node.createScope)
		env.pushScope();

	for (ASTStmt* stmt : node.statements)
		stmt->accept(*this);

	env.dumpEnvironment();
	if (node.createScope)
		env.popScope();
}

void SemanticAnalysis::visitForLoop(ASTForLoop& node)
{
	env.pushScope();
	
	if (node.initializer)
		node.initializer->accept(*this);

	if (node.condition)
	{
		node.condition->accept(*this);
		if (node.condition->typeInfo->type != TypeKind::Bool)
			std::cout << "Error, for loop condition must be of type bool\n";
	}

	if (node.increment)
		node.increment->accept(*this);

	node.body->createScope = false;
	visitBlock(*node.body);

	env.dumpEnvironment();
	env.popScope();
}

void SemanticAnalysis::visitIfStatement(ASTIfStatement& node)
{
	node.condition->accept(*this);
	if (node.condition->typeInfo->type != TypeKind::Bool)
	{
		std::cout << "Error, if statement condition must be of type bool\n";
		return;
	}

	node.trueBranch->accept(*this);
	if (node.falseBranch)
		node.falseBranch->accept(*this);
}

void SemanticAnalysis::visitLogical(ASTLogical& node)
{

}

void SemanticAnalysis::visitBinaryExpr(ASTBinaryExpr& node)
{

}

void SemanticAnalysis::visitUnaryExpr(ASTUnaryExpr& node)
{

}

void SemanticAnalysis::visitCall(ASTCall& node)
{

}

void SemanticAnalysis::visitGroupExpr(ASTGroupExpr& node)
{

}

void SemanticAnalysis::visitPostfix(ASTPostfix& node)
{

}

void SemanticAnalysis::visitParameter(ASTParameter& node)
{
	node.typeInfo = arena.alloc<TypeInfo>();
	node.typeInfo->name = std::get<std::string>(node.paramIdentifier.value);
	node.typeInfo->type = getTypeFromToken(node.paramType.type);
}

void SemanticAnalysis::visitArgument(ASTArgument& node)
{

}

void SemanticAnalysis::visitParamList(ASTParamList& node)
{
	// if functionCtx is a valid pointer, we know this parameter list belongs to a function declaration
	if (functionCtx)
	{
		SymbolTable& table = env.getCurrentScope();
		for (ASTParameter* param : node.params)
		{
			visitParameter(*param);
			std::string& paramIdentifier = param->typeInfo->name;
			
			if (table.isDefined(paramIdentifier))
				std::cout << "Error, duplicate parameter name \'" << paramIdentifier << "\' in \'" << functionCtx->name << "\'()\n";

			table.declare(paramIdentifier, param->typeInfo, env.getScopeDepth());
			functionCtx->paramTypes.emplace_back(param->typeInfo);
		}
	}
}

void SemanticAnalysis::visitArgList(ASTArgList& node)
{

}
