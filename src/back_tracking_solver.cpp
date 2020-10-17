/*********************************************************************************************************************
 * File : backtracking_solver.cpp                                                                                    *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <iostream>

#include "back_tracking_solver.h"

const std::array<short, 81> &BackTrackingSolver::get_cells() const
{
    return cells_;
}

bool BackTrackingSolver::solve(const std::vector<ValKey> &set_cells)
{
    cells_.fill(-1);
    for (const auto &vk : set_cells)
        cells_[vk.key] = vk.val;

    return find_solution(0);
}

bool BackTrackingSolver::solve(const std::array<NewCell, 81> &cells_with_candidates)
{
    cells_.fill(-1);
    return find_solution(cells_with_candidates, 0);
}

bool BackTrackingSolver::find_solution(int key_to_test)
{
    if (key_to_test == 81)
        return true;

    if (cells_[key_to_test] != -1)
        return find_solution(key_to_test + 1);

    for (int val = 0; val < 9; val++)
    {
        if (is_value_valid(val, key_to_test))
        {
            cells_[key_to_test] = val;
            if (find_solution(key_to_test + 1))
                return true;
            cells_[key_to_test] = -1;
        }
    }
    return false;
}

bool BackTrackingSolver::find_solution(const std::array<NewCell, 81> &cells_with_candidates,
                                       int key_to_test)
{
    if (key_to_test == 81)
        return true;

    const auto &candidates = cells_with_candidates[key_to_test].get_candidates();
    for (const int val : candidates)
    {
        if (is_value_valid(val, key_to_test))
        {
            cells_[key_to_test] = val;
            if (find_solution(cells_with_candidates, key_to_test + 1))
                return true;
            cells_[key_to_test] = -1;
        }
    }
    return false;
}

bool BackTrackingSolver::is_value_valid(short val, int key)
{
    const int i_row = key / 9;
    const int j_col = key - 9 * i_row;

    // Row
    const int i0 = 9 * i_row;
    for (int j = 0; j < 9; j++)
        if (cells_[i0 + j] == val)
            return false;

    // Column
    for (int i = 0; i < 9; i++)
        if (cells_[9 * i + j_col] == val)
            return false;

    // Square
    const int i_block = i_row / 3;
    const int j_block = j_col / 3;
    const int i_block_3 = 3 * i_block;
    const int j_block_3 = 3 * j_block;

    for (int i = i_block_3; i < i_block_3 + 3; i++)
        for (int j = j_block_3; j < j_block_3 + 3; j++)
            if (cells_[9 * i + j] == val)
                return false;

    return true;
}
