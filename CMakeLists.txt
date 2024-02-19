cmake_minimum_required(VERSION 3.22)
project(archivator)

set(CXX_STANDART 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")

# all .cpp
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
