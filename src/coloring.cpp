/*********************************************************************************************************************
 * File : coloring.cpp                                                                                               *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "coloring.h"

void Coloring::do_coloring(short val, const std::array<NewCell, 81> &cells, const std::array<NewLine, 9> &rows,
                           const std::array<NewLine, 9> &columns, const std::array<NewSquare, 9> &squares,
                           std::vector<ValKey> &output_cells_to_add)
{
    extract_all_pairs(val, cells, rows, columns, squares);
    colored_cells_.reset();
    output_cells_to_add.clear();
    for (int i = 0; i < 9; i++)
    {
        const int i0 = 9 * i;
        for (int j = 0; j < 9; j++)
        {
            const int key = i0 + j;
            const auto &pairs = existing_pairs_[key];
            if (pairs.empty()) // Isn't paired with other cells
                continue;
            if (colored_cells_[key]) // Has already been colored
                continue;

            valid_checker_0_.reset();
            valid_checker_1_.reset();

            if (!spread_coloring_path(key, true))
            {
                // One of the two paths isn't valid
                if (valid_checker_0_.is_valid)
                    output_cells_to_add.emplace_back(val, key);
                else
                    output_cells_to_add.emplace_back(val, pairs.back()); // One is enough, it will then propagate the info
            }
        }
    }
}

bool Coloring::spread_coloring_path(int key, bool is_base_color)
{
    if (colored_cells_[key]) // Has already been colored
        return true;

    colored_cells_.set(key); // color the cell

    // Check if the cell is valid
    auto &checker = (is_base_color ? valid_checker_0_ : valid_checker_1_);
    if (!checker.is_location_valid(key))
        return false;

    const auto &pairs = existing_pairs_[key];
    if (pairs.empty())
        return true;

    for (const int k : pairs)
        if (!spread_coloring_path(k, !is_base_color))
            return false;
    return true;
}

void Coloring::extract_all_pairs(short val, const std::array<NewCell, 81> &cells, const std::array<NewLine, 9> &rows,
                                 const std::array<NewLine, 9> &columns, const std::array<NewSquare, 9> &squares)
{

    // Clear pairs
    for (auto &vec_cell : existing_pairs_)
        vec_cell.clear();

    // Lambda used to add a pair
    std::vector<int> pair_keys;
    const auto try_add_pair = [&](int key) {
        if (cells[key].is_value_possible(val))
        {
            pair_keys.emplace_back(key);
            if (pair_keys.size() == 2)
            {
                // Add pair
                existing_pairs_[pair_keys.front()].emplace_back(pair_keys.back());
                existing_pairs_[pair_keys.back()].emplace_back(pair_keys.front());
                return true;
            }
        }
        return false;
    };

    // Extract pairs on rows
    for (int i = 0; i < 9; i++)
    {
        if (!rows[i].has_only_two_possibilities(val))
            continue;

        const int i0 = 9 * i;
        pair_keys.clear();
        for (int j = 0; j < 9; j++)
        {
            const int key = i0 + j;
            if (try_add_pair(key))
                break;
        }
    }

    // Extract pairs on columns
    for (int j = 0; j < 9; j++)
    {
        if (!columns[j].has_only_two_possibilities(val))
            continue;

        pair_keys.clear();
        for (int i = 0; i < 9; i++)
        {
            const int key = 9 * i + j;
            if (try_add_pair(key))
                break;
        }
    }

    // Extract pairs on squares
    for (int i_square = 0; i_square < 3; i_square++)
    {
        const int i_square_3 = 3 * i_square;
        for (int j_square = 0; j_square < 3; j_square++)
        {
            if (!squares[i_square_3 + j_square].has_only_two_possibilities(val))
                continue;

            pair_keys.clear();
            const int j_square_3 = 3 * j_square;
            for (int i = i_square_3; i < i_square_3 + 3; i++)
            {
                const int i0 = 9 * i;
                for (int j = j_square_3; j < j_square + 3; j++)
                {
                    const int key = i0 + j;
                    if (try_add_pair(key))
                        break;
                }
            }
        }
    }
}

void Coloring::ValidityChecker::reset()
{
    history_rows.clear();
    history_cols.clear();
    history_squares.clear();
    is_valid = true;
}

bool Coloring::ValidityChecker::is_location_valid(int k)
{
    const int i = k / 9;
    const int j = k - 9 * i;
    const auto it_row = history_rows.find(i);
    if (it_row != history_rows.end())
    {
        is_valid = false;
        return false;
    }

    const auto it_col = history_cols.find(j);
    if (it_col != history_cols.end())
    {
        is_valid = false;
        return false;
    }

    const int square_id = 3 * (i / 3) + j / 3;
    const auto it_square = history_squares.find(square_id);
    if (it_square != history_squares.end())
    {
        is_valid = false;
        return false;
    }

    history_rows.insert(i);
    history_cols.insert(j);
    history_squares.insert(square_id);
    return true;
}