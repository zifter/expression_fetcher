cmake_minimum_required(VERSION 3.0)

project(expression-fetcher-test)

include_directories(lib/)
set(SOURCE lib/expression_fetcher.h src/tests.cpp src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})