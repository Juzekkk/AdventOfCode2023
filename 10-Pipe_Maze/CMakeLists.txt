cmake_minimum_required(VERSION 3.15)
project(10-Pipe_Maze)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(10-Pipe_Maze ${SOURCES})
