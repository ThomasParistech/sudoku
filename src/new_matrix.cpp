/*********************************************************************************************************************
 * File : new_matrix.cpp                                                                                             *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "new_matrix.h"

void NewMatrix::add_constraint(short val, int key)
{
    // Cell
    const auto cell_status = cells_[key].add_constraint(val);
    switch (cell_status)
    {
    case NewCell::Status::ALREADY_KNOWN:
        return; // No need to keep propagating the constraint

    case NewCell::Status::SET_VALUE:
    {
        const int new_val = cells_[key].get_value();
        cells_to_add_[new_val].emplace_back(key);
        break;
    }
    case NewCell::Status::PAIR:
        cells_to_pair_.emplace_back(key);
        break;
    }

    const int i_row = key / 9;
    const int j_col = key - 9 * i_row;
    const int i_block = i_row / 3;
    const int j_block = j_col / 3;
    const int i_block_3 = 3 * i_block;
    const int j_block_3 = 3 * j_block;

    auto &cells_to_lock_at_val = cells_to_lock_[val];

    // Row
    int output_j_lock;
    auto &row = rows_[i_row];
    const auto row_status = row.add_constraint(val, i_block, output_j_lock);
    switch (row_status)
    {
    case NewLine::Status::SET_VALUE:
        // Set the value in the cell
        {
            const int i0 = 9 * i_row;
            for (int j = 0; j < 9; j++)
            {
                const int new_key = i0 + j;
                if (cells_[new_key].is_value_possible(val))
                {
                    cells_to_add_[val].emplace_back(new_key);
                    break;
                }
            }
        }
        break;

    case NewLine::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_j_lock_3 = 3 * output_j_lock;
            for (int i = i_block_3; i < i_block_3 + 3; i++)
                if (i != i_row)
                    for (int j = 3 * out_j_lock_3; j < out_j_lock_3 + 3; j++)
                        cells_to_lock_at_val.emplace_back(9 * i + j);
        }
        break;
    }
    if (row.has_only_two_possibilities(val))
        try_and_find_fish_on_rows(val, i_row);

    // Column
    int output_i_lock;
    auto &col = cols_[j_col];
    const auto col_status = col.add_constraint(val, j_block, output_i_lock);
    switch (col_status)
    {
    case NewLine::Status::SET_VALUE:
        // Set the value in the cell
        {
            for (int i = 0; i < 9; i++)
            {
                const int new_key = 9 * i + j_col;
                if (cells_[new_key].is_value_possible(val))
                {
                    cells_to_add_[val].emplace_back(new_key);
                    break;
                }
            }
        }
        break;

    case NewLine::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_i_lock_3 = 3 * output_i_lock;
            for (int i = out_i_lock_3; i < out_i_lock_3 + 3; i++)
                for (int j = j_block_3; j < j_block_3 + 3; j++)
                    if (j != j_col)
                        cells_to_lock_at_val.emplace_back(9 * i + j);
        }
        break;
    }
    if (col.has_only_two_possibilities(val))
        try_and_find_fish_on_cols(val, j_col);

    // Square
    int output_id_lock;
    auto &square = squares_[i_block_3 + j_block];
    const auto square_status = square.add_constraint(val, i_row, j_col, output_id_lock);

    switch (square_status)
    {
    case NewSquare::Status::SET_VALUE:
        // Set the value in the cell
        {
            for (int i = i_block_3; i < i_block_3 + 3; i++)
            {
                for (int j = j_block_3; j < j_block_3 + 3; j++)
                {
                    const int new_key = 9 * i + j;
                    if (cells_[new_key].is_value_possible(val))
                        cells_to_add_[val].emplace_back(new_key);
                    break;
                }
            }
        }
        break;

    case NewSquare::Status::LOCK_ROW:
        // Prevent other cells in the row to have this value
        {
            const int i0 = 9 * output_id_lock;
            for (int j = 0; j < 9; j++)
                if (j / 3 != j_block)
                    cells_to_lock_at_val.emplace_back(i0 + j);
        }
        break;

    case NewSquare::Status::LOCK_COL:
        // Prevent other cells in the column to have this value
        {
            for (int i = 0; i < 9; i++)
                if (i / 3 != i_block)
                    cells_to_lock_at_val.emplace_back(9 * i + output_id_lock);
        }
        break;
    }
}

void NewMatrix::try_and_find_fish_on_rows(short val, int i_row)
{
    assert(!rows_[i_row].has_only_two_possibilities(val));
    auto &cells_to_lock_at_val = cells_to_lock_[val];

    std::vector<int> j_pair_vec;
    const int i0 = 9 * i_row;
    for (int j = 0; j < 9; j++)
    {
        const int new_key = i0 + j;
        if (cells_[new_key].is_value_possible(val))
            j_pair_vec.emplace_back(new_key);
    }
    assert(j_pair_vec.size() == 2);
    row_fish_.try_match_pair(val, i_row, j_pair_vec.front(), j_pair_vec.back(), fish_lock_info_);

    for (const auto &info : fish_lock_info_)
    {
        const int j_lock = info.perpendicular_i;
        for (int i = 0; i < 9; i++)
        {
            if (i != info.line_i1 && i != info.line_i2)
            {
                const int new_key = 9 * i + j_lock;
                cells_to_lock_at_val.emplace_back(new_key);
            }
        }
    }
}

void NewMatrix::try_and_find_fish_on_cols(short val, int j_col)
{
    assert(!cols_[j_col].has_only_two_possibilities(val));
    auto &cells_to_lock_at_val = cells_to_lock_[val];

    std::vector<int> i_pair_vec;
    for (int i = 0; i < 9; i++)
    {
        const int new_key = 9 * i + j_col;
        if (cells_[new_key].is_value_possible(val))
            i_pair_vec.emplace_back(new_key);
    }
    assert(i_pair_vec.size() == 2);
    col_fish_.try_match_pair(val, j_col, i_pair_vec.front(), i_pair_vec.back(), fish_lock_info_);

    for (const auto &info : fish_lock_info_)
    {
        const int i_lock0 = 9 * info.perpendicular_i;
        for (int j = 0; j < 9; j++)
        {
            if (j != info.line_i1 && j != info.line_i2)
            {
                const int new_key = i_lock0 + j;
                cells_to_lock_at_val.emplace_back(new_key);
            }
        }
    }
}
