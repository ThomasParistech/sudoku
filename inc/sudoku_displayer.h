/*********************************************************************************************************************
 * File : sudoku_displayer.h                                                                                         *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SUDOKU_DISPLAYER_H
#define SUDOKU_DISPLAYER_H

#include <functional>

#include "new_cell.h"
#include "new_square.h"

class SudokuDisplayer
{
public:
    /// @brief Displays in the console the grid wih the values that have been found
    /// @param cells 9x9 grid of cells
    void found_cells(const std::array<NewCell, 81> &cells);

    void num_candidates_per_cell(const std::array<NewCell, 81> &cells);

    void num_candidates_per_cell_containing_val(short val, const std::array<NewCell, 81> &cells);

private:
    /// @brief Callback to get a character from a location (i,j)
    using CellToCharCb = std::function<char(int, int)>;

    void print_grid(CellToCharCb char_cb) const;

    const char *horizontal_line_str = " ______ _______ _______ ";
};

#endif // SUDOKU_DISPLAYER_H