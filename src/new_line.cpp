/*********************************************************************************************************************
 * File : line.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include "new_line.h"

NewLine::NewLine()
{
    // At the initialisation each digit 0,1,..8 is available everywhere along the line
    for (short val = 0; val < 9; val++)
    {
        auto &counts = counts_per_val_[val];
        counts.fill(3);
    }
}

bool NewLine::is_value_set(short val)
{
    const auto it = counts_per_val_.find(val);
    return (it != counts_per_val_.end());
}

bool NewLine::has_only_two_possibilities(short val)
{
    const auto it = counts_per_val_.find(val);
    if (it == counts_per_val_.end())
        return false;

    const auto &counts = it->second;
    const int count = counts[0] + counts[1] + counts[2];
    return count == 2;
}

NewLine::Status NewLine::add_constraint(short val, int i_block, int &output_i_lock)
{
    const auto it = counts_per_val_.find(val);
    if (it == counts_per_val_.end())
        return NOTHING;

    auto &counts = it->second;
    counts[i_block]--;
    const int count = counts[0] + counts[1] + counts[2];
    if (count == 1)
    {
        counts_per_val_.erase(val);
        return SET_VALUE;
    }

    if (counts[i_block] != 0)
        return NOTHING;

    int i_empty = -1, i_full = -1;
    for (int i = 0; i < 3; i++)
    {
        if (i == i_block)
            continue;

        if (counts[i] == 0)
            i_empty = i;
        else
            i_full = i;
    }

    if (i_empty == 1)
        return NOTHING;

    output_i_lock = i_full;
    return LOCK;
}