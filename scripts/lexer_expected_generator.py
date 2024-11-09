import sys

tokenMap = {
    "i8" : "TokenType::i8Keyword",
    "i16" : "TokenType::i16Keyword",
    "i32" : "TokenType::i32Keyword",
    "i64" : "TokenType::i64Keyword",
    "u8" : "TokenType::u8Keyword",
    "u16" : "TokenType::u16Keyword",
    "u32" : "TokenType::u32Keyword",
    "u64" : "TokenType::u64Keyword",
    "f32" : "TokenType::f32Keyword",
    "f64" : "TokenType::f64Keyword",
    "return" : "TokenType::returnKeyword",
    "for" : "TokenType::forKeyword",
    "if" : "TokenType::ifKeyword",
    "while" : "TokenType::whileKeyword",
    "type" : "TokenType::typeKeyword",
    "func" : "TokenType::funcKeyword",
    "break" : "TokenType::breakKeyword",
    ";" : "TokenType::Semicolon",
    "intlit" : "TokenType::IntLiteral",
    "floatlit" : "TokenType::FloatLiteral",
    "=" : "TokenType::Assign",
    ">" : "TokenType::GreaterThan",
    ">=" : "TokenType::GreaterThanEq",
    "<" : "TokenType::LessThan",
    "<=" : "TokenType::LessThanEq",
    "!" : "TokenType::LogicalNot",
    "!=" : "TokenType::NotEq",
    "(" : "TokenType::OpenParen",
    ")" : "TokenType::CloseParen",
    "{" : "TokenType::OpenCurly",
    "}" : "TokenType::CloseCurly",
    ":" : "TokenType::Colon",
    "==" : "TokenType::Equality"
    }

def parseFile(filename):
    file = open(filename).read().splitlines()
    tokensPerLine = 4
    count = 0

    print("{ ", end = '')
    for line in file:
        if count == tokensPerLine:
            print()
            count = 0

        if line in tokenMap.keys():
            print(tokenMap[line], end = ', ')
        else:
            print("TokenType::Identifier", end = ', ')
            
        count += 1

    print ("TokenType::Eof }")    

def main():
    argc = len(sys.argv)

    if argc != 2:
        print("Usage: script.py expected.txt")
        exit()

    parseFile(sys.argv[1])


if __name__ == "__main__":
    main()