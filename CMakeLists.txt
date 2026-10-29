cmake_minimum_required(VERSION 3.20)
project(progra1_lab7 CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets Core REQUIRED)

set(SOURCES
        main.cpp
        Lab7.cpp
        Lab7.h
        ArchivoContactos.cpp
        ArchivoContactos.h
)

add_executable(progra1_lab7 ${SOURCES})

target_link_libraries(progra1_lab7 PRIVATE Qt6::Widgets Qt6::Core)