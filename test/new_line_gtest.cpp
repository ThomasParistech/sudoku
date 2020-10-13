/*********************************************************************************************************************
 * File : new_line_gtest.cpp                                                                                         *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <gtest/gtest.h>

class AAA_GTest : public ::testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(AAA_GTest, aaa)
{
}

TEST_F(AAA_GTest, bbb)
{
}