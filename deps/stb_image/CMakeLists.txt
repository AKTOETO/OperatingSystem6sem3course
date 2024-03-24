cmake_minimum_required(VERSION 3.22)
project(stb)

file(GLOB_RECURSE hdrs "include/*.h")
add_library(${PROJECT_NAME} ${hdrs} stb_image.c)
target_include_directories(${PROJECT_NAME} PUBLIC include/)