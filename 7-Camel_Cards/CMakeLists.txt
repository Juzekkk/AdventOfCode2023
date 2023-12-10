cmake_minimum_required(VERSION 3.15)
project(7-Camel_Cards)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(7-Camel_Cards ${SOURCES})
