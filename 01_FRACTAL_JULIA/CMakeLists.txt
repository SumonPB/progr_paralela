cmake_minimum_required(VERSION 3.16)

project(01_FRACTAL_JULIA)

find_package(fmt CONFIG REQUIRED)
find_package(SFML COMPONENTS 
                    System 
                    Graphics 
                    Window
                    CONFIG REQUIRED)

add_executable(01_FRACTAL_JULIA 
main.cpp
fractal_serial.cpp
palette.cpp
fractal_simd.cpp
)

target_link_libraries(01_FRACTAL_JULIA PRIVATE 
fmt::fmt 
SFML::System
SFML::Graphics
SFML::Window
)