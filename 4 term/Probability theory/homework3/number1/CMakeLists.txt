cmake_minimum_required(VERSION 3.16)
project(ClusterAnalyzer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(Qt5_DIR "/opt/homebrew/Cellar/qt@5/5.15.18/lib/cmake/Qt5")

find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/cluster_analyzer.cpp
)

add_executable(ClusterAnalyzer ${SOURCES})

qt5_wrap_cpp(MOC_SOURCES include/mainwindow.h)
target_sources(ClusterAnalyzer PRIVATE ${MOC_SOURCES})

target_link_libraries(ClusterAnalyzer Qt5::Widgets Qt5::Core Qt5::Gui)