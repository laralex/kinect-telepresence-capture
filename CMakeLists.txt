cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(telepresence_capture)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# ==== artifacts config ====

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
string(TOUPPER "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}" OUTPUT_DIRECTORY) # Debug to DEBUG
set(${OUTPUT_DIRECTORY} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# ==== external libraries ====

# TODO(laralex): check if Debug project build builds Release externals
set(VENDOR_BINARY_DIR ${CMAKE_BINARY_DIR}/vendor)
set(VENDOR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor)

# spdlog - for logging
add_subdirectory(${VENDOR_SOURCE_DIR}/spdlog ${VENDOR_BINARY_DIR}/spdlog)
list(APPEND VENDOR_INCLUDE_DIRS ${VENDOR_SOURCE_DIR}/spdlog/include)

# ==== project libraries and executables ====

add_subdirectory(libs/3d      ${${OUTPUT_DIRECTORY}}/obj/libs/3d)
add_subdirectory(libs/camera  ${${OUTPUT_DIRECTORY}}/obj/libs/camera)
add_subdirectory(libs/network ${${OUTPUT_DIRECTORY}}/obj/libs/network)

add_subdirectory(apps/daemon  ${${OUTPUT_DIRECTORY}}/obj/apps/daemon)

message(STATUS "Building tests")
add_subdirectory(tests        ${${OUTPUT_DIRECTORY}}/obj/tests)

foreach(KTP_TARGET ${KTP_TARGETS})
    target_compile_options(${KTP_TARGET} -Wall -Wextra -Wpedantic -Werror)
endforeach()
