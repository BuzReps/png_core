# Creates test executable target
# Performs common functions like appending utils sources and liking to libs

include(GoogleTest)
function(CreateTestSuiteExecutable target_name test_source)
  set(target_sources
    test_utils.cpp
    ${test_source}
  )

  add_executable(${target_name} ${target_sources})

  target_link_libraries(${target_name}
    PRIVATE
    png_core
    GTest::gtest_main
  )

  gtest_discover_tests(${target_name}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endfunction()

CreateTestSuiteExecutable(chunk_data_test_suite png_core/chunk_data.cpp)
CreateTestSuiteExecutable(chunk_types_test_suite png_core/chunk_types.cpp)

