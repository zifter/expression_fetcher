cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(expression-fetcher-test)

include_directories(lib/)
set(SOURCE lib/expression_fetcher.h src/tests.cpp src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})