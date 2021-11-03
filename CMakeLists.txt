cmake_minimum_required(VERSION 3.0)

project(IMMGRAPHICS)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(include)

file(GLOB_RECURSE src_files "src/*.cpp")

add_executable(gmath_test test/gmath_test.cpp ${src_files})
add_executable(window_test test/window_test.cpp ${src_files})
