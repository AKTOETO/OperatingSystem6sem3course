cmake_minimum_required(VERSION 3.22)
project(lw3)

# c++ standart
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# c standart
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -pthread")

# path to all bins
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# subprojects
# add_subdirectory(deps/stb_image)

# exec files
file(GLOB_RECURSE source CONFIGURE_DEPENDS "src/*.h" "src/*.c")

# main
add_executable(main ${source})

# link stbimage to main with -lm linker flag
target_link_libraries(main PRIVATE m pthread)

# # if debug add flags
# if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fsanitize=address -g")
# endif()

#  # copy assets to bin folder
#  add_custom_target(
#     copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${PROJECT_SOURCE_DIR}/assets"
#         "${CMAKE_BINARY_DIR}/bin"
#     COMMENT "Copy assets from ${PROJECT_SOURCE_DIR}/assets to ${CMAKE_BINARY_DIR}/bin"
# )
# add_dependencies(main copy_assets)