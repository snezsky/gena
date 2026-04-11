set(CMAKE_FOLDER "tests/integration")

add_executable(test_generator "test_generator.cpp")
target_link_libraries(test_generator PRIVATE generator)
gena_setup_target(test_generator)
gena_run_windeployqt(test_generator)

foreach (STD 17 20 23)
    foreach (TYPE lib exe)
        set(GEN_NAME ${TYPE}${STD})

        # generate test project with injected generator tests
        add_test(
            NAME generation_${GEN_NAME}
            COMMAND test_generator ${GEN_NAME})
        set_tests_properties(generation_${GEN_NAME} PROPERTIES FIXTURE_SETUP generation_${GEN_NAME})

        # build generated project and run it's tests + injected tests
        add_test(
            NAME build_and_test_${GEN_NAME}
            COMMAND ${CMAKE_COMMAND} -DGEN_NAME=${GEN_NAME} -P "${CMAKE_CURRENT_SOURCE_DIR}/RunTests.cmake")
        set_tests_properties(build_and_test_${GEN_NAME} PROPERTIES FIXTURE_REQUIRED generation_${GEN_NAME})
        
        # remove generated project directory
        add_test(
            NAME cleanup_${GEN_NAME}
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${GEN_NAME})
        set_tests_properties(cleanup_${GEN_NAME} PROPERTIES FIXTURE_CLEANUP generation_${GEN_NAME})

    endforeach()
endforeach()

message(STATUS "Copying tests assets...")
file(COPY "${PROJECT_SOURCE_DIR}/assets"         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/injected" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
