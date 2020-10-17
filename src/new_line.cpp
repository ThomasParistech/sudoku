/*********************************************************************************************************************
 * File : line.cpp                                                                                                   *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>
#include <unordered_map>

#include "new_line.h"

NewLine::NewLine()
{
    reset();
}

void NewLine::reset()
{
    // At the initialisation each digit 0,1,..8 is available everywhere along the line
    for (short val = 0; val < 9; val++)
        possibilities_per_val_[val].set();

    set_cells_.reset();
}

bool NewLine::is_value_set(short val) const
{
    return set_cells_[val];
}

bool NewLine::has_only_two_possibilities(short val) const
{
    return possibilities_per_val_[val].count() == 2;
}

NewLine::Status NewLine::add_constraint(short val_restrict, int pos, int &output_i)
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

const std::bitset<9> &NewLine::get_bitset(short val) const
{
    return possibilities_per_val_[val];
}

// void NewLine::look_for_locked_sets(std::vector<ValKey> &cells_to_lock)
// {
//     // 1) Look for two values possible on only two cells
//     std::unordered_map<std::bitset<9>, int> map;
//     std::vector<std::pair<int, int>> paired_values;
//     for (int val = 0; val < 9; val++)
//     {
//         if (set_cells_.test(val))
//             continue;

//         const auto &bitset = possibilities_per_val_[val];
//         if (bitset.count() == 2)
//         {
//             auto it = map.find(bitset);
//             if (it == map.end())
//                 map[bitset] == val;
//             else // Two values with the same possibilities
//                 paired_values.emplace_back(it->second, val);
//         }
//     }
//     // Iterate over the line find the two cells with the two values, remove the other candidates inside it
//     // Ex :   (89) (89)  (1289)   (1245) (45) (45)  =>  1 and 2
//     for (const auto &pair : paired_values)
//     {
//         const auto &bitset = possibilities_per_val_[pair.first];
//         for (int i = 0; i < 9; i++)
//             if (bitset.test(i)) // Happens twice
//                 for (int val = 0; val < 9; val++)
//                     if (val != pair.first && val != pair.second)
//                         cells_to_lock.emplace_back(val, i);
//     }
// }