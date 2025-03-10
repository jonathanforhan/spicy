function(create_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE spicy_lib)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

    add_custom_command(
        TARGET ${TEST_NAME}
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND ${CMAKE_CTEST_COMMAND} -R "^${TEST_NAME}\$" "--output-on-failure"
    )
endfunction()

file(GLOB_RECURSE TEST_SOURCES  "${CMAKE_CURRENT_SOURCE_DIR}/*.test.cpp")

foreach(TEST IN LISTS TEST_SOURCES)
    # trim absolute path
    get_filename_component(FILE_NAME ${TEST} NAME_WE)

    # deduce testname
    string(REGEX REPLACE "([^.]+)\\.test\\.cpp" "\\1" TEST_BINARY ${FILE_NAME})

    # create test with (test_$name $name.test.cpp)
    create_test("test_${TEST_BINARY}" "${TEST}")
endforeach()

file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/netlists"
    DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)
