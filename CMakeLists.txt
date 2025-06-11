cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hollow-lang)

add_executable(hollow-lang src/main.cpp
                           src/lexer.cpp
                           src/transition_table.cpp
                           src/lexer_tests.cpp
                           src/ast_node.cpp
                           src/parser.cpp
                           src/compiler.cpp
                           src/error_report.cpp
                           src/ast_printer.cpp
                           src/token_stream.cpp
                           include/lexer.h
                           include/token.h
                           include/transition_table.h
                           include/lexer_state.h
                           include/lexer_tests.h
                           include/ast_node.h
                           include/parser.h
                           include/compiler.h
                           include/error_report.h
                           include/ast_printer.h
                           include/token_stream.h)

target_include_directories(hollow-lang PUBLIC include/)
target_compile_definitions(hollow-lang PUBLIC TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests")
