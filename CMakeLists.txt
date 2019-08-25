# cmake_minimum_required(VERSION <specify CMake version here>)
cmake_minimum_required(VERSION 3.13)
project(fan)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -O3 -pthread" )

add_executable(fan main.cpp fan.cpp fan.h)
find_library(wiringPi_LIB wiringPi)
target_link_libraries(fan ${wiringPi_LIB})