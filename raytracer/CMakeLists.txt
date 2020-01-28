cmake_minimum_required(VERSION 3.13)
project("Freeside Raytracer")
#set(CMAKE_PREFIX_PATH /home/tobias/Qt/5.13.0/gcc_64/)
set(Qt5_DIR /home/tobias/Qt/5.13.0/gcc_64/lib/cmake/Qt5)

add_subdirectory(main)
add_subdirectory(catch)
