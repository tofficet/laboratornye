cmake_minimum_required(VERSION 3.16)
project(FactorySimulation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(FactorySimulation ${SOURCES})

target_link_libraries(FactorySimulation Qt5::Core Qt5::Widgets)