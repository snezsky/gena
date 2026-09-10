set(CMAKE_FOLDER "tests/integration")

add_executable(test_generation "test_generation.cpp")
target_link_libraries(test_generation PRIVATE generator)
gena_setup_target(test_generation)

foreach (PROJECT_TYPE library console widgets quick)

    # generate test project with injected generator tests
    add_test(
        NAME generation_${PROJECT_TYPE}
        COMMAND test_generation ${PROJECT_TYPE})
    set_tests_properties(generation_${PROJECT_TYPE} PROPERTIES FIXTURES_SETUP generation_${PROJECT_TYPE})
    
    # build generated project and run it's tests + injected tests
    add_test(
        NAME build_and_test_${PROJECT_TYPE}
        COMMAND ${CMAKE_COMMAND} -DPROJECT_TYPE=${PROJECT_TYPE} -P "${CMAKE_CURRENT_SOURCE_DIR}/RunTests.cmake")
    set_tests_properties(build_and_test_${PROJECT_TYPE} PROPERTIES FIXTURES_REQUIRED generation_${PROJECT_TYPE})
    
    # remove generated project directory
    add_test(
        NAME cleanup_${PROJECT_TYPE}
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${PROJECT_TYPE})
    set_tests_properties(cleanup_${PROJECT_TYPE} PROPERTIES FIXTURES_CLEANUP generation_${PROJECT_TYPE})

endforeach()

message(STATUS "Copying assets for integration tests...")
file(COPY "${PROJECT_SOURCE_DIR}/assets"         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/injected" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
