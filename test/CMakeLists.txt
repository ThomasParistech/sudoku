set(gtest_srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/new_line_gtest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(sudoku_gtest ${gtest_srcs} ${COMMON_SOURCES})

target_link_libraries(sudoku_gtest ${OpenCV_LIBS} ${Boost_LIBRARIES} gtest)

set(test_name sudoku-tests)
add_test(
    NAME ${test_name}
    COMMAND $<TARGET_FILE:sudoku_gtest> --gtest_color=yes
)