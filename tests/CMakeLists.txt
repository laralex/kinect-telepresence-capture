include(GoogleTest)
enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${VENDOR_SOURCE_DIR}/googletest" "${VENDOR_BINARY_DIR}/googletest")

macro(create_test_target TESTNAME FILES LIBS)
    add_executable(${TESTNAME} "${FILES}")
    target_include_directories(${TESTNAME} PRIVATE "${INCLUDE_DIRS}" "${gtest_SOURCE_DIR}/include" "${gtest_SOURCE_DIR}")
    target_link_libraries(${TESTNAME} gtest_main ${LIBS})
    target_compile_features(${TESTNAME} PUBLIC ${ARGN})
    gtest_discover_tests(${TESTNAME}
        SOURCES "${FILES}"
        WORKING_DIRECTORY "${${OUTPUT_DIRECTORY}}"
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${${OUTPUT_DIRECTORY}}"
    )
    list(APPEND ${KTP_TEST_TARGETS} ${TESTNAME})
endmacro()

# ==== tests ====
file(GLOB TEST_SRC_FILES "test_*.cpp")

create_test_target(test_camera_kinect     test_camera_kinect.cpp     "${LIBS}")
create_test_target(test_asio_compilation  test_asio_compilation.cpp  "${LIBS}")
create_test_target(test_nlohmann_json     test_nlohmann_json.cpp     "${LIBS}")

create_test_target(all_tests "${TEST_SRC_FILES}" "${LIBS}")
add_test(NAME all_tests COMMAND all_tests)


# ==== keep cache cleaner ====
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# ==== keep artifacts cleaner ====
set(gtest_targets gtest gtest_main gmock gmock_main)
foreach(gtest_target ${gtest_targets})
    set_target_properties(${gtest_target}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${VENDOR_BINARY_DIR}/googletest/bin"
        LIBRARY_OUTPUT_DIRECTORY "${VENDOR_BINARY_DIR}/googletest/lib"
        ARCHIVE_OUTPUT_DIRECTORY "${VENDOR_BINARY_DIR}/googletest/lib"
        PDB_OUTPUT_DIRECTORY "${VENDOR_BINARY_DIR}/googletest/bin"
        FOLDER myglib)
endforeach()