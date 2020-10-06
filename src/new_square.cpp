/*********************************************************************************************************************
 * File : new_square.cpp                                                                                              *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#include <assert.h>

#include "new_square.h"

bool NewSquare::is_value_set(short val)
{
    assert(horizontal_triplets_.is_value_set(val) == vertical_triplets_.is_value_set(val));
    return horizontal_triplets_.is_value_set(val);
}

NewSquare::Status NewSquare::add_constraint(short val,
                                            int i, int j,
                                            int &output_id_lock)
{
    int horiz_id_lock, vert_id_lock;
    const auto horiz_status = horizontal_triplets_.add_constraint(val, i % 3, horiz_id_lock);
    const auto vert_status = vertical_triplets_.add_constraint(val, j % 3, vert_id_lock);

    if (horiz_status == NewLine::Status::SET_VALUE)
    {
        assert(vert_status == NewLine::Status::SET_VALUE);
        return SET_VALUE;
    }

    if (horiz_status == NewLine::Status::LOCK)
    {
        output_id_lock = horiz_id_lock;
        return LOCK_ROW;
    }

    if (vert_status == NewLine::Status::LOCK)
    {
        output_id_lock = vert_id_lock;
        return LOCK_COL;
    }

    return NOTHING;
}