cmake_minimum_required(VERSION 3.14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

include_directories(/usr/include)
include_directories(../main)
include_directories(.)

add_executable(catchtests
        ../main/canvas.h
        ../main/color.cpp
        ../main/color.h
        ../main/intersection.cpp
        ../main/intersection.h
        ../main/matrix.h
        ../main/object.cpp
        ../main/object.h
        ../main/point.cpp
        ../main/point.h
        ../main/ray.cpp
        ../main/ray.h
        ../main/sphere.cpp
        ../main/sphere.h
        ../main/tuple.cpp
        ../main/tuple.h
        ../main/vector.cpp
        ../main/vector.h

        main.cpp
        tests.cpp)

target_link_libraries(catchtests Qt5::Core)
