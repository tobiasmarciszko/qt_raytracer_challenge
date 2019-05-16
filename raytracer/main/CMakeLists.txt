cmake_minimum_required(VERSION 3.14)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

include_directories(..)
include_directories(.)
include_directories(../tests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# find_package(Qt5Quick REQUIRED)

add_executable(raytracer
        canvas.h
        color.cpp
        color.h
        effect.h
        glwidget.cpp
        glwidget.h
        helper.cpp
        helper.h
        intersection.cpp
        intersection.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        matrix.h
        object.cpp
        object.h
        point.cpp
        point.h
        ray.cpp
        ray.h
        raytracer.cpp
        raytracer.h
        sphere.cpp
        sphere.h
        tuple.cpp
        tuple.h
        vector.cpp
        vector.h
        )

target_link_libraries(raytracer Qt5::Core)
target_link_libraries(raytracer Qt5::Widgets)
