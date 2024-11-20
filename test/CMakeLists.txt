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

# Bishop Unit Tests
file(GLOB BISHOP_TEST_SOURCE "pieces/BishopTest.cpp")
add_executable(BishopTests ${BISHOP_TEST_SOURCE})
target_link_libraries(BishopTests gtest gtest_main chess_lib)
add_test(NAME BishopTests COMMAND BishopTests)

# Queen Unit Tests
file(GLOB QUEEN_TEST_SOURCE "pieces/QueenTest.cpp")
add_executable(QueenTests ${QUEEN_TEST_SOURCE})
target_link_libraries(QueenTests gtest gtest_main chess_lib)
add_test(NAME QueenTests COMMAND QueenTests)

# Pawn Unit Tests
file(GLOB PAWN_TEST_SOURCE "pieces/PawnTest.cpp")
add_executable(PawnTests ${PAWN_TEST_SOURCE})
target_link_libraries(PawnTests gtest gtest_main chess_lib)
add_test(NAME PawnTests COMMAND PawnTests)