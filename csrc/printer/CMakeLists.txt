FILE(GLOB _PRINT_SRCS *.cpp)

LIST(APPEND PRINT_SRCS ${_PRINT_SRCS})

# pass to parent(the CMakeLists.txt using add_subdirectory)
SET(PRINT_SRCS ${PRINT_SRCS} PARENT_SCOPE)