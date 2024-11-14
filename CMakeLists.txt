cmake_minimum_required(VERSION 3.11)
project(cpprinter VERSION 1.0.0)

# SET(CMAKE_VERBOSE_MAKEFILE on)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3")

# Recursively gather all .cpp files under src/
file(GLOB_RECURSE CPPRINTER_SOURCES "src/*.cpp")

# 添加 cpp-printer 库 
# 默认会在 build/ 下生成 cpprinter.a
# 查找静态库 find . -name 'libcpprinter*' -or -name 'cpprinter*'
# 验证符号 nm -C /path/to/packages/torch_npu/lib/libcpprinter.a | grep getProcessInfo
add_library(cpprinter ${CPPRINTER_SOURCES})

target_link_libraries(cpprinter PRIVATE pthread)
target_link_libraries(cpprinter PRIVATE stdc++fs)

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

# 获取传递的变量 CPPRINTER_USE_CXX11_ABI
if(DEFINED CPPRINTER_USE_CXX11_ABI)
    message(STATUS "CPPRINTER_USE_CXX11_ABI is set to ${CPPRINTER_USE_CXX11_ABI}")
else()
    set(CPPRINTER_USE_CXX11_ABI 0)  # 默认值
    message(STATUS "CPPRINTER_USE_CXX11_ABI is not defined, setting it to default (0)")
endif()

# 设置不同目标的编译选项
if(CPPRINTER_USE_CXX11_ABI EQUAL 1)
    target_compile_options(cpprinter PRIVATE -D_GLIBCXX_USE_CXX11_ABI=1)
    target_compile_options(test_printer PRIVATE -D_GLIBCXX_USE_CXX11_ABI=1)
    target_compile_options(test_vargs PRIVATE -D_GLIBCXX_USE_CXX11_ABI=1)
else()
    target_compile_options(cpprinter PRIVATE -D_GLIBCXX_USE_CXX11_ABI=0)
    target_compile_options(test_printer PRIVATE -D_GLIBCXX_USE_CXX11_ABI=0)
    target_compile_options(test_vargs PRIVATE -D_GLIBCXX_USE_CXX11_ABI=0)
endif()

