/*********************************************************************************************************************
 * File : new_matrix.h                                                                                               *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef NEW_MATRIX_H
#define NEW_MATRIX_H

#include "coloring.h"
#include "fisherman.h"
#include "sudoku_displayer.h"

class NewMatrix
{
public:
    /// @brief Constructor
    NewMatrix() = default;

    void solve_grid(const std::string &input_filename_csv);

private:
    struct ValKey
    {
        ValKey(short val, int key);
        short val;
        int key;
    };

    /// @brief Indicates that the cell at @p key can't have the value @p val
    /// @note cells_to_lock_tmp_ and cells_to_add_ are updated accordingly
    /// @param val_restrict Value
    /// @param key Cell location = 9*i+j
    /// @return false if the value constraint is already known
    bool add_constraint(short val_restrict, int key);

    /// @brief Sets the cell at @p key to the value @p val and propagates constraints
    /// @note cells_to_lock_tmp_ is updated accordingly (But cells_to_add_ remains constant)
    /// @param val Value
    /// @param key Cell location = 9*i+j
    void set_value(short val, int key);

    /// @brief Finds pair of cells along the row that both contains @p val and uses it to find a fish pattern
    ///
    /// If a fish pattern is found, the attribute cells_to_lock_ will be updated
    /// @param val Value
    /// @param i_row Indice of the row containing the pair of @p val
    void try_and_find_fish_on_rows(short val, int i_row);

    /// @brief Finds pair of cells along the column that both contains @p val and uses it to find a fish pattern
    ///
    /// If a fish pattern is found, the attribute cells_to_lock_ will be updated
    /// @param val Value
    /// @param j_col Indice of the column containing the pair of @p val
    void try_and_find_fish_on_cols(short val, int j_col);

    void export_to_csv() const;

    void reset();

    std::string output_filename_csv_;

    std::array<NewCell, 81> cells_;
    std::array<NewLine, 9> rows_;
    std::array<NewLine, 9> cols_;
    std::array<NewSquare, 9> squares_;
    std::vector<short> remaining_digits_tmp_;

    Coloring coloring_;
    std::vector<int> cells_to_add_coloring_;

    FisherMan row_fish_;
    FisherMan col_fish_;
    std::vector<FisherMan::LockInfo> fish_lock_info_tmp_;

    std::vector<ValKey> cells_to_lock_;
    std::vector<ValKey> cells_to_lock_tmp_;
    std::vector<ValKey> cells_to_add_;

    SudokuDisplayer displayer_;
};

#endif // NEW_MATRIX_H