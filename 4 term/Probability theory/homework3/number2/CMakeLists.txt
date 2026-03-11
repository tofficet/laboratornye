cmake_minimum_required(VERSION 3.16)
project(TreeSimulation)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к Qt5
set(Qt5_DIR "/opt/homebrew/Cellar/qt@5/5.15.18/lib/cmake/Qt5")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

include_directories(include)

# Явно обрабатываем только mainwindow.h (если tree_model.h не имеет Q_OBJECT)
qt5_wrap_cpp(MOC_SOURCES include/mainwindow.h)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/tree_model.cpp
)

add_executable(TreeSimulation ${SOURCES} ${MOC_SOURCES})

target_link_libraries(TreeSimulation Qt5::Core Qt5::Widgets)