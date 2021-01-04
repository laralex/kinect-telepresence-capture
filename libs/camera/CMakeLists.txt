get_filename_component(LIB_PATH "." ABSOLUTE)
file(GLOB_RECURSE LIB_CXX_SOURCES ${LIB_PATH}/*.cpp)
file(GLOB_RECURSE LIB_HEADERS ${LIB_PATH}/*.hpp)

add_library(ktp_camera STATIC ${LIB_CXX_SOURCES} ${LIB_HEADERS})
target_include_directories(ktp_camera PRIVATE ${VENDOR_INCLUDE_DIRS})
target_link_libraries(ktp_camera ${VENDOR_LIBS})
list(APPEND ${KTP_TARGETS} ktp_camera)