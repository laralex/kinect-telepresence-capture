get_filename_component(LIB_PATH "." ABSOLUTE)
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

add_library(network STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
