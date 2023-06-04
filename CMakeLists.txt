cmake_minimum_required(VERSION 3.22)
project(unp C)

set(CMAKE_C_STANDARD 11)

set(Sources
    main.c
)

include_directories(include)
add_executable(unp ${Sources})
