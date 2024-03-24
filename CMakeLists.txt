cmake_minimum_required(VERSION 3.22)
project(lw3)

# c++ standart
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# c standart
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lm")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -lm")

# path to all bins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# subprojects
add_subdirectory(deps/stb_image)

# exec files
file(GLOB_RECURSE source CONFIGURE_DEPENDS "src/*.h" "src/*.c")

# main
add_executable(main ${source})
# link stbimage to main with -lm linker flag
target_link_libraries(main stb m)

# if debug add flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")
endif()