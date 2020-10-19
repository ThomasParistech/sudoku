/*********************************************************************************************************************
 * File : line_gtest.cpp                                                                                             *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <gtest/gtest.h>

#include <line.h>

class Line_GTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}

    /// @brief Tests if the class Line properly adding the constraint of a given value @p val
    /// @param line Line to test
    /// @param val Constraint value to set (0,1...8)
    void test_reset_and_add_constraint(Line line, short val)
    {
        for (int i = 0; i < 9; i++)
        {
            // WHEN add the constraint in the cell
            line.reset();
            int output_i = -1;
            line.add_constraint(val, i, output_i);

            // THEN this cell is removed from the candidates on the line
            const auto &bitset = line.get_bitset(val);
            ASSERT_EQ(8, bitset.count());
            ASSERT_FALSE(bitset[i]);
        }
    }

    void apply_constraints_on_line(Line &line, short val, const std::vector<int> &position_constraints,
                                   std::vector<Line::Status> &output_status, std::vector<int> &output_values,
                                   std::vector<std::bitset<9>> &output_bitsets)
    {
        output_status.clear();
        output_values.clear();

        for (const int pos : position_constraints)
        {
            int output_i = -1;
            output_status.emplace_back(line.add_constraint(val, pos, output_i));
            output_values.emplace_back(output_i);

            output_bitsets.push_back(line.get_bitset(val));
        }
    }
};

TEST_F(Line_GTest, reset_and_add_constraint)
{
    // GIVEN a line for which the value val can be set everywhere
    Line line;

    for (int val = 0; val < 9; val++)
        test_reset_and_add_constraint(line, val);
}

TEST_F(Line_GTest, repeat_same_constraint)
{
    // GIVEN a line with constraints on positions {0, 2, 3, 7}
    Line line;
    const int val = 8;
    int output_i;
    for (const int pos : {0, 2, 3, 7, 8})
        line.add_constraint(val, pos, output_i);

    // WHEN we reapply several time some of these constraints
    const auto &init_bitset = line.get_bitset(val);
    for (int k = 0; k < 5; k++)
    {
        line.add_constraint(val, 0, output_i);
        line.add_constraint(val, 7, output_i);
        line.add_constraint(val, 3, output_i);
    }

    // THEN the line remains unchanged
    const auto &final_bitset = line.get_bitset(val);
    for (int pos = 0; pos < 9; pos++)
        ASSERT_EQ(init_bitset[pos], final_bitset[pos]);
}

TEST_F(Line_GTest, setvalue_left_to_rigtht)
{
    const int val = 4;
    std::vector<int> position_constraints;
    std::vector<Line::Status> output_status;
    std::vector<int> output_values;
    std::vector<std::bitset<9>> output_bitsets;

    for (int pos_ref = 0; pos_ref < 9; pos_ref++)
    {
        // GIVEN constraints on each cell from left to right except one
        position_constraints.clear();
        for (int i = 0; i < 9; i++)
            if (i != pos_ref)
                position_constraints.emplace_back(i);

        // WHEN we add the constraints on the line
        Line line;
        apply_constraints_on_line(line, val, position_constraints, output_status, output_values, output_bitsets);

        // THEN we get the SET_VALUE status we impose the last constraint
        ASSERT_EQ(Line::Status::SET_VALUE, output_status.back()) << " At pos_ref= " << pos_ref;
        for (int i = 0; i < 7; i++)
            ASSERT_NE(Line::Status::SET_VALUE, output_status[i]) << " At pos_ref= " << pos_ref << " and i=" << i;

        ASSERT_EQ(pos_ref, output_values.back()) << " At pos_ref= " << pos_ref;
        ASSERT_TRUE(output_bitsets.back().none()) << " At pos_ref= " << pos_ref;

        ASSERT_TRUE(line.is_value_set(val)) << " At pos_ref= " << pos_ref;
        for (int v = 0; v < 9; v++)
            if (v != val)
                ASSERT_FALSE(line.is_value_set(v)) << " At pos_ref= " << pos_ref << " and v=" << v;
    }
}

TEST_F(Line_GTest, setvalue_right_to_left)
{
    const int val = 4;
    std::vector<int> position_constraints;
    std::vector<Line::Status> output_status;
    std::vector<int> output_values;
    std::vector<std::bitset<9>> output_bitsets;

    for (int pos_ref = 0; pos_ref < 9; pos_ref++)
    {
        // GIVEN constraints on each cell from right to left except one
        position_constraints.clear();
        for (int i = 8; i != -1; i--)
            if (i != pos_ref)
                position_constraints.emplace_back(i);

        // WHEN we add the constraints on the line
        Line line;
        apply_constraints_on_line(line, val, position_constraints, output_status, output_values, output_bitsets);

        // THEN we get the SET_VALUE status we impose the last constraint
        ASSERT_EQ(Line::Status::SET_VALUE, output_status.back()) << " At pos_ref= " << pos_ref;
        for (int i = 0; i < 7; i++)
            ASSERT_NE(Line::Status::SET_VALUE, output_status[i]) << " At pos_ref= " << pos_ref << " and i=" << i;

        ASSERT_EQ(pos_ref, output_values.back()) << " At pos_ref= " << pos_ref;
        ASSERT_TRUE(output_bitsets.back().none()) << " At pos_ref= " << pos_ref;

        ASSERT_TRUE(line.is_value_set(val)) << " At pos_ref= " << pos_ref;
        for (int v = 0; v < 9; v++)
            if (v != val)
                ASSERT_FALSE(line.is_value_set(v)) << " At pos_ref= " << pos_ref << " and v=" << v;
    }
}

TEST_F(Line_GTest, lock)
{
    const int val = 4;
    std::vector<std::vector<int>> position_constraints{
        {2, 5, 3, 6, 8, 4, 7},    // Lock 0
        {7, 0, 1, 2, 4, 6, 8},    // Lock 1
        {5, 4, 3, 2, 1, 0},       // Lock 2
        {0, 1, 2, 3, 4, 6, 7, 5}, // SetValue 8 and Lock 2
        {1, 2, 7, 8, 5, 4, 6, 0}, // SetValue 3 and Lock 1
        {0, 1, 3, 4, 5, 6, 7, 8}  // SetValue 2 and Lock 0
    };

    std::vector<Line::Status> gt_final_statuses{
        Line::Status::LOCK,
        Line::Status::LOCK,
        Line::Status::LOCK,
        Line::Status::SET_VALUE,
        Line::Status::SET_VALUE,
        Line::Status::SET_VALUE};

    std::vector<int> gt_final_output_i{0, 1, 2, 8, 3, 2};

    std::vector<Line::Status> output_status;
    std::vector<int> output_values;
    std::vector<std::bitset<9>> output_bitsets;

    for (int k = 0; k < position_constraints.size(); k++)
    {
        // GIVEN constraints on a line
        const auto constraints = position_constraints[k];

        // WHEN we add the constraints on the line
        Line line;
        apply_constraints_on_line(line, val, constraints, output_status, output_values, output_bitsets);

        // THEN we get the expected status and output i
        ASSERT_EQ(gt_final_statuses[k], output_status.back()) << " At k= " << k;
        for (int i = 0; i < output_status.size() - 1; i++)
            ASSERT_EQ(Line::Status::NOTHING, output_status[i]) << " At k= " << k;

        ASSERT_EQ(gt_final_output_i[k], output_values.back()) << " At k= " << k;
    }
}
