cmake_minimum_required(VERSION 3.21)

project(ChessEngine20Project)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:16777216") # Required to compute Magic Bitboards

# List of files for Main executable
file(GLOB_RECURSE SOURCE_LIST "src/*.cpp" "include/*.hpp" "dep/*.hpp" "test/*.cpp")
list(FILTER SOURCE_LIST EXCLUDE REGEX "src/TestMain\.cpp")

# List of files for Test executable
file(GLOB_RECURSE TEST_SOURCE_LIST "src/*.cpp" "include/*.hpp" "dep/*.hpp" "test/*.cpp")
list(FILTER TEST_SOURCE_LIST EXCLUDE REGEX "src/Main\.cpp")

include_directories(include dep/include)

add_executable(ChessEngine20 ${SOURCE_LIST})
add_executable(ChessEngine20Test ${TEST_SOURCE_LIST})









