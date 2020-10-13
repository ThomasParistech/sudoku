cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
project(Sudoku)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/ CACHE PATH "Output directory of all executables.")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lgtest")

find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system program_options)
include(CTest)
find_package(GTest REQUIRED)
enable_testing()

include_directories(inc)
add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(test)
