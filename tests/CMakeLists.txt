
# 创建一个用于测试的可执行文件
add_executable(test_printer multithread.cpp)

# 为 test_printer 可执行文件设置头文件路径
target_include_directories(test_printer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接 cpp_printer 库
target_link_libraries(test_printer PRIVATE cpprinter)

# 添加测试
add_test(NAME cpp_printer_test COMMAND test_printer)
