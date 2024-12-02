# Function to initialize all submodules, less headache for user
find_package(Git QUIET)
if(GIT_FOUND)
    option(UPDATE_SUBMODULES "Check submodules during build" ON)
    if(NOT UPDATE_SUBMODULES)
        return()
    endif()
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    OUTPUT_VARIABLE EXISTING_SUBMODULES
                    RESULT_VARIABLE RETURN_CODE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Updating git submodules:\n${EXISTING_SUBMODULES}")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE RETURN_CODE)
    if(NOT RETURN_CODE EQUAL "0")
        message(WARNING "Cannot update submodules. Git command failed with ${RETURN_CODE}")
        return()
    endif()
    message(STATUS "Git submodules updated successfully")
endif()

# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(chess)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# Group all files based on what they are, source or header files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")
include_directories(${PROJECT_SOURCE_DIR}/include/board)
include_directories(${PROJECT_SOURCE_DIR}/include/util)
include_directories(${PROJECT_SOURCE_DIR}/include/pieces)
include_directories(${PROJECT_SOURCE_DIR}/include/game)

# Include googletest dependency, used for unit tests
add_subdirectory(lib/googletest)

# Create static libary for unit tests to reference
add_library(chess_lib STATIC ${SOURCES} ${HEADERS})

# create chess executable
add_executable(chess ${SOURCES})
target_link_libraries(chess gtest gtest_main chess_lib)

# enable unit tests via cmake
enable_testing()
add_subdirectory(test)