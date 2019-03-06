cmake_minimum_required(VERSION 3.1)

project(expression-fetcher-test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4 /WX" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

include_directories(lib/)
set(SOURCE lib/expression_fetcher.h 
    src/unittests.cpp 
    src/test_dump_types.cpp 
    src/test_dump_expressions.cpp 
    src/test_dump_corner_case.cpp 
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})