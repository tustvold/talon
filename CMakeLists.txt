# CMake Setup

cmake_minimum_required(VERSION 3.8)
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")

# Project Configuration

project(talon)

find_package(GLFW3 REQUIRED)
find_package(FRUIT REQUIRED)
find_package(TBB REQUIRED)
find_package(Vulkan REQUIRED)

add_subdirectory(src)
add_subdirectory(example)


