/*********************************************************************************************************************
 * File : line.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>
#include <unordered_map>

#include "line.h"

Line::Line()
{
    reset();
}

void Line::reset()
{
    // At the initialisation each digit 0,1,..8 is available everywhere along the line
    for (short val = 0; val < 9; val++)
        possibilities_per_val_[val].set();

    set_cells_.reset();
}

bool Line::is_value_set(short val) const
{
    return set_cells_[val];
}

bool Line::has_only_two_possibilities(short val) const
{
    return possibilities_per_val_[val].count() == 2;
}

Line::Status Line::add_constraint(short val_restrict, int pos, int &output_i)
{
    auto &bitset = possibilities_per_val_[val_restrict];
    if (!bitset.test(pos)) // We already have the info
        return NOTHING;

    bitset.reset(pos);
    if (bitset.count() == 1)
    {
        for (int i = 0; i < 9; i++)
            if (bitset.test(i))
            {
                output_i = i;
                bitset.reset();
                set_cells_.set(val_restrict);
                return SET_VALUE;
            }
    }

    bool is_subpart_non_empty[3];
    is_subpart_non_empty[0] = bitset.test(0) || bitset.test(1) || bitset.test(2);
    is_subpart_non_empty[1] = bitset.test(3) || bitset.test(4) || bitset.test(5);
    is_subpart_non_empty[2] = bitset.test(6) || bitset.test(7) || bitset.test(8);

    const int crt_subpart = pos / 3;
    if (is_subpart_non_empty[crt_subpart])
        return NOTHING;

    int i_non_empty;
    int count_non_empty = 0;
    for (int i = 0; i < 3; i++)
    {
        if (is_subpart_non_empty[i])
        {
            i_non_empty = i;
            count_non_empty++;
        }
    }

    if (count_non_empty != 1)
        return NOTHING;

    output_i = i_non_empty;
    return LOCK;
}

const std::bitset<9> &Line::get_bitset(short val) const
{
    return possibilities_per_val_[val];
}
