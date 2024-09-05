


set(PROJECT_SOURCE_ROOT "csrc")
set(PRINT_SRCS)

add_subdirectory()

set(CPP_SRCS ${PROJECT_SOURCE_ROOT}/printer)



# add_library

target_link_libraries(${exe} PUBLIC -ldw)