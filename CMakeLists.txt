cmake_minimum_required(VERSION 3.20)
project(wind-sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(wind-sim src/main.cpp)
target_link_libraries(wind-sim PRIVATE raylib)
