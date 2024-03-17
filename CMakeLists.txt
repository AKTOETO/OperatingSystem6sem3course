cmake_minimum_required(VERSION 3.22)
project(lw3)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

# if debug add flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")
endif()

message(STATUS "Compile flags: ${CMAKE_C_FLAGS}")

# path to all bins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# include folder
set(INCLUDES ${PROJECT_SOURCE_DIR}/includes)

# src folder
set(SRC ${PROJECT_SOURCE_DIR}/src)

# error lib
add_library(errors STATIC ${SRC}/errors.c ${INCLUDES}/errors.h)
target_include_directories(errors PRIVATE ${INCLUDES})

# file lib
add_library(files STATIC ${SRC}/file.c ${INCLUDES}/file.h)
target_include_directories(files PRIVATE ${INCLUDES})

# included libs
set(LINK_LIBS ${errors} ${files})

# reader
set(READER_C ${SRC}/reader.c)
add_executable(reader ${READER_C})
target_include_directories(reader PRIVATE ${INCLUDES})
target_link_libraries(reader errors files)

# xorer
set(XORER_C ${SRC}/reader.c)
add_executable(xorer ${XORER_C})
target_include_directories(xorer PRIVATE ${INCLUDES})
target_link_libraries(xorer errors files)


# # all .c
# file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.c)

# # all .h
# file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/INCLUDES/*.h)

# add_executable(${CMAKE_PROJECT_NAME} ${SRC} ${HDRS})

#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/INCLUDES)

# cmake -B build . && cmake --build build
# ./build/archivate 