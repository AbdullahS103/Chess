# To add new test:
# Group the test source files
# ---> file (GLOB TEST_SOURCE "path/to/cpp")
# Create the unit test executable for google test
# ---> add_executable(ExecutableName ${TEST_SOURCE})
# Link associated libraries required for executable
# ---> target_link_libraries(ExecutableName gtest gtest_main chess_lib)
# Add the test
# ---> add_test(NAME TestName COMMAND ExecutableName)

# King Unit Tests
file(GLOB KING_TEST_SOURCE "pieces/KingTest.cpp")
add_executable(KingTests ${KING_TEST_SOURCE})
target_link_libraries(KingTests gtest gtest_main chess_lib)
add_test(NAME KingTests COMMAND KingTests)

# Knight Unit Tests
file(GLOB KNIGHT_TEST_SOURCE "pieces/KnightTest.cpp")
add_executable(KnightTests ${KNIGHT_TEST_SOURCE})
target_link_libraries(KnightTests gtest gtest_main chess_lib)
add_test(NAME KnightTests COMMAND KnightTests)

# Rook Unit Tests
file(GLOB ROOK_TEST_SOURCE "pieces/RookTest.cpp")
add_executable(RookTests ${ROOK_TEST_SOURCE})
target_link_libraries(RookTests gtest gtest_main chess_lib)
add_test(NAME RookTests COMMAND RookTests)
