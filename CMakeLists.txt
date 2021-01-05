cmake_minimum_required(VERSION 3.17)
project(VoxelNetwork)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if (UNIX)
    set(CMAKE_CXX_FLAGS -pthread)
endif ()

add_executable(VoxelNetwork main.cpp)