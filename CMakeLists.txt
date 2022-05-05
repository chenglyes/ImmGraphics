cmake_minimum_required(VERSION 3.5)

project(IMMGRAPHICS)

# set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -D_DEBUG")
# set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

include_directories(include)
file(GLOB_RECURSE src_files "src/*.cpp")

add_executable(gmath_test test/gmath_test.cpp ${src_files})
add_executable(window_test test/window_test.cpp ${src_files})
add_executable(shader_test test/shader_test.cpp ${src_files})
add_executable(triangle test/triangle.cpp ${src_files})
