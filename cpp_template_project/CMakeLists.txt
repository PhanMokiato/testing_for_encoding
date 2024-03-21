cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_CLANG_TIDY clang-tidy; --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy)
set(CMAKE_CXX_STANDARD 20)

project(cpp_template_project)

include(FetchContent)

add_subdirectory(source)

add_subdirectory(tests)