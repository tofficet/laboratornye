cmake_minimum_required(VERSION 3.16)
project(TreeSimulation2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_DIR "/opt/homebrew/Cellar/qt@5/5.15.18/lib/cmake/Qt5")

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

# Добавляем текущую папку в пути поиска заголовочных файлов
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(TreeSimulation2
    main2.cpp
    mainwindow2.cpp
    tree_model2.cpp
)

target_link_libraries(TreeSimulation2 Qt5::Widgets Qt5::Core Qt5::Gui)