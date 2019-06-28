cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)

include_directories(../main)

add_executable(catchtests
        ../main/color.cpp
        ../main/point.cpp
        ../main/ray.cpp
        ../main/tuple.cpp

        main.cpp
        chapter1.cpp
        chapter2.cpp
        chapter3.cpp
        chapter4.cpp
        chapter5.cpp
        chapter6.cpp
        chapter7.cpp
        chapter8.cpp
        )

target_link_libraries(catchtests Qt5::Core)
