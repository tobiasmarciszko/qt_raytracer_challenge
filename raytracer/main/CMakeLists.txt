cmake_minimum_required(VERSION 3.14)
project(raytracer)

set(CMAKE_CXX_STANDARD 17)

# Sanitizers
#message(WARNING "Sanitizer(s) enabled!")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIE -pie -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_executable(raytracer
        effect.h
        canvas.h
        color.h
        glwidget.h
        helper.h
        intersection.h
        mainwindow.h
        matrix.h
        shape.h
        point.h
        ray.h
        raytracer.h
        sphere.h
        tuple.h
        vector.h
        light.h
        material.h
        lighting.h
        world.h
        computations.h

        color.cpp
        glwidget.cpp
        helper.cpp
        main.cpp
        mainwindow.cpp
        mainwindow.ui
        point.cpp
        ray.cpp
        raytracer.cpp
        tuple.cpp
        )

target_link_libraries(raytracer Qt5::Core)
target_link_libraries(raytracer Qt5::Widgets)
