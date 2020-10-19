set(gtest_srcs
    ${CMAKE_CURRENT_SOURCE_DIR}/fisherman_gtest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/line_gtest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(gtest_sudoku ${gtest_srcs} ${COMMON_SOURCES})

target_link_libraries(gtest_sudoku ${OpenCV_LIBS} ${Boost_LIBRARIES} gtest)

set(test_name sudoku-tests)
add_test(
    NAME ${test_name}
    COMMAND $<TARGET_FILE:gtest_sudoku> --gtest_color=yes
)