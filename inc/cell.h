/*********************************************************************************************************************
 * File : cell.h                                                                                                     *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef CELL_H
#define CELL_H

#include <unordered_set>
#include <vector>

/// @brief Base class storing digit candidates between 0 and 8
class Cell
{
public:
    enum Status
    {
        SET_VALUE,
        PAIR,
        ALREADY_KNOWN,
        UPDATED
    };

    /// @brief Constructor
    Cell();

    ~Cell() = default;

    /// @brief Resets the cell to its original state with 9 candidates
    void reset();

    /// @brief Gets the number of available digits
    size_t get_nbr_candidates() const;

    /// @brief Gets the value of the cell, assuming its value is already known
    short get_value() const;

    /// @brief Returns true if the value of the cell is known
    bool is_set() const;

    /// @brief Returns true if a value is possible
    /// @param val Value to check
    bool is_value_possible(short val) const;

    /// @brief Indicates that a given digit isn't available anymore
    /// @param val_restrict Digit that isn't available anymore (0,1...8)
    /// @return Status
    Status add_constraint(short val_restrict);

    /// @brief Gets candidates on this cell that are different than @p val
    /// @param val Digit to set (0,1...8)
    /// @param remaining_digits Remainining possible digits (excluding @p val )
    /// @note If the choice of the digit is caused by a line or a square, then the set of candidates is not
    /// reduced to a single value yet
    void get_remaining_candidates(short val, std::vector<short> &remaining_digits) const;

    /// @brief Gets a const reference to the set of candidates
    const std::unordered_set<short> &get_candidates() const;

private:
    std::unordered_set<short> possible_digits_;
};

#endif // CELL_H