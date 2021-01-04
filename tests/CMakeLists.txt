include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${VENDOR_SOURCE_DIR}/googletest ${VENDOR_BINARY_DIR}/googletest)
set(gtest_targets gtest gtest_main)
set(KTP_TESTS_INCLUDES ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../libs/ ${VENDOR_INCLUDE_DIRS})

macro(create_test_target TESTNAME FILES)
    add_executable(${TESTNAME} ${FILES})
    target_include_directories(${TESTNAME} PRIVATE ${KTP_TESTS_INCLUDES})
    target_link_libraries(${TESTNAME} ${gtest_targets} ${ARGN})
    gtest_discover_tests(${TESTNAME}
    SOURCES ${FILES}
    WORKING_DIRECTORY "${${OUTPUT_DIRECTORY}}"
    PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${${OUTPUT_DIRECTORY}}"
)
list(APPEND ${KTP_TARGETS} ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER mygtests)
endmacro()

# ==== tests ====
file(GLOB TEST_SRC_FILES "test_*.cpp")

enable_testing()
foreach(TEST_SRC_FILE ${TEST_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SRC_FILE} NAME_WE)
    create_test_target(${TEST_NAME} ${TEST_SRC_FILE} ktp_network ktp_camera ${VENDOR_LIBS})
endforeach()

add_executable(all_tests ${TEST_SRC_FILES})
target_include_directories(all_tests PRIVATE ${KTP_TESTS_INCLUDES})
target_link_libraries(all_tests ${gtest_targets} ktp_network ktp_camera ${VENDOR_LIBS})
set_target_properties(all_tests PROPERTIES FOLDER tests)
list(APPEND ${KTP_TARGETS} all_tests)
add_test(NAME all_tests COMMAND all_tests)

# ==== keep cache cleaner ====
# https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

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