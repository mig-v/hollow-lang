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
                           src/diagnostic.cpp
                           src/ast_printer.cpp
                           src/token_stream.cpp
                           src/parser_tests.cpp
                           src/semantic_analysis.cpp
                           src/symbol_table.cpp
                           src/environment.cpp
                           src/ast_expr.cpp
                           src/ast_stmt.cpp
                           src/debug_utils.cpp
                           src/semantic_utils.cpp
                           src/bytecode_emitter.cpp
                           src/bytecode_disassembler.cpp
                           src/opcode_data.cpp
                           src/type_checker.cpp
                           src/type_conversions.cpp
                           src/vm_function_table.cpp
                           src/vm.cpp
                           src/symbol_resolution.cpp
                           src/ast_utils.cpp
                           src/bytecode_emitter_tests.cpp
                           src/ast_type.cpp
                           src/constant_folder.cpp
                           src/constant_evaluator.cpp
                           include/lexer.h
                           include/token.h
                           include/transition_table.h
                           include/lexer_state.h
                           include/lexer_tests.h
                           include/ast_node.h
                           include/parser.h
                           include/compiler.h
                           include/diagnostic.h
                           include/ast_printer.h
                           include/token_stream.h
                           include/parser_tests.h
                           include/memory_arena.h
                           include/arena_block.h
                           include/type_info.h
                           include/semantic_analysis.h
                           include/symbol_table.h
                           include/environment.h
                           include/ast_expr.h
                           include/ast_stmt.h
                           include/debug_utils.h
                           include/semantic_utils.h
                           include/bytecode_emitter.h
                           include/opcodes.h
                           include/bytecode_disassembler.h
                           include/opcode_data.h
                           include/type_checker.h
                           include/type_conversions.h
                           include/l_value.h
                           include/vm_function_table.h
                           include/vm.h
                           include/symbol_resolution.h
                           include/ast_visitor.h
                           include/ast_utils.h
                           include/bytecode_emitter_tests.h
                           include/ast_type.h
                           include/constant_folder.h
                           include/constant_evaluator.h)

target_include_directories(hollow-lang PUBLIC include/)
target_compile_definitions(hollow-lang PUBLIC TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests")
