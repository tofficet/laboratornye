cmake_minimum_required(VERSION 3.16)
project(ClusterAnalyzer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)

set(Qt5_DIR "/opt/homebrew/Cellar/qt@5/5.15.18/lib/cmake/Qt5")

find_package(Qt5 REQUIRED Core Widgets Gui)

add_executable(ClusterAnalyzer
    main.cpp
    mainwindow.cpp
    cluster_analyzer.cpp
)

target_link_libraries(ClusterAnalyzer Qt5::Core Qt5::Widgets Qt5::Gui)