#include "type_conversions.h"

TypeConversions::TypeConversions()
{
	// types -> i8, i16, i32, i64, u8, u16, u32, u64, f32, f64, char, bool, void, function, struct, unknown

	// i8 conversions
	addConversion(TypeKind::i8, TypeKind::i8, ConversionType::SameType);
	addConversion(TypeKind::i8, TypeKind::i16, ConversionType::Widening);
	addConversion(TypeKind::i8, TypeKind::i32, ConversionType::Widening);
	addConversion(TypeKind::i8, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i8, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to u8");
	addConversion(TypeKind::i8, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to u16");
	addConversion(TypeKind::i8, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to u32");
	addConversion(TypeKind::i8, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to u64");

	addConversion(TypeKind::i8, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to f32");
	addConversion(TypeKind::i8, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i8 to f64");

	addConversion(TypeKind::i8, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i8 to char");
	addConversion(TypeKind::i8, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i8 to bool");
	addConversion(TypeKind::i8, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i8 to void");
	addConversion(TypeKind::i8, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i8 to function");
	addConversion(TypeKind::i8, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i8 to struct");
	addConversion(TypeKind::i8, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i8 to unknown");

	// i16 conversions
	addConversion(TypeKind::i16, TypeKind::i8, ConversionType::Narrowing, "narrowing i16 to i8");
	addConversion(TypeKind::i16, TypeKind::i16, ConversionType::SameType);
	addConversion(TypeKind::i16, TypeKind::i32, ConversionType::Widening);
	addConversion(TypeKind::i16, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i16, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to u8");
	addConversion(TypeKind::i16, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to u16");
	addConversion(TypeKind::i16, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to u32");
	addConversion(TypeKind::i16, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to u64");

	addConversion(TypeKind::i16, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to f32");
	addConversion(TypeKind::i16, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i16 to f64");

	addConversion(TypeKind::i16, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i16 to char");
	addConversion(TypeKind::i16, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i16 to bool");
	addConversion(TypeKind::i16, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i16 to void");
	addConversion(TypeKind::i16, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i16 to function");
	addConversion(TypeKind::i16, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i16 to struct");
	addConversion(TypeKind::i16, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i16 to unknown");

	// i32 conversions
	addConversion(TypeKind::i32, TypeKind::i8, ConversionType::Narrowing, "narrowing i32 to i8");
	addConversion(TypeKind::i32, TypeKind::i16, ConversionType::Narrowing, "narrowing i32 to i16");
	addConversion(TypeKind::i32, TypeKind::i32, ConversionType::SameType);
	addConversion(TypeKind::i32, TypeKind::i64, ConversionType::Widening);

	addConversion(TypeKind::i32, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to u8");
	addConversion(TypeKind::i32, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to u16");
	addConversion(TypeKind::i32, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to u32");
	addConversion(TypeKind::i32, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to u64");

	addConversion(TypeKind::i32, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to f32");
	addConversion(TypeKind::i32, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i32 to f64");

	addConversion(TypeKind::i32, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i32 to char");
	addConversion(TypeKind::i32, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i32 to bool");
	addConversion(TypeKind::i32, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i32 to void");
	addConversion(TypeKind::i32, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i32 to function");
	addConversion(TypeKind::i32, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i32 to struct");
	addConversion(TypeKind::i32, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i32 to unknown");


	// i64 conversions
	addConversion(TypeKind::i64, TypeKind::i8, ConversionType::Narrowing, "narrowing i64 to i8");
	addConversion(TypeKind::i64, TypeKind::i16, ConversionType::Narrowing, "narrowing i64 to i16");
	addConversion(TypeKind::i64, TypeKind::i32, ConversionType::Narrowing, "narrowing i64 to i32");
	addConversion(TypeKind::i64, TypeKind::i64, ConversionType::SameType);

	addConversion(TypeKind::i64, TypeKind::u8, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to u8");
	addConversion(TypeKind::i64, TypeKind::u16, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to u16");
	addConversion(TypeKind::i64, TypeKind::u32, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to u32");
	addConversion(TypeKind::i64, TypeKind::u64, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to u64");

	addConversion(TypeKind::i64, TypeKind::f32, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to f32");
	addConversion(TypeKind::i64, TypeKind::f64, ConversionType::ExplicitOnly, "explicit cast needed to convert i64 to f64");

	addConversion(TypeKind::i64, TypeKind::Char, ConversionType::NotAllowed, "cannot convert i64 to char");
	addConversion(TypeKind::i64, TypeKind::Bool, ConversionType::NotAllowed, "cannot convert i64 to bool");
	addConversion(TypeKind::i64, TypeKind::Void, ConversionType::NotAllowed, "cannot convert i64 to void");
	addConversion(TypeKind::i64, TypeKind::Function, ConversionType::NotAllowed, "cannot convert i64 to function");
	addConversion(TypeKind::i64, TypeKind::Struct, ConversionType::NotAllowed, "cannot convert i64 to struct");
	addConversion(TypeKind::i64, TypeKind::Unknown, ConversionType::NotAllowed, "cannot convert i64 to unknown");

	// u8 conversions
	addConversion(TypeKind::u8, TypeKind::i8, ConversionType::ExplicitOnly);
	addConversion(TypeKind::u8, TypeKind::i16, ConversionType::ExplicitOnly);
	addConversion(TypeKind::u8, TypeKind::i32, ConversionType::ExplicitOnly);
	addConversion(TypeKind::u8, TypeKind::i64, ConversionType::ExplicitOnly);

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

	// u16 conversions
	addConversion(TypeKind::u16, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to i8");
	addConversion(TypeKind::u16, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to i16");
	addConversion(TypeKind::u16, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to i32");
	addConversion(TypeKind::u16, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert u16 to i64");

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

	// u32 conversions
	addConversion(TypeKind::u32, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to i8");
	addConversion(TypeKind::u32, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to i16");
	addConversion(TypeKind::u32, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to i32");
	addConversion(TypeKind::u32, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert u32 to i64");

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

	// u64 conversions
	addConversion(TypeKind::u64, TypeKind::i8, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to i8");
	addConversion(TypeKind::u64, TypeKind::i16, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to i16");
	addConversion(TypeKind::u64, TypeKind::i32, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to i32");
	addConversion(TypeKind::u64, TypeKind::i64, ConversionType::ExplicitOnly, "explicit cast needed to convert u64 to i64");

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
}

void TypeConversions::addConversion(TypeKind from, TypeKind to, ConversionType type, const char* message)
{
	conversionTable[static_cast<size_t>(from)][static_cast<size_t>(to)] = { type, message };
}

ConversionInfo TypeConversions::getConversion(TypeKind from, TypeKind to)
{
	return conversionTable[static_cast<size_t>(from)][static_cast<size_t>(to)];
}