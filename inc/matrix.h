/*********************************************************************************************************************
 * File : matrix.h                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef MATRIX_H
#define MATRIX_H

#include "coloring.h"
#include "fisherman.h"
#include "cell.h"
#include "square.h"

class Matrix
{
public:
    /// @brief Constructor
    Matrix() = default;

    void reset();

    const std::array<Cell, 81> &get_cells() const;

    /// @brief Sets the cell at @p key to the value @p val and propagates constraints
    /// @note cells_to_lock_ is updated accordingly (But cells_to_add_ remains constant)
    /// @param val Value
    /// @param key Cell location = 9*i+j
    /// @param cells_to_lock
    /// @param cells_to_add
    void set_value(short val, int key, std::vector<ValKey> &cells_to_lock,
                   std::vector<ValKey> &cells_to_add);

    /// @brief Indicates that the cell at @p key can't have the value @p val
    /// @note cells_to_lock_ and cells_to_add_ are updated accordingly
    /// @param val_restrict Value
    /// @param key Cell location = 9*i+j
    /// @param cells_to_lock
    /// @param cells_to_add
    /// @return false if the value constraint is already known
    bool add_constraint(short val_restrict, int key, std::vector<ValKey> &cells_to_lock,
                        std::vector<ValKey> &cells_to_add);

    void do_coloring(short val, std::vector<ValKey> &cells_to_add);

private:
    /// @brief Finds pair of cells along the row that both contains @p val and uses it to find a fish pattern
    ///
    /// If a fish pattern is found, the attribute cells_to_lock_ will be updated
    /// @param val Value
    /// @param i_row Indice of the row containing the pair of @p val
    /// @param cells_to_lock
    void try_and_find_fish_on_rows(short val, int i_row, std::vector<ValKey> &cells_to_lock);

    /// @brief Finds pair of cells along the column that both contains @p val and uses it to find a fish pattern
    ///
    /// If a fish pattern is found, the attribute cells_to_lock_ will be updated
    /// @param val Value
    /// @param j_col Indice of the column containing the pair of @p val
    /// @param cells_to_lock
    void try_and_find_fish_on_cols(short val, int j_col, std::vector<ValKey> &cells_to_lock);

    std::array<Cell, 81> cells_;
    std::array<Line, 9> rows_;
    std::array<Line, 9> cols_;
    std::array<Square, 9> squares_;
    std::vector<short> remaining_digits_tmp_;

    FisherMan row_fish_;
    FisherMan col_fish_;
    std::vector<FisherMan::LockInfo> fish_lock_info_tmp_;

    Coloring coloring_;
};

#endif // MATRIX_H