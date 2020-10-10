/*********************************************************************************************************************
 * File : new_matrix.h                                                                                               *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef NEW_MATRIX_H
#define NEW_MATRIX_H

#include "coloring.h"
#include "fisherman.h"

class NewMatrix
{
public:
    /// @brief Constructor
    NewMatrix() = default;

    void add_constraint(short val, int key);

private:
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

    std::array<NewCell, 81> cells_;
    std::array<NewLine, 9> rows_;
    std::array<NewLine, 9> cols_;
    std::array<NewSquare, 9> squares_;

    Coloring coloring_;

    FisherMan row_fish_;
    FisherMan col_fish_;
    std::vector<FisherMan::LockInfo> fish_lock_info_;

    std::array<std::vector<int>, 9> cells_to_lock_;
    std::array<std::vector<int>, 9> cells_to_add_;
    std::vector<int> cells_to_pair_;
};

#endif // NEW_MATRIX_H