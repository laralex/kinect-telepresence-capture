get_filename_component(BIN_PATH "." ABSOLUTE)
file(GLOB_RECURSE BIN_CXX_SOURCES ${BIN_PATH}/*.cpp)
file(GLOB_RECURSE BIN_HEADERS ${BIN_PATH}/*.hpp)

add_executable(capture_daemon ${BIN_CXX_SOURCES} ${BIN_HEADERS})
target_include_directories(capture_daemon PRIVATE ../../libs) 
target_link_libraries(capture_daemon network) 

set_target_properties(capture_daemon PROPERTIES OUTPUT_NAME "capture-daemon")