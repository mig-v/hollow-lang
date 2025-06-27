
# Store higher level info on opcodes -> name, operand count, size?

MAX_OPERANDS = 3

LAST_OPERAND = '}};\n'
REGULAR_OPERAND = ', '

# Each dict value is data for an operand corresponding to the OperandData struct in opcode_data.h
# It's an array of tuples, where each tuple represents an operand
# each tuple holds the size of the operand, and the type of operand
# empty [] indicates no operands for the opcode
opcode_data = {
    "PUSH_I8"  : [(1, "I8")],
    "PUSH_I16" : [(2, "I16")],
    "PUSH_I32" : [(4, "I32")],
    "PUSH_I64" : [(8, "I64")],
    "PUSH_U8"  : [(1, "U8")],
    "PUSH_U16" : [(2, "U16")],
    "PUSH_U32" : [(4, "U32")],
    "PUSH_U64" : [(8, "U64")],
    "PUSH_F32" : [(4, "F32")],
    "PUSH_F64" : [(8, "F64")],
    "PUSH_CHAR" : [(1, "Char")],
    "PUSH_BOOL" : [(1, "Bool")],

    "LOAD_I8"  : [(2, "U16")],
    "LOAD_I16" : [(2, "U16")],
    "LOAD_I32" : [(2, "U16")],
    "LOAD_I64" : [(2, "U16")],
    "LOAD_U8"  : [(2, "U16")],
    "LOAD_U16" : [(2, "U16")],
    "LOAD_U32" : [(2, "U16")],
    "LOAD_U64" : [(2, "U16")],
    "LOAD_F32" : [(2, "U16")],
    "LOAD_F64" : [(2, "U16")],
    "LOAD_CHAR" : [(2, "U16")],
    "LOAD_BOOL" : [(2, "U16")],

    "STORE_I8"  : [(2, "U16")],
    "STORE_I16" : [(2, "U16")],
    "STORE_I32" : [(2, "U16")],
    "STORE_I64" : [(2, "U16")],
    "STORE_U8"  : [(2, "U16")],
    "STORE_U16" : [(2, "U16")],
    "STORE_U32" : [(2, "U16")],
    "STORE_U64" : [(2, "U16")],
    "STORE_F32" : [(2, "U16")],
    "STORE_F64" : [(2, "U16")],
    "STORE_CHAR" : [(2, "U16")],
    "STORE_BOOL" : [(2, "U16")],

    "JMP"          : [(2, "U16")],
    "JMP_IF_FALSE" : [(2, "U16")],
    "JMP_IF_TRUE"  : [(2, "U16")],
    "CALL"         : [(2, "U16"), (1, "U8")],
    "RET"          : [],
    
    "ADD_I8"  : [],
    "ADD_I16" : [],
    "ADD_I32" : [],
    "ADD_I64" : [],
    "ADD_U8"  : [],
    "ADD_U16" : [],
    "ADD_U32" : [],
    "ADD_U64" : [],
    "ADD_F32" : [],
    "ADD_F64" : [],

    "SUB_I8"  : [],
    "SUB_I16" : [],
    "SUB_I32" : [],
    "SUB_I64" : [],
    "SUB_U8"  : [],
    "SUB_U16" : [],
    "SUB_U32" : [],
    "SUB_U64" : [],
    "SUB_F32" : [],
    "SUB_F64" : [],

    "MUL_I8"  : [],
    "MUL_I16" : [],
    "MUL_I32" : [],
    "MUL_I64" : [],
    "MUL_U8"  : [],
    "MUL_U16" : [],
    "MUL_U32" : [],
    "MUL_U64" : [],
    "MUL_F32" : [],
    "MUL_F64" : [],

    "DIV_I8"  : [],
    "DIV_I16" : [],
    "DIV_I32" : [],
    "DIV_I64" : [],
    "DIV_U8"  : [],
    "DIV_U16" : [],
    "DIV_U32" : [],
    "DIV_U64" : [],
    "DIV_F32" : [],
    "DIV_F64" : [],

    "MOD_I8"  : [],
    "MOD_I16" : [],
    "MOD_I32" : [],
    "MOD_I64" : [],
    "MOD_U8"  : [],
    "MOD_U16" : [],
    "MOD_U32" : [],
    "MOD_U64" : [],

    "NEG_I8"  : [],
    "NEG_I16" : [],
    "NEG_I32" : [],
    "NEG_I64" : [],
    "NEG_F32" : [],
    "NEG_F64" : [],

    "EQ_I8"  : [],
    "EQ_I16" : [],
    "EQ_I32" : [],
    "EQ_I64" : [],
    "EQ_U8"  : [],
    "EQ_U16" : [],
    "EQ_U32" : [],
    "EQ_U64" : [],
    "EQ_F32" : [],
    "EQ_F64" : [],
    "EQ_CHAR" : [],
    "EQ_BOOL" : [],

    "NEQ_I8"  : [],
    "NEQ_I16" : [],
    "NEQ_I32" : [],
    "NEQ_I64" : [],
    "NEQ_U8"  : [],
    "NEQ_U16" : [],
    "NEQ_U32" : [],
    "NEQ_U64" : [],
    "NEQ_F32" : [],
    "NEQ_F64" : [],
    "NEQ_CHAR" : [],
    "NEQ_BOOL" : [],

    "GT_I8"  : [],
    "GT_I16" : [],
    "GT_I32" : [],
    "GT_I64" : [],
    "GT_U8"  : [],
    "GT_U16" : [],
    "GT_U32" : [],
    "GT_U64" : [],
    "GT_F32" : [],
    "GT_F64" : [],
    "GT_CHAR" : [],

    "GTE_I8"  : [],
    "GTE_I16" : [],
    "GTE_I32" : [],
    "GTE_I64" : [],
    "GTE_U8"  : [],
    "GTE_U16" : [],
    "GTE_U32" : [],
    "GTE_U64" : [],
    "GTE_F32" : [],
    "GTE_F64" : [],
    "GTE_CHAR" : [],

    "LT_I8"  : [],
    "LT_I16" : [],
    "LT_I32" : [],
    "LT_I64" : [],
    "LT_U8"  : [],
    "LT_U16" : [],
    "LT_U32" : [],
    "LT_U64" : [],
    "LT_F32" : [],
    "LT_F64" : [],
    "LT_CHAR" : [],

    "LTE_I8"  : [],
    "LTE_I16" : [],
    "LTE_I32" : [],
    "LTE_I64" : [],
    "LTE_U8"  : [],
    "LTE_U16" : [],
    "LTE_U32" : [],
    "LTE_U64" : [],
    "LTE_F32" : [],
    "LTE_F64" : [],
    "LTE_CHAR" : [],
}

def print_empty_operands(count):
    for i in range(count):
        if i < count - 1:
            print("OperandData{" + str(0) + ", OperandType::None}", end = REGULAR_OPERAND)
        else:
            print("OperandData{" + str(0) + ", OperandType::None}", end = LAST_OPERAND)

def main():
    for key, value in opcode_data.items():
        print(f"opcodeData[static_cast<size_t>(Opcode::{key})] = {{", end = '')
        print(str(len(value)) + ", { ", end = '')

        operandCount = len(value)
        for i in range(operandCount):
            if i < MAX_OPERANDS - 1:
                print(f"OperandData{{{value[i][0]}, OperandType::{value[i][1]}}}", end = REGULAR_OPERAND)
            else:
                print(f"OperandData{{{value[i][0]}, OperandType::{value[i][1]}}}", end = LAST_OPERAND)

            #print("OperandData{", str(operand[0]), ", OperandType::", operand[1], end = '}, ')

        if operandCount < MAX_OPERANDS:
            print_empty_operands(MAX_OPERANDS - operandCount)

if __name__ == "__main__":
    main()