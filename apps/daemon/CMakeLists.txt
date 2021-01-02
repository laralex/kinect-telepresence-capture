get_filename_component(BIN_PATH "." ABSOLUTE)
file(GLOB_RECURSE BIN_CXX_SOURCES ${BIN_PATH}/*.cpp)
file(GLOB_RECURSE BIN_HEADERS ${BIN_PATH}/*.hpp)

add_executable(ktp_capture_daemon ${BIN_CXX_SOURCES} ${BIN_HEADERS})
target_include_directories(ktp_capture_daemon PRIVATE ../../libs ${VENDOR_INCLUDE_DIRS}) 
target_link_libraries(ktp_capture_daemon ktp_network) 

list(APPEND ${KTP_TARGETS} ktp_capture_daemon)
set_target_properties(ktp_capture_daemon PROPERTIES OUTPUT_NAME "capture-daemon")