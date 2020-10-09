/*********************************************************************************************************************
 * File : fisherman.cpp                                                                                              *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "fisherman.h"

bool FisherMan::try_match_pair(short val, int line_id, int i1, int i2, std::vector<LockInfo> &output_lock_info)
{
    PairsMap &pairs_map = pairs_[val];
    fish_path_.clear();
    fish_path_.emplace_back(i1, line_id);

    output_lock_info.clear();
    if (!find_fish(fish_path_, pairs_map, i2))
    {
        add_pair(pairs_map, line_id, i1, i2);
        return false;
    }

    // Recover info about perpendicular lines to lock
    assert(!fish_path_.empty());
    for (const auto &pt : fish_path_)
    {
        LockInfo info;
        info.perpendicular_i = pt.i;
        info.line_i1 = pt.j;
        info.line_i2 = -1;

        if (!output_lock_info.empty())
            output_lock_info.back().line_i2 = info.line_i1;

        output_lock_info.emplace_back(std::move(info));
    }
    output_lock_info.back().line_i2 = line_id;
    return true;
}

bool FisherMan::find_fish(std::vector<Pt> &path, const PairsMap &pairs_map, const int i_target)
{
    const Pt &last_pt = path.back();
    if (last_pt.i == i_target)
        return true;

    auto it_pairs = pairs_map.find(last_pt.i);
    if (it_pairs == pairs_map.end())
        return false;

    // Go deeper into the path
    for (const auto &pt : it_pairs->second)
    {
        path.emplace_back(pt);
        if (find_fish(path, pairs_map, i_target))
            return true;
        path.pop_back();
    }
    return false;
}

void FisherMan::add_pair(PairsMap &pairs_map, int line_id, int i1, int i2)
{
    auto &vec_pairs_i1 = pairs_map[i1];
    vec_pairs_i1.emplace_back(i2, line_id);

    auto &vec_pairs_i2 = pairs_map[i2];
    vec_pairs_i2.emplace_back(i1, line_id);
}