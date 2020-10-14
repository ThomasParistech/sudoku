/*********************************************************************************************************************
 * File : cell.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "new_cell.h"

NewCell::NewCell()
{
    reset();
}

void NewCell::reset()
{
    // At the initialisation each digit is possible: 0,1,..8
    for (short val = 0; val < 9; val++)
        possible_digits_.insert(val);
}

size_t NewCell::get_nbr_candidates() const
{
    return possible_digits_.size();
}

short NewCell::get_value() const
{
    assert(possible_digits_.size() == 1);
    return *possible_digits_.begin();
}

bool NewCell::is_set() const
{
    return possible_digits_.size() == 1;
}

bool NewCell::is_value_possible(short val) const
{
    return possible_digits_.find(val) != possible_digits_.end();
}

NewCell::Status NewCell::add_constraint(short val)
{
    const int success = possible_digits_.erase(val);
    if (success == 0)
        return ALREADY_KNOWN;

    if (possible_digits_.size() == 1)
        return SET_VALUE;

    if (possible_digits_.size() == 2)
        return PAIR;

    return UPDATED;
}

void NewCell::get_remaining_candidates(short val, std::vector<short> &remaining_digits)
{
    remaining_digits.clear();
    for (const auto &k : possible_digits_)
        if (k != val)
            remaining_digits.emplace_back(k);
}

const std::unordered_set<short> &NewCell::get_candidates()
{
    return possible_digits_;
}