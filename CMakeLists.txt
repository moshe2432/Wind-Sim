cmake_minimum_required(VERSION 3.20)
project(wind-sim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(raylib REQUIRED)

add_executable(wind-sim src/main.cpp src/input/input.cpp src/rendering/rendering.cpp src/physics/physics.cpp)
target_link_libraries(wind-sim PRIVATE raylib)
