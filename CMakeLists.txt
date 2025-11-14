cmake_minimum_required(VERSION 3.10)
project(HatchProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0.2 COMPONENTS System Window Graphics REQUIRED)

add_executable(hatch_generator hatch.cpp)

target_link_libraries(hatch_generator SFML::System SFML::Window SFML::Graphics)

target_compile_definitions(hatch_generator PRIVATE SFML_STATIC)