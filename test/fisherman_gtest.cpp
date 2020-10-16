/*********************************************************************************************************************
 * File : fisherman_gtest.cpp                                                                                        *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <gtest/gtest.h>

#include <fisherman.h>

class FisherMan_GTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}

    void assert_same_lock_infos(const std::vector<std::vector<FisherMan::LockInfo>> &gt_infos,
                                std::vector<std::vector<FisherMan::LockInfo>> &test_infos)
    {
        ASSERT_EQ(gt_infos.size(), test_infos.size());
        for (int k = 0; k < gt_infos.size(); k++)
        {
            const auto &gt_info_vec = gt_infos[k];
            auto &test_info_vec = test_infos[k];
            ASSERT_EQ(gt_info_vec.size(), test_info_vec.size());

            std::sort(test_info_vec.begin(), test_info_vec.end(),
                      [](const FisherMan::LockInfo &left, const FisherMan::LockInfo &right) {
                          return left.perpendicular_i < right.perpendicular_i;
                      });

            for (int i = 0; i < gt_info_vec.size(); i++)
            {
                ASSERT_EQ(gt_info_vec[i].perpendicular_i, test_info_vec[i].perpendicular_i);
                ASSERT_EQ(gt_info_vec[i].line_i1, std::min(test_info_vec[i].line_i1, test_info_vec[i].line_i2));
                ASSERT_EQ(gt_info_vec[i].line_i2, std::max(test_info_vec[i].line_i1, test_info_vec[i].line_i2));
            }
        }
    }
};

TEST_F(FisherMan_GTest, no_fish_and_no_overlap)
{
    // GIVEN pairs that don't form any fish pattern
    const int val = 6;
    std::vector<std::array<int, 3>> input_pairs = {{7, 1, 3},
                                                   {6, 4, 6},
                                                   {0, 2, 8},
                                                   {5, 0, 7}};

    // WHEN we add these pairs to the class
    FisherMan fisherman;
    std::vector<std::pair<bool, std::vector<FisherMan::LockInfo>>> outputs;
    for (const auto &pair_on_line : input_pairs)
    {
        std::vector<FisherMan::LockInfo> output_lock_info;
        const bool success = fisherman.try_match_pair(val, pair_on_line[0], pair_on_line[1], pair_on_line[2],
                                                      output_lock_info);
        outputs.emplace_back(success, std::move(output_lock_info));
    }

    // THEN the class doesn't find any fish pattern
    for (const auto &output : outputs)
        ASSERT_FALSE(output.first) << "Pattern of depth " << output.second.size();
}

TEST_F(FisherMan_GTest, no_fish_but_overlap)
{
    // GIVEN pairs that don't form any fish pattern
    const int val = 6;
    std::vector<std::array<int, 3>> input_pairs = {{7, 1, 3},
                                                   {6, 4, 6},
                                                   {1, 2, 3},
                                                   {0, 3, 4},
                                                   {5, 2, 8},
                                                   {4, 7, 8}};

    // WHEN we add these pairs to the class
    FisherMan fisherman;
    std::vector<std::pair<bool, std::vector<FisherMan::LockInfo>>> outputs;
    for (const auto &pair_on_line : input_pairs)
    {
        std::vector<FisherMan::LockInfo> output_lock_info;
        const bool success = fisherman.try_match_pair(val, pair_on_line[0], pair_on_line[1], pair_on_line[2],
                                                      output_lock_info);
        outputs.emplace_back(success, std::move(output_lock_info));
    }

    // THEN the class doesn't find any fish pattern
    for (const auto &output : outputs)
        ASSERT_FALSE(output.first) << "Pattern of depth " << output.second.size();
}

TEST_F(FisherMan_GTest, fish_size_2_easy)
{
    // GIVEN pairs that form a fish of size 2
    const int val = 6;
    std::vector<std::array<int, 3>> input_pairs = {{4, 5, 8},
                                                   {6, 5, 8}};
    std::vector<std::vector<FisherMan::LockInfo>> gt_infos{{{5, 4, 6}, {8, 4, 6}}};
    // WHEN we add these pairs to the class
    FisherMan fisherman;
    std::vector<bool> checks;
    std::vector<std::vector<FisherMan::LockInfo>> output_lock_infos_;
    for (const auto &pair_on_line : input_pairs)
    {
        std::vector<FisherMan::LockInfo> lock_info;
        const bool success = fisherman.try_match_pair(val, pair_on_line[0], pair_on_line[1], pair_on_line[2],
                                                      lock_info);
        checks.emplace_back(success);
        if (success)
            output_lock_infos_.emplace_back(std::move(lock_info));
    }

    // THEN the class finds the fish pattern
    ASSERT_FALSE(checks[0]);
    ASSERT_TRUE(checks[1]);

    assert_same_lock_infos(gt_infos, output_lock_infos_);
}

TEST_F(FisherMan_GTest, fish_size_2_difficult)
{
    // GIVEN pairs that form 2 fishs of size 2 and pairs that remain alone
    const int val = 6;
    std::vector<std::array<int, 3>> input_pairs = {{7, 3, 1},
                                                   {4, 5, 8},
                                                   {1, 4, 8},
                                                   {6, 5, 8},
                                                   {5, 2, 1},
                                                   {2, 1, 3}};
    std::vector<std::vector<FisherMan::LockInfo>> gt_infos{{{5, 4, 6}, {8, 4, 6}},
                                                           {{1, 2, 7}, {3, 2, 7}}};
    // WHEN we add these pairs to the class
    FisherMan fisherman;
    std::vector<bool> checks;
    std::vector<std::vector<FisherMan::LockInfo>> output_lock_infos_;
    for (const auto &pair_on_line : input_pairs)
    {
        std::vector<FisherMan::LockInfo> lock_info;
        const bool success = fisherman.try_match_pair(val, pair_on_line[0], pair_on_line[1], pair_on_line[2],
                                                      lock_info);
        checks.emplace_back(success);
        if (success)
            output_lock_infos_.emplace_back(std::move(lock_info));
    }

    // THEN the class finds the 2 fish patterns
    ASSERT_FALSE(checks[0]);
    ASSERT_FALSE(checks[1]);
    ASSERT_FALSE(checks[2]);
    ASSERT_TRUE(checks[3]);
    ASSERT_FALSE(checks[4]);
    ASSERT_TRUE(checks[5]);

    assert_same_lock_infos(gt_infos, output_lock_infos_);
}

TEST_F(FisherMan_GTest, fish_size_3_easy)
{
    // GIVEN pairs that form a fish of size 2
    const int val = 6;
    std::vector<std::array<int, 3>> input_pairs = {{4, 5, 8},
                                                   {2, 5, 1},
                                                   {6, 1, 8}};
    std::vector<std::vector<FisherMan::LockInfo>> gt_infos{{{1, 2, 6}, {5, 2, 4}, {8, 4, 6}}};
    // WHEN we add these pairs to the class
    FisherMan fisherman;
    std::vector<bool> checks;
    std::vector<std::vector<FisherMan::LockInfo>> output_lock_infos_;
    for (const auto &pair_on_line : input_pairs)
    {
        std::vector<FisherMan::LockInfo> lock_info;
        const bool success = fisherman.try_match_pair(val, pair_on_line[0], pair_on_line[1], pair_on_line[2],
                                                      lock_info);
        checks.emplace_back(success);
        if (success)
            output_lock_infos_.emplace_back(std::move(lock_info));
    }

    // THEN the class finds the fish pattern
    ASSERT_FALSE(checks[0]);
    ASSERT_FALSE(checks[1]);
    ASSERT_TRUE(checks[2]);

    assert_same_lock_infos(gt_infos, output_lock_infos_);
}