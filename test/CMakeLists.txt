# Add the test source files
file(GLOB TEST_SOURCES "*.cpp")

# Create the unit test executable for Google Test
# add_executable(bishop_test BishopTest.cpp)
# add_executable(king_test KingTest.cpp)
add_executable(ChessTests ${TEST_SOURCES})

# Link Google Test and the main project executable
# target_link_libraries(bishop_test gtest gtest_main chess_lib)
# target_link_libraries(king_test gtest gtest_main chess_lib)
target_link_libraries(ChessTests gtest gtest_main chess_lib)

# add_test(NAME BishopTests COMMAND bishop_test)
# add_test(NAME KingTests COMMAND king_test)
add_test(NAME ChessTests COMMAND ChessTests)
