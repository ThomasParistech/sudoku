/*********************************************************************************************************************
 * File : line.h                                                                                                     *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef LINE_H
#define LINE_H

#include <array>
#include <bitset>
#include <vector>

#include "val_key.h"

/// @brief Class maintaining counts of possibilities for each digit in structure of 3 chunks, each containing 3 cells.
///
/// This can be;
///  - a horizontal row, made of three horizontal pieces of size 3
///  - a vertical column, made of three vertical pieces of size 3
///  - three horizontal pieces of size 3 contained in a square
///  - three vertical pieces of size 3 contained in a square
class Line
{
public:
    enum Status
    {
        NOTHING,
        SET_VALUE, ///< There's only one possible position left for this digit
        LOCK       ///< There's only one of the 3 chunks that can have this digit
                   ///< If this class represents:
                   ///< - a row. Then, the two other horizontal pieces in the square around the valid chunk
                   ///<          can't contain this digit.
                   ///< - a col. Then, the two other vertical pieces in the square around the valid chunk
                   ///<          can't contain this digit.
                   ///< - horiz parts of a square. Then, the two other horizontal pieces in the row containing the
                   ///<          valid chunk can't contain this digit.
                   ///< - vert parts of a square. Then, the two other vertical pieces in the column containing the
                   ///<          valid chunk can't contain this digit.
    };

    /// @brief Constructor
    Line();

    ~Line() = default;

    /// @brief Resets the line to its original state with all the sub-counts set to 3
    void reset();

    /// @brief Returns true if @p val has been set along this line
    /// @param val Digit
    bool is_value_set(short val) const;

    /// @brief Returns true if @p val has only two possibilities along this line
    /// @param val Digit
    bool has_only_two_possibilities(short val) const;

    /// @brief Indicates that a given digit isn't available anymore
    /// @param val_restrict Digit that isn't available anymore (0,1...8)
    /// @param pos Position along the line of the cell on which we're adding the constraint (0,1...8).
    /// @param output_i Output Id depending on the return status
    /// - Status::LOCK, id (0,1,2) is the only block along this line containing possibilities for @p val
    /// - Status::SETVALUE, id (0,1...8) is the position along the line of the cell that must be set to @p val
    /// @return Status
    /// @note If it's Status::SETVALUE, the bitset is automatically reset
    Status add_constraint(short val_restrict, int pos, int &output_i);

    const std::bitset<9> &get_bitset(short val) const;

private:
    std::array<std::bitset<9>, 9> possibilities_per_val_;
    std::bitset<9> set_cells_;
};

#endif // LINE_H