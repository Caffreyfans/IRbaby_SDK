cmake_minimum_required(VERSION 3.10)
project(TEST)

add_subdirectory(3rdparty)

file(GLOB ALL_SRCS "src/*.c" "src/port/*.c")
set(includes
    "include"
    "include/port")
add_library(irbaby ${ALL_SRCS})
target_include_directories(irbaby PUBLIC ${includes})

#add_subdirectory(test)