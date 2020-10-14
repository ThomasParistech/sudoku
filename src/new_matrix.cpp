/*********************************************************************************************************************
 * File : new_matrix.cpp                                                                                             *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>
#include <fstream>
#include <sstream>

#include <iostream> // debug

#include "new_matrix.h"

NewMatrix::ValKey::ValKey(short val, int key) : val(val), key(key) {}

void NewMatrix::solve_grid(const std::string &input_filename_csv)
{
    reset();

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
        displayer_.counts_on_rows(6, rows_);
        //displayer_.found_cells(cells_);
        set_value(it->val, it->key);

        // Add new values
        for (const auto &cell : cells_to_add_)
            set_value(cell.val, cell.key);
        cells_to_add_.clear();

        // Propagate constraints
        while (!cells_to_lock_tmp_.empty())
        {
            // Recover the new cells to lock
            std::move(cells_to_lock_tmp_.begin(), cells_to_lock_tmp_.end(), std::back_inserter(cells_to_lock_));
            cells_to_lock_tmp_.clear();
            // Add these constraints (which might add new elements to cells_to_lock_tmp_)
            for (auto it = cells_to_lock_.cbegin(); it != cells_to_lock_.cend(); it++)
                add_constraint(it->val, it->key);
            cells_to_lock_.clear();
        }

        // // Do Coloring
        // for (int v = 0; v < 9; v++)
        // {
        //     coloring_.do_coloring(v, cells_, rows_, cols_, squares_, cells_to_add_coloring_);
        //     for (const auto &k : cells_to_add_coloring_)
        //         set_value(v, k);
        // }
    }
    // displayer_.num_candidates_per_cell(cells_);
    // displayer_.found_cells(cells_);

    // export_to_csv();
}

void NewMatrix::export_to_csv() const
{
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

void NewMatrix::reset()
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
    cells_to_lock_.clear();
    cells_to_lock_tmp_.clear();
    cells_to_add_.clear();
}

void NewMatrix::set_value(short val, int key)
{
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
        add_constraint(v, key);

    // Add constraint on the row
    for (int j = 0; j < 9; j++)
        if (j != j_col)
            add_constraint(val, 9 * i_row + j);

    // Add constraint on the column
    for (int i = 0; i < 9; i++)
        if (i != i_row)
            add_constraint(val, 9 * i + j_col);

    // Add constraint on the square
    for (int i = i_block_3; i < i_block_3 + 3; i++)
        for (int j = j_block_3; j < j_block_3 + 3; j++)
            if (i != i_row || j != j_col)
                add_constraint(val, 9 * i + j);
}

bool NewMatrix::add_constraint(short val_restrict, int key)
{
    static std::unordered_set<int> new_keys_val_restrict; // Keys added during this method for val_restrict by the row, the column
                                                          // or the square. This prevents returning several time the same Cell.
                                                          // N.B. the cell can add only a value different than val_restrict
    new_keys_val_restrict.clear();

    // Cell
    const auto cell_status = cells_[key].add_constraint(val_restrict);
    switch (cell_status)
    {
    case NewCell::Status::ALREADY_KNOWN:
        return false; // No need to keep propagating the constraint

    case NewCell::Status::SET_VALUE:
    {
        const int new_val = cells_[key].get_value();
        cells_to_add_.emplace_back(new_val, key);
        break;
    }
    case NewCell::Status::PAIR:
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
    case NewLine::Status::SET_VALUE:
        // Set the value in the cell
        new_keys_val_restrict.insert(9 * i_row + output_j);
        break;

    case NewLine::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_j_lock_3 = 3 * output_j;
            for (int i = i_block_3; i < i_block_3 + 3; i++)
                if (i != i_row)
                    for (int j = 3 * out_j_lock_3; j < out_j_lock_3 + 3; j++)
                        cells_to_lock_tmp_.emplace_back(val_restrict, 9 * i + j);
        }
        break;
    }
    // if (row.has_only_two_possibilities(val_restrict))
    //     try_and_find_fish_on_rows(val_restrict, i_row);

    // Column
    int output_i;
    auto &col = cols_[j_col];
    const auto col_status = col.add_constraint(val_restrict, i_row, output_i);
    switch (col_status)
    {
    case NewLine::Status::SET_VALUE:
        // Set the value in the cell
        new_keys_val_restrict.insert(9 * output_i + j_col);
        break;

    case NewLine::Status::LOCK:
        // Prevent other cells in the square to have this value
        {
            const int out_i_lock_3 = 3 * output_i;
            for (int i = out_i_lock_3; i < out_i_lock_3 + 3; i++)
                for (int j = j_block_3; j < j_block_3 + 3; j++)
                    if (j != j_col)
                        cells_to_lock_tmp_.emplace_back(val_restrict, 9 * i + j);
        }
        break;
    }
    // if (col.has_only_two_possibilities(val_restrict))
    //     try_and_find_fish_on_cols(val_restrict, j_col);

    // Square
    int output_square_id;
    auto &square = squares_[i_block_3 + j_block];
    const auto square_status = square.add_constraint(val_restrict, i_row, j_col, output_square_id);

    switch (square_status)
    {
    case NewSquare::Status::SET_VALUE:
        // Set the value in the cell
        {
            const int i_inside_square = output_square_id / 3;
            const int j_inside_square = output_square_id - i_inside_square;
            new_keys_val_restrict.insert(9 * (i_block_3 + i_inside_square) + (j_block_3 + j_inside_square));
        }
        break;

    case NewSquare::Status::LOCK_ROW:
        // Prevent other cells in the row to have this value
        {
            const int i0 = 9 * output_square_id;
            for (int j = 0; j < 9; j++)
                if (j / 3 != j_block)
                    cells_to_lock_tmp_.emplace_back(val_restrict, i0 + j);
        }
        break;

    case NewSquare::Status::LOCK_COL:
        // Prevent other cells in the column to have this value
        {
            for (int i = 0; i < 9; i++)
                if (i / 3 != i_block)
                    cells_to_lock_tmp_.emplace_back(val_restrict, 9 * i + output_square_id);
        }
        break;
    }

    // Return new cells to add
    for (const int k : new_keys_val_restrict)
        cells_to_add_.emplace_back(val_restrict, k);
    return true;
}

void NewMatrix::try_and_find_fish_on_rows(short val, int i_row)
{
    assert(!rows_[i_row].has_only_two_possibilities(val));

    std::vector<int> j_pair_vec;
    const int i0 = 9 * i_row;
    for (int j = 0; j < 9; j++)
    {
        const int new_key = i0 + j;
        if (cells_[new_key].is_value_possible(val))
            j_pair_vec.emplace_back(new_key);
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
                cells_to_lock_tmp_.emplace_back(val, new_key);
            }
        }
    }
}

void NewMatrix::try_and_find_fish_on_cols(short val, int j_col)
{
    assert(!cols_[j_col].has_only_two_possibilities(val));

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
                cells_to_lock_tmp_.emplace_back(val, new_key);
            }
        }
    }
}
