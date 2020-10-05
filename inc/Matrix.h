#ifndef MATRIX_H
#define MATRIX_H

#include "Cell.h"
#include "Line.h"
#include "Square.h"

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <algorithm>

using namespace std;

typedef std::pair<int, std::vector<Cell *>> CellToLock;
struct CellToAdd
{
    int iRow, jCol, val;
    CellToAdd(int _iRow, int _jCol, int _val) : iRow(_iRow), jCol(_jCol), val(_val) {}
    CellToAdd() {}
};

class Matrix
{
public:
    Matrix();
    virtual ~Matrix();

    std::array<std::array<Cell, 9>, 9> cells;
    std::array<Line, 9> rows, cols;
    std::array<std::array<Square, 3>, 3> blocks;
    std::vector<CellToAdd> initCells;

    std::vector<std::vector<std::vector<int>>> xWing;
    //( \           )
    //(   \   Rows  )
    //(     \       )
    //(       \     )
    //(  Cols   \   )
    //(           \ )

    CellToAdd findNewInSquare(int val, int i0, int j0);
    CellToAdd findNewInCol(int val, int idCol);
    CellToAdd findNewInRow(int val, int idRow);

    void decreaseNbr(int val, Cell *cell, std::vector<CellToAdd> &cellsToAdd, std::vector<CellToLock> &cellsToLock, std::vector<Cell *> &cellsToPair);
    void setValue(CellToAdd c);
    void update(int valRestrict, std::vector<Cell *> cellsUpdated);
    std::vector<Cell *> getSquareWithoutPair(int i0, int j0, int idRow, int idCol, int i1, int j1);
    std::vector<Cell *> getRowWithoutPair(int iRow, int j1, int j2);
    std::vector<Cell *> getColWithoutPair(int jCol, int i1, int i2);
    std::vector<Cell *> getAllNeighbors(int iRow, int iCol);
    std::vector<Cell *> getSquareLockedRow(int i0, int j0, int iRow);
    std::vector<Cell *> getSquareLockedCol(int i0, int j0, int jCol);
    std::vector<Cell *> getRowLockedSquare(int i0, int j0, int idHoriz);
    std::vector<Cell *> getColLockedSquare(int i0, int j0, int idVert);
    void save();
    void disp();
    void load(std::string name);
    void solve();

    void dispAllLinesPossible(std::array<Line, 9> &lines, std::string name);
    void dispAllLinesPossible();

    void dispAllSquaresPossible();
    void dispGridVal(int val);

    void dispAllNbrPossible();
    void eraseRemainingCandidates(int i, int j);

    void updateXWingRow(int val, Line &line, std::vector<CellToLock> &cellsToLock);
    void updateXWingCol(int val, Line &col, std::vector<CellToLock> &cellsToLock);

    void doColoring(int val);
    void spreadColoringPath(int &lengthPath, bool &realPath, int iRow, int jCol, int val, int initColour, int currentColour, std::vector<std::vector<int>> &coloringPaths);
    bool getColoringNeighbors(int iRow, int jCol, int val, std::vector<std::pair<int, int>> &coloringNeighbors);
};

#endif // MATRIX_H
