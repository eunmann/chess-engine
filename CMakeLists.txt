cmake_minimum_required(VERSION 3.21)

project(TestChessProject)

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:16777216")

file(GLOB_RECURSE SOURCE_LIST "src/*.cpp" "include/*.hpp" "dep/*.hpp" "test/*.cpp")
include_directories(include dep/include)
add_executable(TestChessProject ${SOURCE_LIST})










