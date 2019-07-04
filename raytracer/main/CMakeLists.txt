cmake_minimum_required(VERSION 3.14)
project(raytracer)

# Sanitizers
#message(WARNING "Sanitizer(s) enabled!")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIE -pie -g")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(qml LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Concurrent REQUIRED)

add_executable(raytracer
        qml.qrc
        canvas.h
        color.h
        intersection.h
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
        imageitem.h
        plane.h

        color.cpp
        main.cpp
        point.cpp
        ray.cpp
        raytracer.cpp
        tuple.cpp
        imageitem.cpp
        camera.h)

target_compile_definitions(raytracer PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(raytracer Qt5::Core)
target_link_libraries(raytracer Qt5::Quick)
target_link_libraries(raytracer Qt5::Concurrent)
