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
            {
                char_cb(ss_row, i, j);
                ss_row << " ";
            }
        }
        std::cout << ss_row.str() << "|" << std::endl;
    }
    std::cout << horizontal_line_str << std::endl
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

void SudokuDisplayer::num_candidates_per_cell(const std::array<NewCell, 81> &cells)
{
    static int count = 0;


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

void SudokuDisplayer::counts_on_rows(short val, const std::array<NewLine, 9> &rows)
{
    static int count = 0;

    std::cout << count++ << ") "
              << "Possibilities on row for value "
              << val + 1 << std::endl;

    const auto char_cb = [&rows, &val](std::stringstream &ss, int i, int j) {
        const auto &bitset = rows[i].get_bitset(val);
        if (bitset.none())
            ss << " ";
        else
            ss << bitset[j];
    };

    print_grid(char_cb);
}

void SudokuDisplayer::counts_on_cols(short val, const std::array<NewLine, 9> &cols)
{
    static int count = 0;
    std::cout << count++ << ") "
              << "Possibilities on cols for value "
              << val + 1 << std::endl;

    const auto char_cb = [&cols, &val](std::stringstream &ss, int i, int j) {
        const auto &bitset = cols[j].get_bitset(val);
        if (bitset.none())
            ss << " ";
        else
            ss << bitset[i];
    };

    print_grid(char_cb);
}