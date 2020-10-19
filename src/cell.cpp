/*********************************************************************************************************************
 * File : cell.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "cell.h"

Cell::Cell()
{
    reset();
}

void Cell::reset()
{
    // At the initialisation each digit is possible: 0,1,..8
    for (short val = 0; val < 9; val++)
        possible_digits_.insert(val);
}

size_t Cell::get_nbr_candidates() const
{
    return possible_digits_.size();
}

short Cell::get_value() const
{
    assert(possible_digits_.size() == 1);
    return *possible_digits_.begin();
}

bool Cell::is_set() const
{
    return possible_digits_.size() == 1;
}

bool Cell::is_value_possible(short val) const
{
    return possible_digits_.find(val) != possible_digits_.end();
}

Cell::Status Cell::add_constraint(short val_restrict)
{
    if (possible_digits_.size() == 1)
        return ALREADY_KNOWN;

    const int success = possible_digits_.erase(val_restrict);
    if (success == 0)
        return ALREADY_KNOWN;

    if (possible_digits_.size() == 1)
        return SET_VALUE;

    if (possible_digits_.size() == 2)
        return PAIR;

    return UPDATED;
}

void Cell::get_remaining_candidates(short val, std::vector<short> &remaining_digits) const
{
    remaining_digits.clear();
    for (const auto &k : possible_digits_)
        if (k != val)
            remaining_digits.emplace_back(k);
}

const std::unordered_set<short> &Cell::get_candidates() const
{
    return possible_digits_;
}