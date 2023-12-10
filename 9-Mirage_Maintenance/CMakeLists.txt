cmake_minimum_required(VERSION 3.15)
project(9-Mirage_Maintenance)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(9-Mirage_Maintenance ${SOURCES})
