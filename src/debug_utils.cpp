#include "debug_utils.h"

namespace DebugUtils
{
	const char* typeKindToString(TypeKind type)
	{
		switch (type)
		{
			case TypeKind::u8:   return "u8";
			case TypeKind::u16:  return "u16";
			case TypeKind::u32:  return "u32";
			case TypeKind::u64:  return "u64";
			case TypeKind::i8:   return "i8";
			case TypeKind::i16:  return "i16";
			case TypeKind::i32:  return "i32";
			case TypeKind::i64:  return "i64";
			case TypeKind::f32:  return "f32";
			case TypeKind::f64:  return "f64";
			case TypeKind::Char: return "char";
			case TypeKind::Bool: return "bool";
			case TypeKind::Void: return "void";
			case TypeKind::Function: return "function";
			default:
				return "unknown";
		}
	}

	const char* tokenTypeToString(const Token& token)
	{
		switch (token.type)
		{
			case TokenType::None:                    return "none"; break;
			case TokenType::i8Keyword:		         return "i8"; break;
			case TokenType::i16Keyword:		         return "i16"; break;
			case TokenType::i32Keyword:		         return "i32"; break;
			case TokenType::i64Keyword:		         return "i64"; break;
			case TokenType::u8Keyword:		         return "u8"; break;
			case TokenType::u16Keyword:		         return "u16"; break;
			case TokenType::u32Keyword:		         return "u32"; break;
			case TokenType::u64Keyword:		         return "u64"; break;
			case TokenType::f32Keyword:		         return "f32"; break;
			case TokenType::f64Keyword:		         return "f64"; break;
			case TokenType::charKeyword:	         return "char"; break;
			case TokenType::boolKeyword:	         return "bool"; break;
			case TokenType::falseKeyword:	         return "false"; break;
			case TokenType::trueKeyword:	         return "true"; break;
			case TokenType::typeKeyword:	         return "type"; break;
			case TokenType::funcKeyword:	         return "func"; break;
			case TokenType::returnKeyword:	         return "return"; break;
			case TokenType::breakKeyword:	         return "break"; break;
			case TokenType::ifKeyword:		         return "if"; break;
			case TokenType::forKeyword:		         return "for"; break;
			case TokenType::whileKeyword:	         return "while"; break;
			case TokenType::elseKeyword:	         return "else"; break;
			case TokenType::continueKeyword:         return "continue"; break;
			case TokenType::IntLiteral:		         return "int literal"; break;
			case TokenType::FloatLiteral:	         return "float literal"; break;
			case TokenType::CharLiteral:	         return "char literal"; break;
			case TokenType::Identifier:		         return "identifier"; break;
			case TokenType::Semicolon:		         return ";"; break;
			case TokenType::Assign:			         return "="; break;
			case TokenType::Invalid:		         return "invalid"; break;
			case TokenType::LessThan:		         return "<"; break;
			case TokenType::LessThanEq:		         return "<="; break;
			case TokenType::GreaterThan:	         return ">"; break;
			case TokenType::GreaterThanEq:	         return ">="; break;
			case TokenType::LogicalNot:		         return "!"; break;
			case TokenType::NotEq:			         return "!="; break;
			case TokenType::Equality:		         return "=="; break;
			case TokenType::OpenParen:		         return "("; break;
			case TokenType::CloseParen:		         return ")"; break;
			case TokenType::OpenCurly:		         return "{"; break;
			case TokenType::CloseCurly:		         return "}"; break;
			case TokenType::Colon:			         return ":"; break;
			case TokenType::Comma:			         return ","; break;
			case TokenType::Plus:			         return "+"; break;
			case TokenType::Minus:			         return "-"; break;
			case TokenType::Asterisk:		         return "*"; break;
			case TokenType::ForwardSlash:	         return "/"; break;
			case TokenType::LogicalAnd:              return "&&"; break;
			case TokenType::LogicalOr:               return "||"; break;
			case TokenType::BitwiseAnd:              return "&"; break;
			case TokenType::BitwiseOr:               return "|"; break;
			case TokenType::BitwiseXor:              return "^"; break;
			case TokenType::BitwiseNot:              return "~"; break;
			case TokenType::BitwiseLeftShift:        return "<<"; break;
			case TokenType::BitwiseRightShift:       return ">>"; break;
			case TokenType::Increment:               return "++"; break;
			case TokenType::Decrement:               return "--"; break;
			case TokenType::PlusEquals:              return "+="; break;
			case TokenType::MinusEquals:             return "-="; break;
			case TokenType::TimesEquals:             return "*="; break;
			case TokenType::DividedEquals:           return "/-"; break;
			case TokenType::DotOperator:             return "."; break;
			case TokenType::OpenBracket:             return "["; break;
			case TokenType::CloseBracket:            return "]"; break;
			case TokenType::BitwiseAndEquals:        return "&="; break;
			case TokenType::BitwiseOrEquals:         return "|="; break;
			case TokenType::BitwiseXorEquals:        return "^="; break;
			case TokenType::BitwiseLeftShiftEquals:  return "<<="; break;
			case TokenType::BitwiseRightShiftEquals: return ">>="; break;
			case TokenType::Comment:                 return "comment"; break;
			case TokenType::Eof:			         return "EOF"; break;
		}
	}
}