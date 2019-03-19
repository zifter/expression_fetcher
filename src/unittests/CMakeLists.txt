cmake_minimum_required(VERSION 3.1)

project(expression-fetcher-unittests)

include_directories(.)
set(SOURCES
    unittests.cpp 
    main.cpp
    tests/test_dump_types.cpp 
    tests/test_dump_expressions.cpp 
    tests/test_dump_corner_case.cpp 
    tests/test_eval_condition.cpp 
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_test(NAME UnitTests COMMAND ${PROJECT_NAME})
