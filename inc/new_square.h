/*********************************************************************************************************************
 * File : new_square.h                                                                                                *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef NEW_SQUARE_H
#define NEW_SQUARE_H

#include "new_line.h"

class NewSquare
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
    NewSquare() = default;

    /// @brief Returns true if @p val has been set in this square
    /// @param val Digit
    bool is_value_set(short val);

    /// @brief Indicates that a given digit isn't available anymore
    /// @param val Digit that isn't available anymore (0,1...8)
    /// @param i Id of the row on which we're adding the constraint (0,1,..8)
    /// @param j Id of the column on which we're adding the constraint (0,1,..8)
    /// @param output_id_lock Output Id of the only block in this square containing possibilities for @p val
    /// @note Use this id for horizontal blocks when the output status is @ref Status::LOCK_ROW and for vertical
    /// blocks when the output status is @ref Status::LOCK_COL
    ///
    /// It's not possible to have both LOCK_ROW and LOCK_COL, because if we have only one available horizontal
    /// block and only one vertical block, then it means we have only one possibility and get @ref Status::SET_VALUE
    /// @return Status
    Status add_constraint(short val,
                          int i, int j,
                          int &output_id_lock);

    /// @brief Returns true if @p val has only two possibilities along this line
    /// @param val Digit
    bool has_only_two_possibilities(short val) const;

private:
    NewLine horizontal_triplets_;
    NewLine vertical_triplets_;
};

#endif // NEW_SQUARE_H