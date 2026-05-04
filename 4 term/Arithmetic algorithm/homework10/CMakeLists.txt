cmake_minimum_required(VERSION 3.10)
project(ComputeEandPi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_policy(SET CMP0167 NEW)

find_package(Boost REQUIRED)

add_executable(compute main.cpp)
target_link_libraries(compute Boost::boost)