cmake_minimum_required(VERSION 3.13)
project("Catch Tests")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

include_directories(../main)

add_executable(

        # target
        catchtests

        # test header
        catch.hpp

        # needed from main project
        ../main/color.cpp
        ../main/engine.cpp
        ../main/sphere.cpp
        ../main/appsettings.cpp
        ../main/cube.cpp
        ../main/cylinder.cpp

        # test files
        main.cpp
        chapter1.cpp
        chapter2.cpp
        chapter3.cpp
        chapter4.cpp
        chapter5.cpp
        chapter6.cpp
        chapter7.cpp
        chapter8.cpp
        chapter9.cpp
        chapter10.cpp
        chapter11.cpp
        chapter12.cpp
        chapter13.cpp
        arealights.cpp)

target_link_libraries(catchtests Qt5::Core)
