cmake_minimum_required(VERSION 3.14)
project(tests)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(../main/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

add_executable(tests
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
        tst_tests.cpp)

target_link_libraries(tests Qt5::Core)
target_link_libraries(tests Qt5::Widgets)
target_link_libraries(tests Qt5::Test)
