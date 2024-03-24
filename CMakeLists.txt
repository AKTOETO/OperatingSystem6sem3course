cmake_minimum_required(VERSION 3.22)
project(lw3)

# c++ standart
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
target_link_libraries(main stb)

# if debug add flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(main PRIVATE "-lm -Wall -Werror -fsanitize=address -g")
    target_compile_options(main PRIVATE "-lm -Wall -Werror -fsanitize=address -g")
endif()
# target_include_directories(main PRIVATE ${INCLUDES})
# target_link_libraries(main ${LINK_LIBS})

# # reader
# add_executable(reader ${SRC}/reader.c)
# target_include_directories(reader PRIVATE ${INCLUDES})
# target_link_libraries(reader ${LINK_LIBS})

# # xorer
# add_executable(xorer ${SRC}/xorer.c)
# target_include_directories(xorer PRIVATE ${INCLUDES})
# target_link_libraries(xorer ${LINK_LIBS})

# # close
# add_executable(clear ${SRC}/clear.c)
# target_include_directories(clear PRIVATE ${INCLUDES})
# target_link_libraries(clear ${LINK_LIBS})

# # generator
# add_executable(generator ${SRC}/generator.c)
# target_include_directories(generator PRIVATE ${INCLUDES})
# target_link_libraries(generator ${LINK_LIBS})

# # all .c
# file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.c)

# # all .h
# file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/includes/*.h)

# add_executable(${CMAKE_PROJECT_NAME} ${SRC} ${HDRS})

#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/INCLUDES)

# cmake -B build . && cmake --build build
# ./build/archivate 