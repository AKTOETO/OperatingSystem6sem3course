cmake_minimum_required(VERSION 3.22)
project(archivator)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -std=c11")

# all .c
file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.c)

# all .h
file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/includes/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SRC} ${HDRS})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/includes)

# cmake -B build . && cmake --build build
# ./build/archivate 