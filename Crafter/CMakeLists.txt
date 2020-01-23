cmake_minimum_required(VERSION 3.16)

add_subdirectory(src)
include(CMakePrintHelpers)
file(COPY ./data DESTINATION "${CMAKE_BINARY_DIR}")
# cmake_print_variables($<TARGET_FILE_DIR:CrafterUI>)
