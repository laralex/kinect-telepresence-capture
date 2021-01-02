cmake_minimum_required(VERSION 3.10)

project(telepresence_capture)

set(BIN_NAME_DAEMON capture-daemon)
set(BIN_NAME_WEB capture-web-gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_subdirectory(apps/daemon)
add_subdirectory(apps/web)
add_subdirectory(libs/3d)
add_subdirectory(libs/camera)
add_subdirectory(libs/network)

add_subdirectory(tests)
