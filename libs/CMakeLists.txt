macro(create_library target_name library_type src_dir)
    file(GLOB_RECURSE CXX_SOURCES "${src_dir}/*.cpp")
    file(GLOB_RECURSE CXX_HEADERS "${src_dir}/*.hpp")

    add_library(${target_name} ${library_type} "${CXX_SOURCES}" "${CXX_HEADERS}")
    target_include_directories(${target_name} PRIVATE "${VENDOR_INCLUDE_DIRS}")
    target_link_libraries(${target_name} ${VENDOR_LIBS})
    list(APPEND ${KTP_LIBRARY_TARGETS} ${target_name})
endmacro()

get_filename_component(KTP_LIBRARIES_SRC_DIR "." ABSOLUTE)

create_library(camera STATIC "${KTP_LIBRARIES_SRC_DIR}/camera")
create_library(network STATIC "${KTP_LIBRARIES_SRC_DIR}/network")
#create_library(3d STATIC "${KTP_LIBRARIES_SRC_DIR}/3d")