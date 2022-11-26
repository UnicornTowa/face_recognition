cmake_minimum_required(VERSION 3.20)
project(face_rec_double)


set(CMAKE_CXX_STANDARD 20)

include_directories(C:/Users/tosha/CLionProjects/lib/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/) #path to SFML library

set(SFML_DIR C:/Users/tosha/CLionProjects/lib/SFML-2.5.1-windows-gcc-7.3.0-mingw-64-bit/SFML-2.5.1/lib/cmake/SFML) #path to SFML cmake dir

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 COMPONENTS graphics REQUIRED)


add_executable(face_rec_double main.cpp functions.cpp header.h sfml.cpp)

target_link_libraries(face_rec_double sfml-graphics)
