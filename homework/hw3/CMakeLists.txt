cmake_minimum_required(VERSION 3.16)
project(hw3 C)

set(CMAKE_C_STANDARD 11)

add_executable(hw3
#        cmd.c
        sort.c list.c list.h ui.c ui.h
        menu.c
        )

