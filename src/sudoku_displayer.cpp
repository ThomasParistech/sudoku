/*********************************************************************************************************************
 * File : sudoku_displayer.cpp                                                                                       *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>
#include <iostream>
#include <sstream>
#include "sudoku_displayer.h"

void SudokuDisplayer::print_grid(CellToCharCb char_cb, bool use_long_line) const
{
    // Value + 1 because the value is shifted to [0;8]
    for (int i = 0; i < 9; i++)
    {
        if (3 * (i / 3) == i) // Multiple of 3
        {
            for (int k = 0; k < 3; k++)
                std::cout << (use_long_line ? long_horizontal_line_str : horizontal_line_str);
            std::cout << std::endl;
        }
        const int i0 = 9 * i;
        std::stringstream ss_row;
        for (int j0 = 0; j0 < 3; j0++)
        {
            ss_row << "| ";
            for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
            {
                char_cb(ss_row, i, j);
                ss_row << " ";
            }
        }
        std::cout << ss_row.str() << "|" << std::endl;
    }

    for (int k = 0; k < 3; k++)
        std::cout << (use_long_line ? long_horizontal_line_str : horizontal_line_str);
    std::cout << std::endl
              << std::endl;
}

void SudokuDisplayer::found_cells(const std::array<NewCell, 81> &cells)
{
    static int count = 0;
    std::cout << count++ << ") "
              << "Found cells" << std::endl;

    const auto char_cb = [&cells](std::stringstream &ss, int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
            ss << cell.get_value() + 1;
        else
            ss << " ";
    };

    print_grid(char_cb);
}

void SudokuDisplayer::candidates_per_cell(const std::array<NewCell, 81> &cells)
{
    static int count = 0;

    if (count > 12)
        return;

    std::cout << count++ << ") "
              << "Number of possible candidates in each cell" << std::endl;

    const auto char_cb = [&cells](std::stringstream &ss, int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
        {
            ss << "           ";
            return;
        }

        ss << '(';
        for (int val = 0; val < 9; val++)
        {
            if (cell.is_value_possible(val))
                ss << val;
            else
                ss << " ";
        }
        ss << ")";
    };

    print_grid(char_cb, true);
}

void SudokuDisplayer::num_candidates_per_cell(const std::array<NewCell, 81> &cells)
{
    static int count = 0;

    if (count > 12)
        return;

    std::cout << count++ << ") "
              << "Number of possible candidates in each cell" << std::endl;

    const auto char_cb = [&cells](std::stringstream &ss, int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (cell.is_set())
            ss << " ";
        else
            ss << cell.get_nbr_candidates();
    };

    print_grid(char_cb);
}

void SudokuDisplayer::candidate_cells_containing_val(short val,
                                                     const std::array<NewCell, 81> &cells)
{
    static int count = 0;
    std::cout << count++ << ") "
              << "Candidate cells that could take the value "
              << val + 1 << std::endl;

    const auto char_cb = [&cells, &val](std::stringstream &ss, int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (!cell.is_set() && cell.is_value_possible(val))
            ss << "x";
        else
            ss << " ";
    };

    print_grid(char_cb);
}

void SudokuDisplayer::num_candidates_per_cell_containing_val(short val,
                                                             const std::array<NewCell, 81> &cells)
{
    static int count = 0;
    std::cout << count++ << ") "
              << "Number of possible candidates in each non-found cell containing a "
              << val + 1 << std::endl;

    const auto char_cb = [&cells, &val](std::stringstream &ss, int i, int j) {
        const auto &cell = cells[9 * i + j];
        if (!cell.is_set() && cell.is_value_possible(val))
            ss << cell.get_nbr_candidates();
        else
            ss << " ";
    };

    print_grid(char_cb);
}
