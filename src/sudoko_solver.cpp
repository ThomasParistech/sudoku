/*********************************************************************************************************************
 * File : sudoku_solver.cpp                                                                                          *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>

#include "sudoku_solver.h"

void SudokuSolver::solve_grid(const std::string &input_filename_csv)
{
    matrix_.reset();

    // Load
    std::vector<ValKey> init_cells;
    std::ifstream file(input_filename_csv);
    std::string line;
    for (int i = 0; i < 9; i++)
    {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string field;
        for (int j = 0; j < 9; j++)
        {
            std::getline(ss, field, ';');
            std::stringstream fs(field);
            int val = 0;
            fs >> val;
            if (val != 0)
                init_cells.emplace_back(val - 1, 9 * i + j); // val-1 because the value is shifted to 0:8
        }
    }

    // Solve
    for (auto it = init_cells.cbegin(); it != init_cells.end(); it++)
    {
        // displayer_.candidate_cells_containing_val(2, matrix_.get_cells());
        matrix_.set_value(it->val, it->key, cells_to_lock_, cells_to_add_);

        while (!cells_to_lock_.empty() || !cells_to_add_.empty())
        {
            // Add new values
            while (!cells_to_add_.empty())
            {
                cells_tmp_.clear();
                std::move(cells_to_add_.begin(), cells_to_add_.end(), std::back_inserter(cells_tmp_));
                cells_to_add_.clear();
                for (const auto &cell : cells_tmp_)
                    matrix_.set_value(cell.val, cell.key, cells_to_lock_, cells_to_add_);
            }

            // Propagate constraints
            while (!cells_to_lock_.empty())
            {
                // Recover the new cells to lock
                cells_tmp_.clear();
                std::move(cells_to_lock_.begin(), cells_to_lock_.end(), std::back_inserter(cells_tmp_));
                cells_to_lock_.clear();
                // Add these constraints (which might add new elements to cells_to_lock_)
                for (auto it = cells_tmp_.cbegin(); it != cells_tmp_.cend(); it++)
                    matrix_.add_constraint(it->val, it->key, cells_to_lock_, cells_to_add_);
                cells_to_lock_.clear();
            }

            // // Do Coloring
            // for (int v = 0; v < 9; v++)
            // {
            //     coloring_.do_coloring(v, matrix_.get_cells(), matrix_.get_rows(), matrix_.get_cols(), matrix_.get_squares(), cells_to_add_coloring_);
            //     for (const auto &k : cells_to_add_coloring_)
            //         matrix_.set_value(v, k, cells_to_lock_, cells_to_add_);
            // }
        }
    }
    // displayer_.num_candidates_per_cell(matrix_.get_cells());
    // displayer_.found_cells(matrix_.get_cells());
    // for (int k = 0; k < 9; k++)
    //     displayer_.candidate_cells_containing_val(k, matrix_.get_cells());

    displayer_.candidates_per_cell(matrix_.get_cells());
    // export_to_csv();
}

void SudokuSolver::export_to_csv() const
{
    const auto cells_ = matrix_.get_cells();
    // Value + 1 because the value is shifted to [0;8]
    std::ofstream file(output_filename_csv_);
    int key = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 8; j++)
            file << cells_[key++].get_value() + 1 << ";";
        file << cells_[key++].get_value() + 1 << std::endl;
    }
    file.close();
}