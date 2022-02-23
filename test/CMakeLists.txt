# Build the test
add_executable (simple_tests simple_tests.c)
target_include_directories (simple_tests PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries (simple_tests ${HDF5_C_LIBRARIES})

# Add the test
add_test (simple_tests simple_tests)
set_tests_properties(simple_tests PROPERTIES
    ENVIRONMENT "HDF5_PLUGIN_PATH=${PROJECT_BINARY_DIR}/src")
