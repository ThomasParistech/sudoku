/*********************************************************************************************************************
 * File : backtracking_solver.h                                                                                      *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef BACK_TRACKING_SOLVER_H
#define BACK_TRACKING_SOLVER_H

#include <array>

#include "cell.h"
#include "val_key.h"

class BackTrackingSolver
{
public:
    bool solve(const std::vector<ValKey> &set_cells);

    bool solve(const std::array<Cell, 81> &cells_with_candidates);

    const std::array<short, 81> &get_cells() const;

private:
    bool find_solution(int key_to_test);

    bool find_solution(const std::array<Cell, 81> &cells_with_candidates,
                       int key_to_test);

    bool is_value_valid(short val, int key);

    std::array<short, 81> cells_;
};

#endif // BACK_TRACKING_SOLVER_H
