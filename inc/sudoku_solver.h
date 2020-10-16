/*********************************************************************************************************************
 * File : sudoku_solver.h                                                                                            *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "coloring.h"
#include "new_matrix.h"
#include "sudoku_displayer.h"

class SudokuSolver
{
public:
    /// @brief Constructor
    SudokuSolver() = default;

    void solve_grid(const std::string &input_filename_csv);

    void export_to_csv() const;

private:
    std::string output_filename_csv_;

    NewMatrix matrix_;
    SudokuDisplayer displayer_;

    Coloring coloring_;
    std::vector<int> cells_to_add_coloring_;

    std::vector<ValKey> cells_tmp_;
    std::vector<ValKey> cells_to_lock_;
    std::vector<ValKey> cells_to_add_;
};

#endif // SUDOKU_SOLVER_H