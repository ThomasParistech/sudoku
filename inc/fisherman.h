/*********************************************************************************************************************
 * File : fisherman.h                                                                                                *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef FISHERMAN_H
#define FISHERMAN_H

#include <array>
#include <unordered_map>
#include <vector>

/// @brief X-Wing, Swordfish, Jellyfish ...
/// Instantiate either for column or row, but must choose
class FisherMan
{
public:
    /// @brief Struct representing a perpendicular line to lock, and two indices to keep
    struct LockInfo
    {
        int perpendicular_i;
        int line_i1;
        int line_i2;
    };

    /// @brief Constructor
    FisherMan() = default;

    ~FisherMan() = default;

    /// @brief Resets the square to its original state without any pairs
    void reset();

    /// @brief Looks for a fish pattern
    /// @note If no fish has been found, the class keeps this pair it in memory in hope of a future fish
    /// @param val Value
    /// @param line_id Id of the current line owning the pair
    /// @param i1 First possible location of the pair
    /// @param i2 Second possible location of the pair
    /// @param output_lock_info Perpendicular lines to lock
    /// @return true if a fish pattern has been found
    bool try_match_pair(short val, int line_id, int i1, int i2, std::vector<LockInfo> &output_lock_info);

private:
    struct Pt
    {
        Pt() = default;
        Pt(int i, int j);

        int i;
        int j;
    };

    using PairsMap = std::unordered_map<int, std::vector<Pt>>; ///< For each location i along the line,
                                                               ///< get the vector of (j, k) with j the second location
                                                               ///< along the line and k the id of the new line
                                                               ///< N.B a pair (a,b) is thus present twice in the map
                                                               ///< at keys a and b

    /// @brief Finds a fish using backtracking
    /// @param path Backtracking path
    /// @param pairs_map Available pairs for the fixed desired value
    /// @param i_target Target to close the path. For a pair [i1, i2], starts the path using i1 and try to
    /// end it on i2
    //// @return true if a fish pattern has been found
    bool find_fish(std::vector<Pt> &path, const PairsMap &pairs_map, const int i_target);

    /// @brief Adds pair to the map
    /// @param pairs_map Available pairs for the fixed desired value
    /// @param line_id Id of the current line owning the pair
    /// @param i1 First possible location of the pair
    /// @param i2 Second possible location of the pair
    void add_pair(PairsMap &pairs_map, int line_id, int i1, int i2);

    std::vector<Pt> fish_path_;
    std::array<PairsMap, 9> pairs_; ///< Map of pairs along the line for each value
};

#endif // FISHERMAN_H