/*********************************************************************************************************************
 * File : sudoku_solver.h                                                                                            *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SUDOKU_SOLVER_H
#define SUDOKU_SOLVER_H

#include "matrix.h"
#include "sudoku_displayer.h"

class SudokuSolver
{
public:
    /// @brief Constructor
    SudokuSolver(const std::string &output_dir_path = "");

    bool solve_grid(const std::string &input_filename_csv,
                    bool display = true, bool use_backtracking_only = false);

private:
    void export_to_csv(const std::array<short, 81> &solution) const;

    std::string output_dir_path_;

    Matrix matrix_;
    SudokuDisplayer displayer_;

    std::vector<ValKey> cells_tmp_;
    std::vector<ValKey> cells_to_lock_;
    std::vector<ValKey> cells_to_add_;
};

#endif // SUDOKU_SOLVER_H