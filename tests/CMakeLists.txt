find_package(Threads REQUIRED)

# 创建一个用于测试的可执行文件
add_executable(test_printer multithread.cpp)
add_executable(test_destructor destructor.cpp)
add_executable(test_vargs vargs.cpp)

set(EXECUTABLES_APP test_printer test_destructor)

# 设置公共的包含目录和库
set(COMMON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(COMMON_LIBS cpprinter Threads::Threads)

# 批量设置 app1 和 app2 的 include 和 link
foreach(target IN LISTS EXECUTABLES_APP)
    target_include_directories(${target} PRIVATE ${COMMON_INCLUDE_DIRS})
    target_link_libraries(${target} PRIVATE ${COMMON_LIBS})
endforeach()

# 添加测试
add_test(NAME cpp_printer_test COMMAND test_printer)
