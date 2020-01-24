cmake_minimum_required(VERSION 3.5)

project(Crafter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Crafter
    import.cpp
    graph-construct.cpp
    graph-builder.cpp
)

add_executable(GraphConstruct graph-construct.cpp)
target_link_libraries(Crafter "yaml-cpp")

target_link_libraries(GraphConstruct Crafter)
target_include_directories(Crafter PUBLIC ./)
target_include_directories(GraphConstruct PUBLIC ./)
include_directories("/")
target_compile_options(Crafter PRIVATE -Wall -Wextra)
target_compile_options(GraphConstruct PRIVATE -Wall -Wextra)
