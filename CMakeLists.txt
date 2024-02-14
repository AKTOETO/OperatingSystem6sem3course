cmake_minimum_required(VERSION 3.22)
project(archivator)

set(CXX_STANDART 20)

# all .cpp
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC})
