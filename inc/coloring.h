/*********************************************************************************************************************
 * File : coloring.h                                                                                                 *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef COLORING_H
#define COLORING_H

#include <array>
#include <bitset>
#include <unordered_set>
#include <vector>

#include "cell.h"
#include "square.h"
#include "val_key.h"

/// @brief Class implementing a well known single-digit solving technique that uses colors to mark the parity of
/// candidates
///
/// If a digit is a candidate for exactly two cells in a row, in a column or in a square, then it's one or the other.
/// Let these two cells be A and B. If A is also linked as pair with another cell C from another structure, then
/// B and C have the same status: either both take the value, or none of them. That way, starting from A and B, we
/// can spread two  "coloring paths" alternating between the colors of A and B.
///
/// If we end up with two cells of the same color A that can't be set at the same time (same row, same column, same
/// square), then it confirms the fact that B must be set.
class Coloring
{
public:
    /// @brief Constructor
    Coloring() = default;

    ~Coloring() = default;

    /// @brief Finds pairs of @p val inside a row, column or square, computes their coloring paths and outputs cells
    /// to add if one of the two paths of a pair isn't valid
    /// @param val Value
    /// @param cells 9x9 grid of cells
    /// @param rows 9 rows
    /// @param columns 9 columns
    /// @param squares 9 squares
    /// @param output_cells_to_add Keys of the cells to set at @p val
    void run(short val,
             const std::array<Cell, 81> &cells,
             const std::array<Line, 9> &rows,
             const std::array<Line, 9> &columns,
             const std::array<Square, 9> &squares,
             std::vector<ValKey> &output_cells_to_add);

private:
    /// @brief Propagates a coloring path in a recursive way
    /// @param key Location of the cell (9*i+j)
    /// @param is_base_color Indicate which of the two intricated color paths it is
    bool spread_coloring_path(int key, bool is_base_color);

    /// @brief Extracts pairs coming from rows, columns and squares, and stores them inside attribute existing_pairs_
    /// @param val Value
    /// @param cells 9x9 grid of cells
    /// @param rows 9 rows
    /// @param columns 9 columns
    /// @param squares 9 squares
    void extract_all_pairs(short val, const std::array<Cell, 81> &cells, const std::array<Line, 9> &rows,
                           const std::array<Line, 9> &columns, const std::array<Square, 9> &squares);

    /// @brief Struct storing the history of a coloring path and detecting when this path can't be valid
    struct ValidityChecker
    {
        ValidityChecker() = default;

        /// @brief Clears history about rows, columns and squares
        void reset();

        /// @brief Checks if a cell location is valid during coloring spread and adds it if it's the case
        /// @param k Position 9*i + j
        /// @return true if the location is valid
        bool is_location_valid(int k);

        bool is_valid = true;
        std::unordered_set<int> history_rows;
        std::unordered_set<int> history_cols;
        std::unordered_set<int> history_squares;
    };

    ValidityChecker valid_checker_0_;
    ValidityChecker valid_checker_1_;

    std::bitset<81> colored_cells_;                   ///< 2D 9x9 grid indicating if a cell is colored or not
    std::array<std::vector<int>, 81> existing_pairs_; ///< 2D 9x9 grid containing for each cell the position (9*i+j) of
                                                      ///< the cells that form a pair with it. There are atmost 3 pairs,
                                                      ///< since they might come from the row, column or square
};

#endif // COLORING_H