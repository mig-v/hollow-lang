cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hollow-lang)

set(HOLLOW_CORE_FILES
    src/core/compiler.cpp
    src/core/diagnostic.cpp
    src/core/compiler.h
    src/core/diagnostic.h
    src/core/l_value.h
    src/core/arena_block.h
    src/core/memory_arena.h
    src/core/type_info.h
    src/core/ast_visitor.h)

set(HOLLOW_LEXER_FILES
    src/lexer/lexer.cpp
    src/lexer/transition_table.cpp
    src/lexer/lexer.h
    src/lexer/token.h
    src/lexer/transition_table.h
    src/lexer/lexer_state.h)

set(HOLLOW_PARSER_FILES
    src/parser/ast_node.cpp
    src/parser/parser.cpp
    src/parser/token_stream.cpp
    src/parser/ast_expr.cpp
    src/parser/ast_stmt.cpp
    src/parser/ast_type.cpp
    src/parser/ast_node.h
    src/parser/ast_type.h
    src/parser/parser.h
    src/parser/token_stream.h
    src/parser/ast_expr.h
    src/parser/ast_stmt.h)

set(HOLLOW_SEMANTICS_FILES
    src/semantics/semantic_analysis.cpp
    src/semantics/symbol_table.cpp
    src/semantics/environment.cpp
    src/semantics/type_checker.cpp
    src/semantics/type_conversions.cpp
    src/semantics/symbol_resolution.cpp
    src/semantics/constant_evaluator.cpp
    src/semantics/type_table.cpp
    src/semantics/semantic_analysis.h
    src/semantics/symbol_table.h
    src/semantics/environment.h
    src/semantics/type_checker.h
    src/semantics/type_conversions.h
    src/semantics/symbol_resolution.h
    src/semantics/constant_evaluator.h
    src/semantics/type_table.h)

set(HOLLOW_IR_FILES
    src/ir/ir_generator.cpp
    src/ir/ir_environment.cpp
    src/ir/ir_lowering.cpp
    src/ir/ir_logger.cpp
    src/ir/ir_generator.h
    src/ir/ir_environment.h
    src/ir/ir_core.h
    src/ir/ir_lowering.h
    src/ir/ir_logger.h)

set(HOLLOW_OPTIMIZATION_FILES
    src/optimization/constant_folder.cpp
    src/optimization/dead_code_elimination.cpp
    src/optimization/constant_propagation.cpp
    src/optimization/optimizer.cpp
    src/optimization/constant_folder.h
    src/optimization/dead_code_elimination.h
    src/optimization/constant_propagation.h
    src/optimization/optimization_pass.h
    src/optimization/optimizer.h)

set(HOLLOW_CODEGEN_FILES
    src/codegen/code_emitter.h
    src/codegen/target_info.h)

set(HOLLOW_UTILS_FILES
    src/utils/ast_printer.cpp
    src/utils/debug_utils.cpp
    src/utils/semantic_utils.cpp
    src/utils/ast_utils.cpp
    src/utils/type_utils.cpp
    src/utils/ast_printer.h
    src/utils/debug_utils.h
    src/utils/semantic_utils.h
    src/utils/ast_utils.h
    src/utils/type_utils.h)                 

set(HOLLOW_TESTING_FILES
    src/testing/lexer_tests.cpp
    src/testing/parser_tests.cpp
    src/testing/lexer_tests.h
    src/testing/parser_tests.h)

set(HOLLOW_ARCHIVE_FILES
    src/archive/bytecode_emitter.cpp
    src/archive/bytecode_disassembler.cpp
    src/archive/opcode_data.cpp
    src/archive/vm_function_table.cpp
    src/archive/vm.cpp
    src/archive/bytecode_emitter_tests.cpp
    src/archive/x86_64_emitter.cpp
    src/archive/register_allocation.cpp
    src/archive/register_pool.cpp
    src/archive/constant_pool.cpp
    src/archive/bytecode_emitter.h
    src/archive/opcodes.h
    src/archive/bytecode_disassembler.h
    src/archive/opcode_data.h
    src/archive/vm_function_table.h
    src/archive/vm.h
    src/archive/bytecode_emitter_tests.h
    src/archive/x86_64_emitter.h
    src/archive/register_allocation.h
    src/archive/register_allocation_core.h
    src/archive/register_pool.h
    src/archive/register.h
    src/archive/constant_pool.h)

add_executable(hollow-lang 
               src/main.cpp
               ${HOLLOW_CORE_FILES}
               ${HOLLOW_LEXER_FILES}
               ${HOLLOW_PARSER_FILES}
               ${HOLLOW_SEMANTICS_FILES}
               ${HOLLOW_IR_FILES}
               ${HOLLOW_OPTIMIZATION_FILES}
               ${HOLLOW_CODEGEN_FILES}
               ${HOLLOW_UTILS_FILES}
               ${HOLLOW_TESTING_FILES}
               ${HOLLOW_ARCHIVE_FILES})

source_group(CORE FILES ${HOLLOW_CORE_FILES})
source_group(LEXER FILES ${HOLLOW_LEXER_FILES})
source_group(PARSER FILES ${HOLLOW_PARSER_FILES})
source_group(SEMANTICS FILES ${HOLLOW_SEMANTICS_FILES})
source_group(IR FILES ${HOLLOW_IR_FILES})
source_group(OPTIMIZATION FILES ${HOLLOW_OPTIMIZATION_FILES})
source_group(CODEGEN FILES ${HOLLOW_CODEGEN_FILES})
source_group(UTILS FILES ${HOLLOW_UTILS_FILES})
source_group(TESTING FILES ${HOLLOW_TESTING_FILES})
source_group(ARCHIVE FILES ${HOLLOW_ARCHIVE_FILES})

target_include_directories(hollow-lang PUBLIC src/)
target_compile_definitions(hollow-lang PUBLIC TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests")