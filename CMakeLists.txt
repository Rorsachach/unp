cmake_minimum_required(VERSION 3.22)
project(unp C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(bin "${PROJECT_BINARY_DIR}/bin")
set(src "${PROJECT_SOURCE_DIR}/src")
set(include "${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH ${bin}/1-TCP)
add_executable(client ${src}/1-tcp/client.c)
add_executable(server ${src}/1-tcp/server.c)
