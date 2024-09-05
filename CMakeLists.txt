cmake_minimum_required(VERSION 3.0.0)

PROJECT(cpp_printer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SOURCE_ROOT "csrc")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_ROOT})
set(exe "main")

set(PRINT_SRCS)

add_subdirectory(${PROJECT_SOURCE_ROOT}/printer)

set(CPP_SRCS ${PRINT_SRCS} ${PROJECT_SOURCE_ROOT}/main.cpp)

ADD_EXECUTABLE(${exe} ${CPP_SRCS})

# add_library

target_link_libraries(${exe} PUBLIC -ldw)