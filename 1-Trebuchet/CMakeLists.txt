cmake_minimum_required(VERSION 3.15)
project(1-Trebuchet)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(1-Trebuchet ${SOURCES})
