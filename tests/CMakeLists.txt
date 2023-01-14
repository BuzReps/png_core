add_executable(chunk_data_test_suite png_core/chunk_data.cpp)
target_link_libraries(chunk_data_test_suite 
  PRIVATE
  png_core
  GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(chunk_data_test_suite)

