/*********************************************************************************************************************
 * File : fisherman.cpp                                                                                              *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "fisherman.h"

FisherMan::LockInfo::LockInfo(int perpendicular_i, int line_i1, int line_i2) : perpendicular_i(perpendicular_i),
                                                                               line_i1(line_i1),
                                                                               line_i2(line_i2) {}

FisherMan::Pt::Pt(int i, int id_line) : i(i), id_line(id_line)
{
}

void FisherMan::reset()
{
    for (int k = 0; k < 9; k++)
        pairs_[k].clear(); // Warning we loose the memory allocation of the vectors
                           // We should remplace std::map by an array
}

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
        info.line_i1 = pt.id_line;
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
        // Make sure it's on a different line
        if (pt.id_line == last_pt.id_line)
            continue;

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