/*********************************************************************************************************************
 * File : sudoku_displayer.h                                                                                         *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SUDOKU_DISPLAYER_H
#define SUDOKU_DISPLAYER_H

#include <functional>

#include "cell.h"
#include "square.h"

/// @brief Class displaying sudoku grids, whether they are solved or in progress
class SudokuDisplayer
{
public:
    SudokuDisplayer() = default;

    /// @brief Displays in the console the grid wih the values that have been found
    /// @param cells 9x9 grid of cells
    void found_cells(const std::array<short, 81> &cells);

    /// @brief Displays in the console the grid wih the values that have been found
    /// @param cells 9x9 grid of cells
    void found_cells(const std::array<Cell, 81> &cells);

    /// @brief Displays in the console the grid wih the values that have been found
    /// @param cells 9x9 grid of cells
    void found_cells(const std::vector<ValKey> &cells);

    void candidates_per_cell(const std::array<Cell, 81> &cells);

    void num_candidates_per_cell(const std::array<Cell, 81> &cells);

    void candidate_cells_containing_val(short val, const std::array<Cell, 81> &cells);

    void num_candidates_per_cell_containing_val(short val, const std::array<Cell, 81> &cells);

private:
    /// @brief Callback to append a character to a stream from a location (i,j)
    using CellToCharCb = std::function<void(std::stringstream &, int, int)>;

    void print_grid(CellToCharCb char_cb, bool use_long_line = false) const;

    const char *horizontal_line_str = " _______";
    const char *long_horizontal_line_str = " _____________________________________";
};

#endif // SUDOKU_DISPLAYER_H