cmake_minimum_required(VERSION 3.16)

project(03_FRACTAL-DLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")


find_package(fmt CONFIG REQUIRED)


add_library(03_FRACTAL-DLL SHARED
    palette.cpp
    fractal_simd.cpp
)

target_link_libraries(03_FRACTAL-DLL PRIVATE 
fmt::fmt 
)