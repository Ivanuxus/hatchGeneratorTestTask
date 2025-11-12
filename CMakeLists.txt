cmake_minimum_required(VERSION 3.10)
project(HatchProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6.1 COMPONENTS graphics REQUIRED)

add_executable(hatch_generator hatch.cpp)

target_link_libraries(hatch_generator PRIVATE sfml-graphics)