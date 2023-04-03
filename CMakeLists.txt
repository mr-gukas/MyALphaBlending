cmake_minimum_required(VERSION 3.16)
project(alpha)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

add_compile_options(-O2)

add_executable(alpha main.cpp alpha.cpp)
target_link_libraries(alpha sfml-graphics sfml-window sfml-system sfml-audio)

