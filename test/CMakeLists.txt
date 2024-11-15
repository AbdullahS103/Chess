# Add the test source files
file(GLOB KING_TEST_SOURCES "pieces/*.cpp")

# Create the unit test executable for Google Test
add_executable(KingTests ${KING_TEST_SOURCES})

# Link Google Test and the main project executable
target_link_libraries(KingTests gtest gtest_main chess_lib)

# Add the test
add_test(NAME KingTests COMMAND KingTests)
