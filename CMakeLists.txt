cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(telepresence_capture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# ==== artifacts config ====

string(TOUPPER "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}" OUTPUT_DIRECTORY) # Debug to DEBUG
set(${OUTPUT_DIRECTORY} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# ==== project libraries and executables ====

add_subdirectory(libs/3d      ${${OUTPUT_DIRECTORY}}/obj/libs/3d)
add_subdirectory(libs/camera  ${${OUTPUT_DIRECTORY}}/obj/libs/camera)
add_subdirectory(libs/network ${${OUTPUT_DIRECTORY}}/obj/libs/network)

add_subdirectory(apps/daemon  ${${OUTPUT_DIRECTORY}}/obj/apps/daemon)

message(STATUS "Building tests")
add_subdirectory(tests        ${${OUTPUT_DIRECTORY}}/obj/tests)
