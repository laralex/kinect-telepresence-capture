macro(create_executable target_name src_dir dlls)
    file(GLOB_RECURSE CXX_SOURCES ${src_dir}/*.cpp)
    file(GLOB_RECURSE CXX_HEADERS ${src_dir}/*.hpp)

    add_executable(${target_name} ${CXX_SOURCES} ${CXX_HEADERS})
    target_include_directories(${target_name} PRIVATE ${INCLUDE_DIRS}) 
    target_link_libraries(${target_name} ${LIBS}) 

    list(APPEND ${KTP_EXECUTABLE_TARGETS} ${target_name})
    if(CMAKE_HOST_WIN32)
        message(STATUS "Copying DLLs to executable folder of ${target_name}")
        foreach(dll ${dlls})
            add_custom_command(TARGET ${target_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}" 
                $<TARGET_FILE_DIR:${target_name}>)
        endforeach()
    endif() 
endmacro()

# ==== executable targets ====
get_filename_component(KTP_EXECUTABLES_SRC_DIR "." ABSOLUTE)

create_executable(capture_daemon "${KTP_EXECUTABLES_SRC_DIR}/daemon" "${k4a_DLLS}")
set_target_properties(capture_daemon PROPERTIES OUTPUT_NAME "ktp_capture_daemon")