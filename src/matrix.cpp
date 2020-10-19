/*********************************************************************************************************************
 * File : matrix.cpp                                                                                                 *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "matrix.h"

void Matrix::reset()
{
    for (int k = 0; k < 81; k++)
        cells_[k].reset();

    for (int k = 0; k < 9; k++)
    {
        rows_[k].reset();
        cols_[k].reset();
        squares_[k].reset();
    }
    row_fish_.reset();
    col_fish_.reset();
}

const std::array<Cell, 81> &Matrix::get_cells() const
{
    return cells_;
}

void Matrix::set_value(short val, int key,
                       std::vector<ValKey> &cells_to_lock,
                       std::vector<ValKey> &cells_to_add)
{
    assert(0 <= val && val < 9);
    assert(0 <= key && key < 81);
    if (cells_[key].is_set())
        return;

    const int i_row = key / 9;
    const int j_col = key - 9 * i_row;
    const int i_block = i_row / 3;
    const int j_block = j_col / 3;
    const int i_block_3 = 3 * i_block;
    const int j_block_3 = 3 * j_block;

    // Remaining candidate values in the cell
    cells_[key].get_remaining_candidates(val, remaining_digits_tmp_);
    for (const auto v : remaining_digits_tmp_)
        add_constraint(v, key, cells_to_lock, cells_to_add);

    // Add constraint on the row
    for (int j = 0; j < 9; j++)
        if (j != j_col)
            add_constraint(val, 9 * i_row + j, cells_to_lock, cells_to_add);

    // Add constraint on the column
    for (int i = 0; i < 9; i++)
        if (i != i_row)
            add_constraint(val, 9 * i + j_col, cells_to_lock, cells_to_add);

    // Add constraint on the square
    for (int i = i_block_3; i < i_block_3 + 3; i++)
        for (int j = j_block_3; j < j_block_3 + 3; j++)
            if (i != i_row || j != j_col)
                add_constraint(val, 9 * i + j, cells_to_lock, cells_to_add);
}

bool Matrix::add_constraint(short val_restrict, int key,
                            std::vector<ValKey> &cells_to_lock,
                            std::vector<ValKey> &cells_to_add)
{
    assert(0 <= val_restrict && val_restrict < 9);
    assert(0 <= key && key < 81);

    static std::unordered_set<int> new_keys_val_restrict; // Keys added during this method for val_restrict by the row, the column
                                                          // or the square. This prevents returning several time the same Cell.
                                                          // N.B. the cell can add only a value different than val_restrict
    new_keys_val_restrict.clear();

    // Cell
    const auto cell_status = cells_[key].add_constraint(val_restrict);
    switch (cell_status)
    {
    case Cell::Status::ALREADY_KNOWN:
        assert(!rows_[key / 9].get_bitset(val_restrict)[key - 9 * (key / 9)]);
        assert(!cols_[key - 9 * (key / 9)].get_bitset(val_restrict)[key / 9]);
        return false; // No need to keep propagating the constraint

    case Cell::Status::SET_VALUE:
    {
        const int new_val = cells_[key].get_value();
        cells_to_add.emplace_back(new_val, key);
        break;
    }
    case Cell::Status::PAIR:

        break;
    }

    const int i_row = key / 9;
    const int j_col = key - 9 * i_row;
    const int i_block = i_row / 3;
    const int j_block = j_col / 3;
    const int i_block_3 = 3 * i_block;
    const int j_block_3 = 3 * j_block;

    // Row
    int output_j;
    auto &row = rows_[i_row];
    const auto row_status = row.add_constraint(val_restrict, j_col, output_j);
    switch (row_status)
    {
    case Line::Status::SET_VALUE:
        // Set the value in the cell
        new_keys_val_restrict.insert(9 * i_row + output_j);
        break;

    case Line::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_j_lock_3 = 3 * output_j;
            for (int i = i_block_3; i < i_block_3 + 3; i++)
                if (i != i_row)
                    for (int j = 3 * out_j_lock_3; j < out_j_lock_3 + 3; j++)
                        cells_to_lock.emplace_back(val_restrict, 9 * i + j);
        }
        break;
    }
    if (row.has_only_two_possibilities(val_restrict))
        try_and_find_fish_on_rows(val_restrict, i_row, cells_to_lock);

    // Column
    int output_i;
    auto &col = cols_[j_col];
    const auto col_status = col.add_constraint(val_restrict, i_row, output_i);
    switch (col_status)
    {
    case Line::Status::SET_VALUE:
        // Set the value in the cell
        new_keys_val_restrict.insert(9 * output_i + j_col);
        break;

    case Line::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_i_lock_3 = 3 * output_i;
            for (int i = out_i_lock_3; i < out_i_lock_3 + 3; i++)
                for (int j = j_block_3; j < j_block_3 + 3; j++)
                    if (j != j_col)
                        cells_to_lock.emplace_back(val_restrict, 9 * i + j);
        }
        break;
    }
    if (col.has_only_two_possibilities(val_restrict))
        try_and_find_fish_on_cols(val_restrict, j_col, cells_to_lock);

    // Square
    int output_square_id;
    auto &square = squares_[i_block_3 + j_block];
    const auto square_status = square.add_constraint(val_restrict, i_row, j_col, output_square_id);

    switch (square_status)
    {
    case Square::Status::SET_VALUE:
        // Set the value in the cell
        {
            const int i_inside_square = output_square_id / 3;
            const int j_inside_square = output_square_id - i_inside_square;
            new_keys_val_restrict.insert(9 * (i_block_3 + i_inside_square) + (j_block_3 + j_inside_square));
        }
        break;

    case Square::Status::LOCK_ROW:
        // Prevent other cells in the row to have this value
        {
            const int i0 = 9 * output_square_id;
            for (int j = 0; j < 9; j++)
                if (j / 3 != j_block)
                    cells_to_lock.emplace_back(val_restrict, i0 + j);
        }
        break;

    case Square::Status::LOCK_COL:
        // Prevent other cells in the column to have this value
        {
            for (int i = 0; i < 9; i++)
                if (i / 3 != i_block)
                    cells_to_lock.emplace_back(val_restrict, 9 * i + output_square_id);
        }
        break;
    }

    // Return new cells to add
    for (const int k : new_keys_val_restrict)
        cells_to_add.emplace_back(val_restrict, k);
    return true;
}

void Matrix::try_and_find_fish_on_rows(short val, int i_row, std::vector<ValKey> &cells_to_lock)
{
    assert(rows_[i_row].has_only_two_possibilities(val));

    std::vector<int> j_pair_vec;
    const int i0 = 9 * i_row;
    for (int j = 0; j < 9; j++)
    {
        const int new_key = i0 + j;
        if (cells_[new_key].is_value_possible(val))
            j_pair_vec.emplace_back(j);
    }
    assert(j_pair_vec.size() == 2);
    row_fish_.try_match_pair(val, i_row, j_pair_vec.front(), j_pair_vec.back(), fish_lock_info_tmp_);

    for (const auto &info : fish_lock_info_tmp_)
    {
        const int j_lock = info.perpendicular_i;
        for (int i = 0; i < 9; i++)
        {
            if (i != info.line_i1 && i != info.line_i2)
            {
                const int new_key = 9 * i + j_lock;
                cells_to_lock.emplace_back(val, new_key);
            }
        }
    }
}

void Matrix::try_and_find_fish_on_cols(short val, int j_col, std::vector<ValKey> &cells_to_lock)
{
    assert(cols_[j_col].has_only_two_possibilities(val));

    std::vector<int> i_pair_vec;
    for (int i = 0; i < 9; i++)
    {
        const int new_key = 9 * i + j_col;
        if (cells_[new_key].is_value_possible(val))
            i_pair_vec.emplace_back(new_key);
    }
    assert(i_pair_vec.size() == 2);
    col_fish_.try_match_pair(val, j_col, i_pair_vec.front(), i_pair_vec.back(), fish_lock_info_tmp_);

    for (const auto &info : fish_lock_info_tmp_)
    {
        const int i_lock0 = 9 * info.perpendicular_i;
        for (int j = 0; j < 9; j++)
        {
            if (j != info.line_i1 && j != info.line_i2)
            {
                const int new_key = i_lock0 + j;
                cells_to_lock.emplace_back(val, new_key);
            }
        }
    }
}

void Matrix::do_coloring(short val, std::vector<ValKey> &cells_to_add)
{
    coloring_.do_coloring(val, cells_, rows_, cols_, squares_, cells_to_add);
}
