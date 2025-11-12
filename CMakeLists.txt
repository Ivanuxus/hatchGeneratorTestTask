cmake_minimum_required(VERSION 3.10)
project(HatchProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hatch hatch.cpp)

target_compile_options(hatch PRIVATE -Wall -Wextra)