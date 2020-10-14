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

    void num_candidates_per_cell(const std::array<NewCell, 81> &cells);

    void num_candidates_per_cell_containing_val(short val, const std::array<NewCell, 81> &cells);

    void counts_on_rows(short val, const std::array<NewLine, 9> &rows);

    void counts_on_cols(short val, const std::array<NewLine, 9> &cols);

private:
    /// @brief Callback to append a character to a stream from a location (i,j)
    using CellToCharCb = std::function<void(std::stringstream &, int, int)>;

    void print_grid(CellToCharCb char_cb) const;

    const char *horizontal_line_str = " _______ _______ _______ ";
};

#endif // SUDOKU_DISPLAYER_H