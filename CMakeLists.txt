cmake_minimum_required(VERSION 3.16)
project(Assignment C)

set(CMAKE_C_STANDARD 99)

add_executable(Assignment main.c main.h "position/position.h" "position/position.c" "movement/movement.c" "movement/movement.h" "file/file.c" "file/file.h" config/config.c config/config.h)