cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(kinect-telepresence)

option(ENABLE_TESTS "Build unit tests executables" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# ==== artifacts config ====

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE) # Debug to debug
string(TOUPPER "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}" OUTPUT_DIRECTORY) # debug to DEBUG
set(${OUTPUT_DIRECTORY} ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

# ==== external libraries ====

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(VENDOR_BINARY_DIR ${${OUTPUT_DIRECTORY}}/vendor)
set(VENDOR_SOURCE_DIR ${CMAKE_SOURCE_DIR}/vendor)

# spdlog
add_subdirectory(${VENDOR_SOURCE_DIR}/spdlog ${VENDOR_BINARY_DIR}/spdlog)
list(APPEND VENDOR_INCLUDE_DIRS ${VENDOR_SOURCE_DIR}/spdlog/include)

# azure kinect SDK
find_package(k4a REQUIRED)
list(APPEND VENDOR_LIBS ${k4a_LIBS})
list(APPEND VENDOR_INCLUDE_DIRS ${k4a_INCLUDE_DIR})

# ==== targets configuration utilities ====

set(LIBS camera network ${VENDOR_LIBS})
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libs ${VENDOR_INCLUDE_DIRS})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${$OUTPUT_DIRECTORY}})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${$OUTPUT_DIRECTORY}})

# ==== project targets ====
add_subdirectory(libs ${${OUTPUT_DIRECTORY}}/obj/libs)
add_subdirectory(apps ${${OUTPUT_DIRECTORY}}/obj/apps)

if(ENABLE_TESTS)
    message(STATUS "Enabling tests")
    add_subdirectory(tests ${${OUTPUT_DIRECTORY}}/obj/tests)
endif()

# ==== compile with pedantic warnings ====
set(PEDANTIC_TARGETS ${KTP_EXECUTABLE_TARGETS} ${KTP_LIBRARY_TARGETS} ${KTP_TEST_TARGETS})
foreach(target ${PEDANTIC_TARGETS})
    target_compile_options(${target} -Wall -Wextra -Wpedantic -Werror)
endforeach()
