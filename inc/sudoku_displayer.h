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
    SudokuDisplayer() = default;

    /// @brief Displays in the console the grid wih the values that have been found
    /// @param cells 9x9 grid of cells
    void found_cells(const std::array<NewCell, 81> &cells);

    void candidates_per_cell(const std::array<NewCell, 81> &cells);

    void num_candidates_per_cell(const std::array<NewCell, 81> &cells);

    void candidate_cells_containing_val(short val, const std::array<NewCell, 81> &cells);

    void num_candidates_per_cell_containing_val(short val, const std::array<NewCell, 81> &cells);

private:
    /// @brief Callback to append a character to a stream from a location (i,j)
    using CellToCharCb = std::function<void(std::stringstream &, int, int)>;

    void print_grid(CellToCharCb char_cb, bool use_long_line = false) const;

    const char *horizontal_line_str = " _______";
    const char *long_horizontal_line_str = " _____________________________________";
};

#endif // SUDOKU_DISPLAYER_H