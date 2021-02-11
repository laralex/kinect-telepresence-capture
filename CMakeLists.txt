cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(kinect-telepresence)

option(ENABLE_TESTS "Build unit tests executables" ON)
option(ENABLE_AZURE_KINECT_BODYTRACKING "Link Azure Kinect Bodytracking SDK and copy its ML model files (200 Mb)" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

# ==== artifacts config ====

string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE) # Debug to debug
string(TOUPPER "CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CMAKE_BUILD_TYPE}" OUTPUT_DIRECTORY) # debug to DEBUG
set(${OUTPUT_DIRECTORY} "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

# ==== external libraries ====

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

set(VENDOR_BINARY_DIR "${${OUTPUT_DIRECTORY}}/vendor")
set(VENDOR_SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor")

# log - spdlog
add_subdirectory("${VENDOR_SOURCE_DIR}/spdlog" "${VENDOR_BINARY_DIR}/spdlog")
list(APPEND VENDOR_INCLUDE_DIRS "${VENDOR_SOURCE_DIR}/spdlog/include")

# azure kinect SDK
find_package(k4a REQUIRED)
list(APPEND VENDOR_LIBS ${k4a_LIBS})
list(APPEND VENDOR_INCLUDE_DIRS "${k4a_INCLUDE_DIR}")

if (ENABLE_AZURE_KINECT_BODYTRACKING)
    # TODO(laralex): set specific minimal version
    find_package(k4abt REQUIRED)
    list(APPEND VENDOR_LIBS ${k4abt_LIBS})
    list(APPEND VENDOR_INCLUDE_DIRS "${k4abt_INCLUDE_DIR}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DENABLED_BODYTRACKING")
endif()

# json - nlohmann
add_subdirectory("${VENDOR_SOURCE_DIR}/nlohmann_json" "${VENDOR_BINARY_DIR}/nlohmann_json")
list(APPEND VENDOR_INCLUDE_DIRS "${VENDOR_SOURCE_DIR}/nlohmann_json/include")

# networking - asio
list(APPEND VENDOR_INCLUDE_DIRS "${VENDOR_SOURCE_DIR}/asio/asio/include")
list(APPEND VENDOR_INCLUDE_DIRS "${VENDOR_SOURCE_DIR}/asio/asio")
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINDOWS")
endif()

# parsing command line arguments 
add_subdirectory("${VENDOR_SOURCE_DIR}/cxxopts" "${VENDOR_BINARY_DIR}/cxxopts")
list(APPEND VENDOR_INCLUDE_DIRS "${VENDOR_SOURCE_DIR}/cxxopts/include")
list(APPEND VENDOR_LIBS cxxopts)

# ==== targets configuration utilities ====

set(OWN_LIBS camera network)
set(LIBS "${OWN_LIBS}" "${VENDOR_LIBS}")
set(OWN_INLUDE_DIRS "${CMAKE_SOURCE_DIR}/libs")
set(INCLUDE_DIRS "${OWN_INLUDE_DIRS}" "${VENDOR_INCLUDE_DIRS}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${$OUTPUT_DIRECTORY}}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${$OUTPUT_DIRECTORY}}")

# ==== project targets ====
add_subdirectory(libs "${${OUTPUT_DIRECTORY}}/obj/libs")
add_subdirectory(apps "${${OUTPUT_DIRECTORY}}/obj/apps")

if(ENABLE_TESTS)
    message(STATUS "Enabling tests")
    add_subdirectory(tests "${${OUTPUT_DIRECTORY}}/obj/tests")
endif()

# ==== compile with pedantic warnings ====
set(PEDANTIC_TARGETS ${KTP_EXECUTABLE_TARGETS} ${KTP_LIBRARY_TARGETS} ${KTP_TEST_TARGETS})
foreach(target ${PEDANTIC_TARGETS})
    target_compile_options(${target} -Wall -Wextra -Wpedantic -Werror)
endforeach()
