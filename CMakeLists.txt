cmake_minimum_required(VERSION 3.11)
project(cpprinter VERSION 1.0.0)

# SET(CMAKE_VERBOSE_MAKEFILE on)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")

# Recursively gather all .cpp files under src/
file(GLOB_RECURSE CPPRINTER_SOURCES "src/*.cpp")

# 添加 cpp-printer 库
add_library(cpprinter ${CPPRINTER_SOURCES})

target_link_libraries(cpprinter PRIVATE pthread)

# 包含头文件路径
target_include_directories(cpprinter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 将私有头文件仅限内部使用
target_include_directories(cpprinter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 测试代码
enable_testing()
add_subdirectory(tests)

