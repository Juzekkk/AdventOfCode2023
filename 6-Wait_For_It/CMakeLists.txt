cmake_minimum_required(VERSION 3.15)
project(6-Wait_For_It)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(6-Wait_For_It ${SOURCES})
