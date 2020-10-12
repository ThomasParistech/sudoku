/*********************************************************************************************************************
 * File : sudoku_displayer.cpp                                                                                       *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>
#include <iostream>
#include <sstream>
#include "sudoku_displayer.h"

void SudokuDisplayer::print_grid(CellToCharCb char_cb) const
{
    // Value + 1 because the value is shifted to [0;8]
    for (int i = 0; i < 9; i++)
    {
        if (3 * (i / 3) == i) // Multiple of 3
            std::cout << horizontal_line_str << std::endl;
        const int i0 = 9 * i;
        std::stringstream ss_row;
        for (int j0 = 0; j0 < 3; j0++)
        {
            ss_row << "| ";
            for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
                ss_row << char_cb(i, j) << " ";
        }
        ss_row << "|" << std::endl;
    }
    std::cout << horizontal_line_str << std::endl;
}

void SudokuDisplayer::found_cells(const std::array<NewCell, 81> &cells)
{
    std::cout << "Found cells" << std::endl;

    const auto char_cb = [&cells](int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
            return char(cell.get_value() + 1);
        return ' ';
    };

    print_grid(char_cb);
}

void SudokuDisplayer::num_candidates_per_cell(const std::array<NewCell, 81> &cells)
{
    std::cout << "Number of possible candidates in each cell" << std::endl;

    const auto char_cb = [&cells](int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
            return ' ';
        return char(cell.get_nbr_candidates());
    };

    print_grid(char_cb);
}

void SudokuDisplayer::num_candidates_per_cell_containing_val(short val,
                                                             const std::array<NewCell, 81> &cells)
{
    std::cout << "Number of possible candidates in each non-found cell containing a "
              << val + 1 << std::endl;

    const auto char_cb = [&cells, &val](int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
            return ' ';
        if (cell.is_value_possible(val))
            return char(cell.get_nbr_candidates());
        return ' ';
    };

    print_grid(char_cb);
}
