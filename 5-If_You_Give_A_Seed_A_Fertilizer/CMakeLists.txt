cmake_minimum_required(VERSION 3.15)
project(5-If_You_Give_A_Seed_A_Fertilizer)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(5-If_You_Give_A_Seed_A_Fertilizer ${SOURCES})
