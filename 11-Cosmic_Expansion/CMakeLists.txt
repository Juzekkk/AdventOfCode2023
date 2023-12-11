cmake_minimum_required(VERSION 3.15)
project(11-Cosmic_Expansion)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(11-Cosmic_Expansion ${SOURCES})
