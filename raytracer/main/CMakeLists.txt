cmake_minimum_required(VERSION 3.13)
project(freeside-raytracer)

# Sanitizers
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g -O1")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fPIE -pie -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")

project(qml LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Quick Concurrent REQUIRED)

add_executable(

        # target
        freeside-raytracer

        # resources
        qml.qrc
        textures.qrc
        application.qrc

        # headers
        canvas.h
        color.h
        intersection.h
        matrix.h
        shape.h
        point.h
        ray.h
        raytracerbackend.h
        sphere.h
        tuple.h
        vector.h
        light.h
        material.h
        world.h
        computations.h
        imageitem.h
        plane.h
        pattern.h
        camera.h
        shapeqmlbridge.h
        worlds.h
        materials.h
        engine.h
        equal.h
        appsettings.h
        patterns.h
        cube.h
        cylinder.h

        # cpp
        sphere.cpp
        engine.cpp
        color.cpp
        main.cpp
        point.cpp
        raytracerbackend.cpp
        imageitem.cpp
        shapeqmlbridge.cpp
        appsettings.cpp
        cube.cpp
        cylinder.cpp
)

target_compile_definitions(freeside-raytracer PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(freeside-raytracer Qt5::Core)
target_link_libraries(freeside-raytracer Qt5::Quick)
target_link_libraries(freeside-raytracer Qt5::Concurrent)
