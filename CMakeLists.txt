cmake_minimum_required(VERSION 3.0.0)

PROJECT(cpp_printer)

SET( CMAKE_VERBOSE_MAKEFILE on )


# for inline
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")


set(PROJECT_SOURCE_ROOT "csrc")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_ROOT})
set(exe "main")

set(PRINT_SRCS)

add_subdirectory(${PROJECT_SOURCE_ROOT}/printer)

set(CPP_SRCS ${PRINT_SRCS} ${PROJECT_SOURCE_ROOT}/main.cpp)

ADD_EXECUTABLE(${exe} ${CPP_SRCS})

include(FetchContent)
FetchContent_Declare(
  cpptrace
  GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
  GIT_TAG        v0.7.0 # <HASH or TAG>
)
FetchContent_MakeAvailable(cpptrace)
target_link_libraries(${exe} PUBLIC cpptrace::cpptrace)

# add_library for backward-cpp
target_link_libraries(${exe} PUBLIC -ldw)