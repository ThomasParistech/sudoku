/*********************************************************************************************************************
 * File : new_square.cpp                                                                                              *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "new_square.h"

void NewSquare::reset()
{
    horizontal_triplets_.reset();
    vertical_triplets_.reset();
}

bool NewSquare::is_value_set(short val)
{
    assert(horizontal_triplets_.is_value_set(val) == vertical_triplets_.is_value_set(val));
    return horizontal_triplets_.is_value_set(val);
}

NewSquare::Status NewSquare::add_constraint(short val_restrict, int i, int j, int &output_i)
{
    int horiz_output_id, vert_output_id;
    const auto horiz_status = horizontal_triplets_.add_constraint(val_restrict, i % 3, horiz_output_id);
    const auto vert_status = vertical_triplets_.add_constraint(val_restrict, j % 3, vert_output_id);

    if (horiz_status == NewLine::Status::SET_VALUE)
    {
        assert(vert_status == NewLine::Status::SET_VALUE);
        assert(horiz_output_id == vert_output_id);
        output_i = horiz_output_id;
        return SET_VALUE;
    }

    if (horiz_status == NewLine::Status::LOCK)
    {
        output_i = horiz_output_id;
        return LOCK_ROW;
    }

    if (vert_status == NewLine::Status::LOCK)
    {
        output_i = vert_output_id;
        return LOCK_COL;
    }

    return NOTHING;
}

bool NewSquare::has_only_two_possibilities(short val) const
{
    return horizontal_triplets_.has_only_two_possibilities(val);
}
