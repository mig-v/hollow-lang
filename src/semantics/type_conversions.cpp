#include "semantics/type_conversions.h"

TypeConversions::TypeConversions()
{
	// types -> i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, char, bool, void, function, struct, unknown

	// i8 conversions
	addConversion(TypeKind::i8, TypeKind::i8, ConversionType::SameType);
	addConversion(TypeKind::i8, TypeKind::i16, ConversionType::Widening);
	addConversion(TypeKind::i8, TypeKind::i32, ConversionType::Widening);
	addConversion(TypeKind::i8, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i8, TypeKind::u8, ConversionType::Narrowing, "narrowing i8 to u8");
	addConversion(TypeKind::i8, TypeKind::u16, ConversionType::Narrowing, "narrowing i8 to u16");
	addConversion(TypeKind::i8, TypeKind::u32, ConversionType::Narrowing, "narrowing i8 to u32");
	addConversion(TypeKind::i8, TypeKind::u64, ConversionType::Narrowing, "narrowing i8 to u64");

	addConversion(TypeKind::i8, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to f32");
	addConversion(TypeKind::i8, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to f64");

	addConversion(TypeKind::i8, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i8 to char");
	addConversion(TypeKind::i8, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i8 to bool");
	addConversion(TypeKind::i8, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i8 to void");
	addConversion(TypeKind::i8, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i8 to function");
	addConversion(TypeKind::i8, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i8 to struct");
	addConversion(TypeKind::i8, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i8 to unknown");
	addConversion(TypeKind::i8, TypeKind::Array, ConversionType::NotAllowed, "cannot convert i8 to array");
	addConversion(TypeKind::i8, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert i8 to pointer");

	// i16 conversions
	addConversion(TypeKind::i16, TypeKind::i8, ConversionType::Narrowing, "narrowing i16 to i8");
	addConversion(TypeKind::i16, TypeKind::i16, ConversionType::SameType);
	addConversion(TypeKind::i16, TypeKind::i32, ConversionType::Widening);
	addConversion(TypeKind::i16, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i16, TypeKind::u8, ConversionType::Narrowing, "narrowing i16 to u8");
	addConversion(TypeKind::i16, TypeKind::u16, ConversionType::Narrowing, "narrowing i16 to u16");
	addConversion(TypeKind::i16, TypeKind::u32, ConversionType::Narrowing, "narrowing i16 to u32");
	addConversion(TypeKind::i16, TypeKind::u64, ConversionType::Narrowing, "narrowing i16 to u64");

	addConversion(TypeKind::i16, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to f32");
	addConversion(TypeKind::i16, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to f64");

	addConversion(TypeKind::i16, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i16 to char");
	addConversion(TypeKind::i16, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i16 to bool");
	addConversion(TypeKind::i16, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i16 to void");
	addConversion(TypeKind::i16, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i16 to function");
	addConversion(TypeKind::i16, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i16 to struct");
	addConversion(TypeKind::i16, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i16 to unknown");
	addConversion(TypeKind::i16, TypeKind::Array, ConversionType::NotAllowed, "cannot convert i16 to array");
	addConversion(TypeKind::i16, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert i16 to pointer");

	// i32 conversions
	addConversion(TypeKind::i32, TypeKind::i8, ConversionType::Narrowing, "narrowing i32 to i8");
	addConversion(TypeKind::i32, TypeKind::i16, ConversionType::Narrowing, "narrowing i32 to i16");
	addConversion(TypeKind::i32, TypeKind::i32, ConversionType::SameType);
	addConversion(TypeKind::i32, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i32, TypeKind::u8, ConversionType::Narrowing, "narrowing i32 to u8");
	addConversion(TypeKind::i32, TypeKind::u16, ConversionType::Narrowing, "narrowing i32 to u16");
	addConversion(TypeKind::i32, TypeKind::u32, ConversionType::Narrowing, "narrowing i32 to u32");
	addConversion(TypeKind::i32, TypeKind::u64, ConversionType::Narrowing, "narrowing i32 to u64");

	addConversion(TypeKind::i32, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to f32");
	addConversion(TypeKind::i32, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to f64");

	addConversion(TypeKind::i32, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i32 to char");
	addConversion(TypeKind::i32, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i32 to bool");
	addConversion(TypeKind::i32, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i32 to void");
	addConversion(TypeKind::i32, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i32 to function");
	addConversion(TypeKind::i32, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i32 to struct");
	addConversion(TypeKind::i32, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i32 to unknown");
	addConversion(TypeKind::i32, TypeKind::Array, ConversionType::NotAllowed, "cannot convert i32 to array");
	addConversion(TypeKind::i32, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert i32 to pointer");

	// i64 conversions
	addConversion(TypeKind::i64, TypeKind::i8, ConversionType::Narrowing, "narrowing i64 to i8");
	addConversion(TypeKind::i64, TypeKind::i16, ConversionType::Narrowing, "narrowing i64 to i16");
	addConversion(TypeKind::i64, TypeKind::i32, ConversionType::Narrowing, "narrowing i64 to i32");
	addConversion(TypeKind::i64, TypeKind::i64, ConversionType::SameType);

	addConversion(TypeKind::i64, TypeKind::u8, ConversionType::Narrowing, "narrowing i64 to u8");
	addConversion(TypeKind::i64, TypeKind::u16, ConversionType::Narrowing, "narrowing i64 to u16");
	addConversion(TypeKind::i64, TypeKind::u32, ConversionType::Narrowing, "narrowing i64 to u32");
	addConversion(TypeKind::i64, TypeKind::u64, ConversionType::Narrowing, "narrowing i64 to u64");

	addConversion(TypeKind::i64, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to f32");
	addConversion(TypeKind::i64, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to f64");

	addConversion(TypeKind::i64, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i64 to char");
	addConversion(TypeKind::i64, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i64 to bool");
	addConversion(TypeKind::i64, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i64 to void");
	addConversion(TypeKind::i64, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i64 to function");
	addConversion(TypeKind::i64, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i64 to struct");
	addConversion(TypeKind::i64, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i64 to unknown");
	addConversion(TypeKind::i64, TypeKind::Array, ConversionType::NotAllowed, "cannot convert i64 to array");
	addConversion(TypeKind::i64, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert i64 to pointer");

	// u8 conversions
	addConversion(TypeKind::u8, TypeKind::i8, ConversionType::Narrowing, "narrowing u8 to i8");
	addConversion(TypeKind::u8, TypeKind::i16, ConversionType::Narrowing, "narrowing u16 to i16");
	addConversion(TypeKind::u8, TypeKind::i32, ConversionType::Narrowing, "narrowing u32 to i32");
	addConversion(TypeKind::u8, TypeKind::i64, ConversionType::Narrowing, "narrowing u64 to i64");

	addConversion(TypeKind::u8, TypeKind::u8, ConversionType::SameType);
	addConversion(TypeKind::u8, TypeKind::u16, ConversionType::Widening);
	addConversion(TypeKind::u8, TypeKind::u32, ConversionType::Widening);
	addConversion(TypeKind::u8, TypeKind::u64, ConversionType::Widening);

	addConversion(TypeKind::u8, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert u8 to f32");
	addConversion(TypeKind::u8, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert u8 to f64");

	addConversion(TypeKind::u8, TypeKind::Char, ConversionType::NotAllowed, "cannot convert u8 to char");
	addConversion(TypeKind::u8, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert u8 to bool");
	addConversion(TypeKind::u8, TypeKind::Void, ConversionType::NotAllowed, "cannot convert u8 to void");
	addConversion(TypeKind::u8, TypeKind::Function, ConversionType::NotAllowed, "cannot convert u8 to function");
	addConversion(TypeKind::u8, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert u8 to struct");
	addConversion(TypeKind::u8, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert u8 to unknown");
	addConversion(TypeKind::u8, TypeKind::Array, ConversionType::NotAllowed, "cannot convert u8 to array");
	addConversion(TypeKind::u8, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert u8 to pointer");

	// u16 conversions
	addConversion(TypeKind::u16, TypeKind::i8, ConversionType::Narrowing, "narrowing u16 to i8");
	addConversion(TypeKind::u16, TypeKind::i16, ConversionType::Narrowing, "narrowing u16 to i16");
	addConversion(TypeKind::u16, TypeKind::i32, ConversionType::Narrowing, "narrowing u16 to i32");
	addConversion(TypeKind::u16, TypeKind::i64, ConversionType::Narrowing, "narrowing u16 to i64");

	addConversion(TypeKind::u16, TypeKind::u8, ConversionType::Narrowing, "narrowing u16 to u8");
	addConversion(TypeKind::u16, TypeKind::u16, ConversionType::SameType);
	addConversion(TypeKind::u16, TypeKind::u32, ConversionType::Widening);
	addConversion(TypeKind::u16, TypeKind::u64, ConversionType::Widening);

	addConversion(TypeKind::u16, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to f32");
	addConversion(TypeKind::u16, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to f64");

	addConversion(TypeKind::u16, TypeKind::Char, ConversionType::NotAllowed, "cannot convert u16 to char");
	addConversion(TypeKind::u16, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert u16 to bool");
	addConversion(TypeKind::u16, TypeKind::Void, ConversionType::NotAllowed, "cannot convert u16 to void");
	addConversion(TypeKind::u16, TypeKind::Function, ConversionType::NotAllowed, "cannot convert u16 to function");
	addConversion(TypeKind::u16, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert u16 to struct");
	addConversion(TypeKind::u16, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert u16 to unknown");
	addConversion(TypeKind::u16, TypeKind::Array, ConversionType::NotAllowed, "cannot convert u16 to array");
	addConversion(TypeKind::u16, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert u16 to pointer");

	// u32 conversions
	addConversion(TypeKind::u32, TypeKind::i8, ConversionType::Narrowing, "narrowing u32 to i8");
	addConversion(TypeKind::u32, TypeKind::i16, ConversionType::Narrowing, "narrowing u32 to i16");
	addConversion(TypeKind::u32, TypeKind::i32, ConversionType::Narrowing, "narrowing u32 to i32");
	addConversion(TypeKind::u32, TypeKind::i64, ConversionType::Narrowing, "narrowing u32 to i64");

	addConversion(TypeKind::u32, TypeKind::u8, ConversionType::Narrowing, "narrowing u32 to u8");
	addConversion(TypeKind::u32, TypeKind::u16, ConversionType::Narrowing, "narrowing u32 to u16");
	addConversion(TypeKind::u32, TypeKind::u32, ConversionType::SameType);
	addConversion(TypeKind::u32, TypeKind::u64, ConversionType::Widening);

	addConversion(TypeKind::u32, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to f32");
	addConversion(TypeKind::u32, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to f64");

	addConversion(TypeKind::u32, TypeKind::Char, ConversionType::NotAllowed, "cannot convert u32 to char");
	addConversion(TypeKind::u32, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert u32 to bool");
	addConversion(TypeKind::u32, TypeKind::Void, ConversionType::NotAllowed, "cannot convert u32 to void");
	addConversion(TypeKind::u32, TypeKind::Function, ConversionType::NotAllowed, "cannot convert u32 to function");
	addConversion(TypeKind::u32, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert u32 to struct");
	addConversion(TypeKind::u32, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert u32 to unknown");
	addConversion(TypeKind::u32, TypeKind::Array, ConversionType::NotAllowed, "cannot convert u32 to array");
	addConversion(TypeKind::u32, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert u32 to pointer");

	// u64 conversions
	addConversion(TypeKind::u64, TypeKind::i8, ConversionType::Narrowing, "narrowing u64 to i8");
	addConversion(TypeKind::u64, TypeKind::i16, ConversionType::Narrowing, "narrowing u64 to i16");
	addConversion(TypeKind::u64, TypeKind::i32, ConversionType::Narrowing, "narrowing u64 to i32");
	addConversion(TypeKind::u64, TypeKind::i64, ConversionType::Narrowing, "narrowing u64 to i64");

	addConversion(TypeKind::u64, TypeKind::u8, ConversionType::Narrowing, "narrowing u64 to u8");
	addConversion(TypeKind::u64, TypeKind::u16, ConversionType::Narrowing, "narrowing u64 to u16");
	addConversion(TypeKind::u64, TypeKind::u32, ConversionType::Narrowing, "narrowing u64 to u32");
	addConversion(TypeKind::u64, TypeKind::u64, ConversionType::SameType);

	addConversion(TypeKind::u64, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to f32");
	addConversion(TypeKind::u64, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to f64");

	addConversion(TypeKind::u64, TypeKind::Char, ConversionType::NotAllowed, "cannot convert u64 to char");
	addConversion(TypeKind::u64, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert u64 to bool");
	addConversion(TypeKind::u64, TypeKind::Void, ConversionType::NotAllowed, "cannot convert u64 to void");
	addConversion(TypeKind::u64, TypeKind::Function, ConversionType::NotAllowed, "cannot convert u64 to function");
	addConversion(TypeKind::u64, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert u64 to struct");
	addConversion(TypeKind::u64, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert u64 to unknown");
	addConversion(TypeKind::u64, TypeKind::Array, ConversionType::NotAllowed, "cannot convert u64 to array");
	addConversion(TypeKind::u64, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert u64 to pointer");

	// f32 conversions
	addConversion(TypeKind::f32, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to i8");
	addConversion(TypeKind::f32, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to i16");
	addConversion(TypeKind::f32, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to i32");
	addConversion(TypeKind::f32, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to i64");

	addConversion(TypeKind::f32, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to u8");
	addConversion(TypeKind::f32, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to u16");
	addConversion(TypeKind::f32, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to u32");
	addConversion(TypeKind::f32, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert f32 to u64");

	addConversion(TypeKind::f32, TypeKind::f32, ConversionType::SameType);
	addConversion(TypeKind::f32, TypeKind::f64, ConversionType::Widening);

	addConversion(TypeKind::f32, TypeKind::Char, ConversionType::NotAllowed, "cannot convert f32 to char");
	addConversion(TypeKind::f32, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert f32 to bool");
	addConversion(TypeKind::f32, TypeKind::Void, ConversionType::NotAllowed, "cannot convert f32 to void");
	addConversion(TypeKind::f32, TypeKind::Function, ConversionType::NotAllowed, "cannot convert f32 to function");
	addConversion(TypeKind::f32, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert f32 to struct");
	addConversion(TypeKind::f32, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert f32 to unknown");
	addConversion(TypeKind::f32, TypeKind::Array, ConversionType::NotAllowed, "cannot convert f32 to array");
	addConversion(TypeKind::f32, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert f32 to pointer");

	// f64 conversions
	addConversion(TypeKind::f64, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to i8");
	addConversion(TypeKind::f64, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to i16");
	addConversion(TypeKind::f64, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to i32");
	addConversion(TypeKind::f64, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to i64");

	addConversion(TypeKind::f64, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to u8");
	addConversion(TypeKind::f64, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to u16");
	addConversion(TypeKind::f64, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to u32");
	addConversion(TypeKind::f64, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert f64 to u64");

	addConversion(TypeKind::f64, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to narrow f64 to f32");
	addConversion(TypeKind::f64, TypeKind::f64, ConversionType::SameType);

	addConversion(TypeKind::f64, TypeKind::Char, ConversionType::NotAllowed, "cannot convert f64 to char");
	addConversion(TypeKind::f64, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert f64 to bool");
	addConversion(TypeKind::f64, TypeKind::Void, ConversionType::NotAllowed, "cannot convert f64 to void");
	addConversion(TypeKind::f64, TypeKind::Function, ConversionType::NotAllowed, "cannot convert f64 to function");
	addConversion(TypeKind::f64, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert f64 to struct");
	addConversion(TypeKind::f64, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert f64 to unknown");
	addConversion(TypeKind::f64, TypeKind::Array, ConversionType::NotAllowed, "cannot convert f64 to array");
	addConversion(TypeKind::f64, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert f64 to pointer");

	// char conversions
	addConversion(TypeKind::Char, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert char to i8");
	addConversion(TypeKind::Char, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert char to i16");
	addConversion(TypeKind::Char, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert char to i32");
	addConversion(TypeKind::Char, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert char to i64");

	addConversion(TypeKind::Char, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert char to u8");
	addConversion(TypeKind::Char, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert char to u16");
	addConversion(TypeKind::Char, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert char to u32");
	addConversion(TypeKind::Char, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert char to u64");

	addConversion(TypeKind::Char, TypeKind::f32, ConversionType::NotAllowed, "cannot convert char to f32");
	addConversion(TypeKind::Char, TypeKind::f64, ConversionType::NotAllowed, "cannot convert char to f64");

	addConversion(TypeKind::Char, TypeKind::Char, ConversionType::SameType);
	addConversion(TypeKind::Char, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert char  to bool");
	addConversion(TypeKind::Char, TypeKind::Void, ConversionType::NotAllowed, "cannot convert char  to void");
	addConversion(TypeKind::Char, TypeKind::Function, ConversionType::NotAllowed, "cannot convert char  to function");
	addConversion(TypeKind::Char, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert char  to struct");
	addConversion(TypeKind::Char, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert char to unknown");
	addConversion(TypeKind::Char, TypeKind::Array, ConversionType::NotAllowed, "cannot convert char to array");
	addConversion(TypeKind::Char, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert char to pointer");

	// bool conversions
	addConversion(TypeKind::Bool, TypeKind::i8, ConversionType::NotAllowed, "cannot convert bool to i8");
	addConversion(TypeKind::Bool, TypeKind::i16, ConversionType::NotAllowed, "cannot convert bool to i16");
	addConversion(TypeKind::Bool, TypeKind::i32, ConversionType::NotAllowed, "cannot convert bool to i32");
	addConversion(TypeKind::Bool, TypeKind::i64, ConversionType::NotAllowed, "cannot convert bool to i64");

	addConversion(TypeKind::Bool, TypeKind::u8, ConversionType::NotAllowed, "cannot convert bool to u8");
	addConversion(TypeKind::Bool, TypeKind::u16, ConversionType::NotAllowed, "cannot convert bool to u16");
	addConversion(TypeKind::Bool, TypeKind::u32, ConversionType::NotAllowed, "cannot convert bool to u32");
	addConversion(TypeKind::Bool, TypeKind::u64, ConversionType::NotAllowed, "cannot convert bool to u64");

	addConversion(TypeKind::Bool, TypeKind::f32, ConversionType::NotAllowed, "cannot convert bool to f32");
	addConversion(TypeKind::Bool, TypeKind::f64, ConversionType::NotAllowed, "cannot convert bool to f64");

	addConversion(TypeKind::Bool, TypeKind::Char, ConversionType::NotAllowed, "cannot convert bool to char");
	addConversion(TypeKind::Bool, TypeKind::Bool, ConversionType::SameType);
	addConversion(TypeKind::Bool, TypeKind::Void, ConversionType::NotAllowed, "cannot convert bool to void");
	addConversion(TypeKind::Bool, TypeKind::Function, ConversionType::NotAllowed, "cannot convert bool to function");
	addConversion(TypeKind::Bool, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert bool to struct");
	addConversion(TypeKind::Bool, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert bool to unknown");
	addConversion(TypeKind::Bool, TypeKind::Array, ConversionType::NotAllowed, "cannot convert bool to array");
	addConversion(TypeKind::Bool, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert bool to pointer");

	// void conversions
	addConversion(TypeKind::Void, TypeKind::i8, ConversionType::NotAllowed, "cannot convert void to i8");
	addConversion(TypeKind::Void, TypeKind::i16, ConversionType::NotAllowed, "cannot convert void to i16");
	addConversion(TypeKind::Void, TypeKind::i32, ConversionType::NotAllowed, "cannot convert void to i32");
	addConversion(TypeKind::Void, TypeKind::i64, ConversionType::NotAllowed, "cannot convert void to i64");

	addConversion(TypeKind::Void, TypeKind::u8, ConversionType::NotAllowed, "cannot convert void to u8");
	addConversion(TypeKind::Void, TypeKind::u16, ConversionType::NotAllowed, "cannot convert void to u16");
	addConversion(TypeKind::Void, TypeKind::u32, ConversionType::NotAllowed, "cannot convert void to u32");
	addConversion(TypeKind::Void, TypeKind::u64, ConversionType::NotAllowed, "cannot convert void to u64");

	addConversion(TypeKind::Void, TypeKind::f32, ConversionType::NotAllowed, "cannot convert void to f32");
	addConversion(TypeKind::Void, TypeKind::f64, ConversionType::NotAllowed, "cannot convert void to f64");

	addConversion(TypeKind::Void, TypeKind::Char, ConversionType::NotAllowed, "cannot convert void to char");
	addConversion(TypeKind::Void, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert void to bool");
	addConversion(TypeKind::Void, TypeKind::Void, ConversionType::SameType);
	addConversion(TypeKind::Void, TypeKind::Function, ConversionType::NotAllowed, "cannot convert void to function");
	addConversion(TypeKind::Void, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert void to struct");
	addConversion(TypeKind::Void, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert void to unknown");
	addConversion(TypeKind::Void, TypeKind::Array, ConversionType::NotAllowed, "cannot convert void to array");
	addConversion(TypeKind::Void, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert void to pointer");

	// function conversions
	addConversion(TypeKind::Function, TypeKind::i8, ConversionType::NotAllowed, "cannot convert function to i8");
	addConversion(TypeKind::Function, TypeKind::i16, ConversionType::NotAllowed, "cannot convert function to i16");
	addConversion(TypeKind::Function, TypeKind::i32, ConversionType::NotAllowed, "cannot convert function to i32");
	addConversion(TypeKind::Function, TypeKind::i64, ConversionType::NotAllowed, "cannot convert function to i64");

	addConversion(TypeKind::Function, TypeKind::u8, ConversionType::NotAllowed, "cannot convert function to u8");
	addConversion(TypeKind::Function, TypeKind::u16, ConversionType::NotAllowed, "cannot convert function to u16");
	addConversion(TypeKind::Function, TypeKind::u32, ConversionType::NotAllowed, "cannot convert function to u32");
	addConversion(TypeKind::Function, TypeKind::u64, ConversionType::NotAllowed, "cannot convert function to u64");

	addConversion(TypeKind::Function, TypeKind::f32, ConversionType::NotAllowed, "cannot convert function to f32");
	addConversion(TypeKind::Function, TypeKind::f64, ConversionType::NotAllowed, "cannot convert function to f64");

	addConversion(TypeKind::Function, TypeKind::Char, ConversionType::NotAllowed, "cannot convert function to char");
	addConversion(TypeKind::Function, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert function to bool");
	addConversion(TypeKind::Function, TypeKind::Void, ConversionType::NotAllowed, "cannot convert function to void");
	addConversion(TypeKind::Function, TypeKind::Function, ConversionType::SameType);
	addConversion(TypeKind::Function, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert function to struct");
	addConversion(TypeKind::Function, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert function to unknown");
	addConversion(TypeKind::Function, TypeKind::Array, ConversionType::NotAllowed, "cannot convert function to array");
	addConversion(TypeKind::Function, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert function to pointer");

	// struct conversions
	addConversion(TypeKind::Struct, TypeKind::i8, ConversionType::NotAllowed, "cannot convert struct to i8");
	addConversion(TypeKind::Struct, TypeKind::i16, ConversionType::NotAllowed, "cannot convert struct to i16");
	addConversion(TypeKind::Struct, TypeKind::i32, ConversionType::NotAllowed, "cannot convert struct to i32");
	addConversion(TypeKind::Struct, TypeKind::i64, ConversionType::NotAllowed, "cannot convert struct to i64");

	addConversion(TypeKind::Struct, TypeKind::u8, ConversionType::NotAllowed, "cannot convert struct to u8");
	addConversion(TypeKind::Struct, TypeKind::u16, ConversionType::NotAllowed, "cannot convert struct to u16");
	addConversion(TypeKind::Struct, TypeKind::u32, ConversionType::NotAllowed, "cannot convert struct to u32");
	addConversion(TypeKind::Struct, TypeKind::u64, ConversionType::NotAllowed, "cannot convert struct to u64");

	addConversion(TypeKind::Struct, TypeKind::f32, ConversionType::NotAllowed, "cannot convert struct to f32");
	addConversion(TypeKind::Struct, TypeKind::f64, ConversionType::NotAllowed, "cannot convert struct to f64");

	addConversion(TypeKind::Struct, TypeKind::Char, ConversionType::NotAllowed, "cannot convert struct to char");
	addConversion(TypeKind::Struct, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert struct to bool");
	addConversion(TypeKind::Struct, TypeKind::Void, ConversionType::NotAllowed, "cannot convert struct to void");
	addConversion(TypeKind::Struct, TypeKind::Function, ConversionType::NotAllowed, "cannot convert struct to function");
	addConversion(TypeKind::Struct, TypeKind::Struct, ConversionType::SameType);
	addConversion(TypeKind::Struct, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert struct to unknown");
	addConversion(TypeKind::Struct, TypeKind::Array, ConversionType::NotAllowed, "cannot convert struct to array");
	addConversion(TypeKind::Struct, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert struct to pointer");

	// array conversions (revise these later, like allowing arrays of same type to be converted to another array of the same type?)
	addConversion(TypeKind::Array, TypeKind::i8, ConversionType::NotAllowed, "cannot convert array to i8");
	addConversion(TypeKind::Array, TypeKind::i16, ConversionType::NotAllowed, "cannot convert array to i16");
	addConversion(TypeKind::Array, TypeKind::i32, ConversionType::NotAllowed, "cannot convert array to i32");
	addConversion(TypeKind::Array, TypeKind::i64, ConversionType::NotAllowed, "cannot convert array to i64");

	addConversion(TypeKind::Array, TypeKind::u8, ConversionType::NotAllowed, "cannot convert array to u8");
	addConversion(TypeKind::Array, TypeKind::u16, ConversionType::NotAllowed, "cannot convert array to u16");
	addConversion(TypeKind::Array, TypeKind::u32, ConversionType::NotAllowed, "cannot convert array to u32");
	addConversion(TypeKind::Array, TypeKind::u64, ConversionType::NotAllowed, "cannot convert array to u64");

	addConversion(TypeKind::Array, TypeKind::f32, ConversionType::NotAllowed, "cannot convert array to f32");
	addConversion(TypeKind::Array, TypeKind::f64, ConversionType::NotAllowed, "cannot convert array to f64");

	addConversion(TypeKind::Array, TypeKind::Char, ConversionType::NotAllowed, "cannot convert array to char");
	addConversion(TypeKind::Array, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert array to bool");
	addConversion(TypeKind::Array, TypeKind::Void, ConversionType::NotAllowed, "cannot convert array to void");
	addConversion(TypeKind::Array, TypeKind::Function, ConversionType::NotAllowed, "cannot convert array to function");
	addConversion(TypeKind::Array, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert array to struct");
	addConversion(TypeKind::Array, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert array to unknown");
	addConversion(TypeKind::Array, TypeKind::Array, ConversionType::SameType, "cannot convert array to array");
	addConversion(TypeKind::Array, TypeKind::Pointer, ConversionType::ExplicitOnly, "explicit cast needed to convert array to pointer");

	// pointer conversions
	addConversion(TypeKind::Pointer, TypeKind::i8, ConversionType::NotAllowed, "cannot convert pointer to i8");
	addConversion(TypeKind::Pointer, TypeKind::i16, ConversionType::NotAllowed, "cannot convert pointer to i16");
	addConversion(TypeKind::Pointer, TypeKind::i32, ConversionType::NotAllowed, "cannot convert pointer to i32");
	addConversion(TypeKind::Pointer, TypeKind::i64, ConversionType::NotAllowed, "cannot convert pointer to i64");

	addConversion(TypeKind::Pointer, TypeKind::u8, ConversionType::NotAllowed, "cannot convert pointer to u8");
	addConversion(TypeKind::Pointer, TypeKind::u16, ConversionType::NotAllowed, "cannot convert pointer to u16");
	addConversion(TypeKind::Pointer, TypeKind::u32, ConversionType::NotAllowed, "cannot convert pointer to u32");
	addConversion(TypeKind::Pointer, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert pointer to u64");

	addConversion(TypeKind::Pointer, TypeKind::f32, ConversionType::NotAllowed, "cannot convert pointer to f32");
	addConversion(TypeKind::Pointer, TypeKind::f64, ConversionType::NotAllowed, "cannot convert pointer to f64");

	addConversion(TypeKind::Pointer, TypeKind::Char, ConversionType::NotAllowed, "cannot convert pointer to char");
	addConversion(TypeKind::Pointer, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert pointer to bool");
	addConversion(TypeKind::Pointer, TypeKind::Void, ConversionType::NotAllowed, "cannot convert pointer to void");
	addConversion(TypeKind::Pointer, TypeKind::Function, ConversionType::NotAllowed, "cannot convert pointer to function");
	addConversion(TypeKind::Pointer, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert pointer to struct");
	addConversion(TypeKind::Pointer, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert pointer to unknown");
	addConversion(TypeKind::Pointer, TypeKind::Array, ConversionType::NotAllowed, "cannot convert pointer to array");
	addConversion(TypeKind::Pointer, TypeKind::Pointer, ConversionType::SameType, "cannot convert pointer to array");

	// unknown conversions
	addConversion(TypeKind::Unknown, TypeKind::i8, ConversionType::NotAllowed, "cannot convert unknown to i8");
	addConversion(TypeKind::Unknown, TypeKind::i16, ConversionType::NotAllowed, "cannot convert unknown to i16");
	addConversion(TypeKind::Unknown, TypeKind::i32, ConversionType::NotAllowed, "cannot convert unknown to i32");
	addConversion(TypeKind::Unknown, TypeKind::i64, ConversionType::NotAllowed, "cannot convert unknown to i64");

	addConversion(TypeKind::Unknown, TypeKind::u8, ConversionType::NotAllowed, "cannot convert unknown to u8");
	addConversion(TypeKind::Unknown, TypeKind::u16, ConversionType::NotAllowed, "cannot convert unknown to u16");
	addConversion(TypeKind::Unknown, TypeKind::u32, ConversionType::NotAllowed, "cannot convert unknown to u32");
	addConversion(TypeKind::Unknown, TypeKind::u64, ConversionType::NotAllowed, "cannot convert unknown to u64");

	addConversion(TypeKind::Unknown, TypeKind::f32, ConversionType::NotAllowed, "cannot convert unknown to f32");
	addConversion(TypeKind::Unknown, TypeKind::f64, ConversionType::NotAllowed, "cannot convert unknown to f64");

	addConversion(TypeKind::Unknown, TypeKind::Char, ConversionType::NotAllowed, "cannot convert unknown to char");
	addConversion(TypeKind::Unknown, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert unknown to bool");
	addConversion(TypeKind::Unknown, TypeKind::Void, ConversionType::NotAllowed, "cannot convert unknown to void");
	addConversion(TypeKind::Unknown, TypeKind::Function, ConversionType::NotAllowed, "cannot convert unknown to function");
	addConversion(TypeKind::Unknown, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert unknown to struct");
	addConversion(TypeKind::Unknown, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert unknown to unknown");
	addConversion(TypeKind::Unknown, TypeKind::Array, ConversionType::NotAllowed, "cannot convert unknown to array");
	addConversion(TypeKind::Unknown, TypeKind::Pointer, ConversionType::NotAllowed, "cannot convert unknown to pointer");
}

void TypeConversions::addConversion(TypeKind from, TypeKind to, ConversionType type, const char* message)
{
	conversionTable[static_cast<size_t>(from)][static_cast<size_t>(to)] = { type, message };
}

ConversionInfo TypeConversions::getConversion(TypeKind from, TypeKind to)
{
	return conversionTable[static_cast<size_t>(from)][static_cast<size_t>(to)];
}