/*********************************************************************************************************************
 * File : square.h                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef SQUARE_H
#define SQUARE_H

#include "line.h"

/// @brief Class maintaining counts of possibilities for each digit in a square
///
/// The square is divide inoto two @ref Line instances:
///  - three horizontal pieces of size 3
///  - three vertical pieces of size 3
class Square
{
public:
    enum Status
    {
        NOTHING,
        SET_VALUE,
        LOCK_ROW,
        LOCK_COL
    };

    /// @brief Constructor
    Square() = default;

    /// @brief Resets the square to its original state with all the sub-counts set to 3
    void reset();

    /// @brief Returns true if @p val has been set in this square
    /// @param val Digit
    bool is_value_set(short val);

    /// @brief Indicates that a given digit isn't available anymore
    /// @param val_restrict Digit that isn't available anymore (0,1...8)
    /// @param i Id of the row on which we're adding the constraint (0,1,..8)
    /// @param j Id of the column on which we're adding the constraint (0,1,..8)
    /// @param output_i Output Id depending on the return status
    /// - Status::LOCK_ROW, id (0,1,2) is the only horiz block along in the square containing possibilities for @p val
    /// - Status::LOCK_COL, id (0,1,2) is the only vert block along in the square containing possibilities for @p val
    /// - Status::SETVALUE, id (0,1...8) is the position in the square of the cell that must be set to @p val
    /// @return Status
    /// @note If it's Status::SETVALUE, the horizontal and vertical bitsets are automatically reset
    Status add_constraint(short val_restrict, int i, int j, int &output_i);

    /// @brief Returns true if @p val has only two possibilities along this line
    /// @param val Digit
    bool has_only_two_possibilities(short val) const;

private:
    Line horizontal_triplets_;
    Line vertical_triplets_;
};

#endif // SQUARE_H