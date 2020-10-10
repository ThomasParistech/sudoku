/*********************************************************************************************************************
 * File : new_matrix.cpp                                                                                             *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "new_matrix.cpp"

NewMatrix::CellToAdd(int key, short val) : key(key), val(val) {}

void NewMatrix::add_constraint(short val, int key)
{
    // Cell
    const auto cell_status = cells_[key].add_constraint(val);
    if (cell_status == NewCell::Status::ALREADY_KNOWN)
        return;

    switch (cell_status)
    {
    case NewCell::Status::SET_VALUE:
        const int new_val = cells_[key].get_value();
        cells_to_add_[new_val].emplace_back(key);
        break;

    case NewCell::Status::PAIR:
        cells_to_pair_.emplace_back(key);
        break;
    }

    const int i_row = key / 9;
    const int j_col = key - 9 * i_row;
    const int i_block = i / 3;
    const int j_block = j / 3;
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

    // Column
    int output_i_lock;
    auto &col = cols[j_col];
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


    // Square
    int output_id_lock;
    auto &square = squares[i_block_3 + j_block];
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

    case NewLine::Status::LOCK_ROW:
        // Prevent other cells in the row to have this value
        {
            const int i0 = 9 * output_id_lock;
            for (int j = 0; j < 9; j++)
                if (j / 3 != j_block)
                    cells_to_lock_at_val.emplace_back(i0 + j);
        }
        break;

    case NewLine::Status::LOCK_COL:
        // Prevent other cells in the column to have this value
        {
            for (int i = 0; i < 9; i++)
                if (i / 3 != i_block)
                    cells_to_lock_at_val.emplace_back(9 * i + output_id_lock);
        }
        break;
    }
}