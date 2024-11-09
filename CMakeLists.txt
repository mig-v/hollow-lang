cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(hollow-lang)

add_executable(hollow-lang src/main.cpp
                           src/lexer.cpp
                           src/transition_table.cpp
                           src/lexer_tests.cpp
                           include/lexer.h
                           include/token.h
                           include/transition_table.h
                           include/lexer_state.h
                           include/lexer_tests.h)

target_include_directories(hollow-lang PUBLIC include/)
target_compile_definitions(hollow-lang PUBLIC LEXER_TESTS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests/lexer_tests")
