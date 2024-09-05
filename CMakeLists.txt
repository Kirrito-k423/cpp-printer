set(PROJECT_SOURCE_ROOT "csrc")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_ROOT})
set(exe "./bin/exe")

set(PRINT_SRCS)

add_subdirectory(${PROJECT_SOURCE_ROOT}/printer)

set(CPP_SRCS ${PRINT_SRCS} ${PROJECT_SOURCE_ROOT}/main.cpp)

ADD_EXECUTABLE(${exe} ${CPP_SRCS})

# add_library

target_link_libraries(${exe} PUBLIC -ldw)