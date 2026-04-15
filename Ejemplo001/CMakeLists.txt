cmake_minimum_required(VERSION 3.16)

project(Ejemplo001)

find_package(fmt CONFIG REQUIRED)

add_executable(ejemplo001 main.cpp)

target_link_libraries(ejemplo001 PRIVATE fmt::fmt)