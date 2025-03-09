cmake_minimum_required (VERSION 3.28)
project(spicy VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE CXX_SOURCES  "${SRC_DIR}/*.cpp")
list(FILTER CXX_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(spicy_lib STATIC "${CXX_SOURCES}")
target_include_directories(spicy_lib PUBLIC "${SRC_DIR}")
target_compile_options(spicy_lib PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Wconversion>
)

add_executable(spicy "${SRC_DIR}/main.cpp")
target_link_libraries(spicy PRIVATE spicy_lib)

add_subdirectory(tests)
