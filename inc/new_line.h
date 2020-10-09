/*********************************************************************************************************************
 * File : line.h                                                                                                     *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef NEW_LINE_H
#define NEW_LINE_H

#include <array>
#include <unordered_map>

/// @brief Class maintaining counts of possibilities for each digit in structure of 3 chunks, each containing 3 cells.
///
/// This can be;
///  - a horizontal row, made of three horizontal pieces of size 3
///  - a vertical column, made of three vertical pieces of size 3
///  - three horizontal pieces of size 3 contained in a square
///  - three vertical pieces of size 3 contained in a square
class NewLine
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
    NewLine();

    ~NewLine() = default;

    /// @brief Returns true if @p val has been set along this line
    /// @param val Digit
    bool is_value_set(short val) const;

    /// @brief Returns true if @p val has only two possibilities along this line
    /// @param val Digit
    bool has_only_two_possibilities(short val) const;

    /// @brief Indicates that a given digit isn't available anymore
    /// @param val Digit that isn't available anymore (0,1...8)
    /// @param i_block Id of the block on which we're adding the constraint (0,1,2)
    /// @param output_i_lock Output Id of the only block along this line containing possibilities for @p val
    /// @note Use this id only when the output status is @ref Status::LOCK
    /// @return Status
    Status add_constraint(short val, int i_block, int &output_i_lock);

private:
    std::unordered_map<short, std::array<int, 3>> counts_per_val_;
};

#endif // NEW_LINE_H