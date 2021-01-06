#.rst:
# Findk4a
# -------
#
# Find Azure Kinect Sensor SDK include dirs, and libraries.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines the :prop_tgt:`IMPORTED` targets:
#
# ``${iter_lib}``
#  Defined if the system has Azure Kinect Sensor SDK.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module sets the following variables:
#
# ::
#
#   k4a_FOUND               True in case Azure Kinect Sensor SDK is found, otherwise false
#   k4a_ROOT                Path to the root of found Azure Kinect Sensor SDK installation
#
# Example usage
# ^^^^^^^^^^^^^
#
# ::
#
#     find_package(k4a REQUIRED)
#
#     add_executable(foo foo.cc)
#     target_link_libraries(foo ${iter_lib})
#
# License
# ^^^^^^^
#
# Copyright (c) 2019 Tsukasa SUGIURA
# Distributed under the MIT License.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

find_path(k4a_INCLUDE_DIR
  NAMES
    k4a/k4a.h
  HINTS
    $ENV{K4A_DIR}/sdk/
    /usr/include
  PATHS
    "${K4A_DIR}/sdk/"
  PATH_SUFFIXES
    include
  REQUIRED
)

find_library(k4a_LIBRARY 
  NAMES 
    k4a 
    libk4a
  HINTS
    $ENV{K4A_DIR}/sdk/windows-desktop/amd64/release
    /usr/lib
  PATHS
    "${K4A_DIR}/sdk/windows-desktop/amd64/release"
  PATH_SUFFIXES
    lib
    x86_64-linux-gnu
  REQUIRED
)

find_library(k4arecord_LIBRARY 
  NAMES 
    k4arecord 
    lib4arecord
  HINTS
    $ENV{K4A_DIR}/sdk/windows-desktop/amd64/release
    /usr/lib
  PATHS
    "${K4A_DIR}/sdk/windows-desktop/amd64/release"
  PATH_SUFFIXES
    lib
    x86_64-linux-gnu
  REQUIRED
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  k4a DEFAULT_MSG
  k4a_LIBRARY k4a_INCLUDE_DIR
)
find_package_handle_standard_args(
  k4arecord DEFAULT_MSG
  k4arecord_LIBRARY k4a_INCLUDE_DIR
)

if(k4a_FOUND)
  if(CMAKE_HOST_WIN32)
    get_filename_component(k4a_SHARED_LIBS_DIR "${k4a_LIBRARY}" DIRECTORY) # lib
    get_filename_component(k4a_SHARED_LIBS_DIR "${k4a_SHARED_LIBS_DIR}" DIRECTORY) # release
    file(GLOB k4a_DLLS "${k4a_SHARED_LIBS_DIR}/bin/*.dll")
  endif()
  
  # add_library(k4a::k4a UNKNOWN IMPORTED GLOBAL)
  # set_target_properties(k4a::k4a PROPERTIES
  #   INTERFACE_INCLUDE_DIRECTORIES ${k4a_INCLUDE_DIR}
  #   IMPORTED_LOCATION ${k4a_LIBRARY}
  #   IMPORTED_LOCATION_DEBUG ${k4a_LIBRARY})

  # add_library(k4a::k4a_record UNKNOWN IMPORTED)
  # set_target_properties(k4a::k4a_record PROPERTIES
  #   INTERFACE_INCLUDE_DIRECTORIES ${k4a_INCLUDE_DIR}
  #   IMPORTED_LOCATION ${k4arecord_LIBRARY}
  #   IMPORTED_LOCATION_DEBUG ${k4arecord_LIBRARY})

  set(k4a_LIBS k4a::k4a k4a::k4a_record)
  set(k4a_LIBS_FILES ${k4a_LIBRARY} ${k4arecord_LIBRARY})

  foreach(i RANGE 1)
    list(GET k4a_LIBS ${i} iter_lib)
    list(GET k4a_LIBS_FILES ${i} iter_lib_file)
    
    add_library(${iter_lib} SHARED IMPORTED)

    set_target_properties(${iter_lib} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${k4a_INCLUDE_DIR}")
    set_property(TARGET ${iter_lib} APPEND PROPERTY IMPORTED_CONFIGURATIONS "RELEASE")
    set_target_properties(${iter_lib} PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX")
    if(WIN32)
      set_target_properties(${iter_lib} PROPERTIES IMPORTED_IMPLIB_RELEASE "${iter_lib_file}")
    else()
      set_target_properties(${iter_lib} PROPERTIES IMPORTED_LOCATION_RELEASE "${iter_lib_file}")
    endif()

    set_property(TARGET ${iter_lib} APPEND PROPERTY IMPORTED_CONFIGURATIONS "DEBUG")
    set_target_properties(${iter_lib} PROPERTIES IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX")
    if(WIN32)
      set_target_properties(${iter_lib} PROPERTIES IMPORTED_IMPLIB_DEBUG "${iter_lib_file}")
    else()
      set_target_properties(${iter_lib} PROPERTIES IMPORTED_LOCATION_DEBUG "${iter_lib_file}")
    endif()

  endforeach()
endif()


# find_package_handle_standard_args(
#   k4a_depth DEFAULT_MSG
#   k4a_depth_LIBRARY k4a_INCLUDE_DIR
# )

# if(k4a_FOUND)
#   #set(k4a_LIBS k4a k4a_record k4a_depth)
#   
# endif()
