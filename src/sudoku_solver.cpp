/*********************************************************************************************************************
 * File : sudoku_solver.cpp                                                                                          *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "back_tracking_solver.h"
#include "sudoku_solver.h"

namespace bfs = boost::filesystem;

SudokuSolver::SudokuSolver(const std::string &output_dir_path) : output_dir_path_(output_dir_path)
{
}

bool SudokuSolver::solve_grid(const std::string &input_filename_csv, bool display, bool use_backtracking_only)
{

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
    if (use_backtracking_only)
    {
        BackTrackingSolver backtracking;
        if (!backtracking.solve(init_cells))
            return false;

        export_to_csv(backtracking.get_cells());
        if (display)
            displayer_.found_cells(backtracking.get_cells());
        return true;
    }

    matrix_.reset();
    for (auto it = init_cells.cbegin(); it != init_cells.end(); it++)
    {
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
        }
    }

    BackTrackingSolver backtracking;
    if (!backtracking.solve(matrix_.get_cells()))
        return false;

    export_to_csv(backtracking.get_cells());
    if (display)
        displayer_.found_cells(backtracking.get_cells());
    return true;
}

void SudokuSolver::export_to_csv(const std::array<short, 81> &solution) const
{
    static int count = 0;
    std::stringstream ss;
    ss << "solved_sudoku_" << count++ << ".csv";
    const std::string output_filename_csv = (bfs::path(output_dir_path_) / ss.str()).string();

    // Value + 1 because the value is shifted to [0;8]
    std::ofstream file(output_filename_csv);
    int key = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 8; j++)
            file << solution[key++] + 1 << ";";
        file << solution[key++] + 1 << std::endl;
    }
    file.close();

    std::cout << "Sudoku has been solved and saved to " << output_filename_csv << std::endl;
}
