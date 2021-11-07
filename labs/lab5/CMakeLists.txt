cmake_minimum_required(VERSION 3.16)
project(lab5 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(lab5
        lab5_dlist.h lab5_dlist.c)
