#include "Matrix.h"

Matrix::Matrix()
{
    //ctor
    for (int i = 0; i < 9; i++)
    {
        rows[i].idLine = i;
        cols[i].idLine = i;
        rows[i].isRow = true;
        cols[i].isRow = false;

        for (int j = 0; j < 9; j++)
        {
            cells[i][j].idRow = i;
            cells[i][j].idCol = j;
            int i0 = int(i / 3), j0 = int(j / 3);
            cells[i][j].idRowBlock = i0;
            cells[i][j].idColBlock = j0;
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            blocks[i][j].idRowBlock = i;
            blocks[i][j].idColBlock = j;
        }
    }

    xWing = vector<vector<vector<int>>>(9, vector<vector<int>>(9, vector<int>(9, -1)));
}

CellToAdd Matrix::findNewInSquare(int val, int i0, int j0)
{
    for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
        for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
            if (cells[i][j].possible[val])
                return CellToAdd(i, j, val);

    return CellToAdd();
}

CellToAdd Matrix::findNewInCol(int val, int idCol)
{
    for (int i = 0; i < 9; i++)
        if (cells[i][idCol].possible[val])
            return CellToAdd(i, idCol, val);

    return CellToAdd();
}
CellToAdd Matrix::findNewInRow(int val, int idRow)
{
    for (int j = 0; j < 9; j++)
        if (cells[idRow][j].possible[val])
            return CellToAdd(idRow, j, val);

    return CellToAdd();
}

void Matrix::decreaseNbr(int val, Cell *cell, std::vector<CellToAdd> &cellsToAdd, std::vector<CellToLock> &cellsToLock, std::vector<Cell *> &cellsToPair)
{
    int i = cell->idRow, j = cell->idCol;
    //cell
    Cell::TO_DO resultCell = (*cell).decreaseNbr(val);
    if (cell->isFree())
    {
        switch (resultCell)
        {
        case Cell::TO_DO::SETVALUE:
            cellsToAdd.push_back(CellToAdd(i, j, (*cell).findNew()));
            break;
        case Cell::TO_DO::PAIR:
            cellsToPair.push_back(cell);
            break;
        case Cell::TO_DO::NOTHING:
            break;
        }
    }

    int iBlock = cell->idRowBlock;
    int jBlock = cell->idColBlock;
    //row
    if (rows[i].getTotalNbrPossible(val) != -3)
    {
        switch (rows[i].decreaseNbr(val, jBlock))
        {
        case Line::TO_DO::SETVALUE:
            cellsToAdd.push_back(findNewInRow(val, i));
            break;
        case Line::TO_DO::LOCK_SQUARE0:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedRow(iBlock, 0, i)));
            break;
        case Line::TO_DO::LOCK_SQUARE1:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedRow(iBlock, 1, i)));
            break;
        case Line::TO_DO::LOCK_SQUARE2:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedRow(iBlock, 2, i)));
            break;
        case Line::TO_DO::NOTHING:
            break;
        }

        if (rows[i].getTotalNbrPossible(val) == 2)
        {
            updateXWingRow(val, rows[i], cellsToLock);
        }
    }

    //col
    if (cols[j].getTotalNbrPossible(val) != -3)
    {
        switch (cols[j].decreaseNbr(val, iBlock))
        {
        case Line::TO_DO::SETVALUE:
            cellsToAdd.push_back(findNewInCol(val, j));
            break;
        case Line::TO_DO::LOCK_SQUARE0:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedCol(0, jBlock, j)));
            break;
        case Line::TO_DO::LOCK_SQUARE1:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedCol(1, jBlock, j)));
            break;
        case Line::TO_DO::LOCK_SQUARE2:
            cellsToLock.push_back(std::make_pair(val, getSquareLockedCol(2, jBlock, j)));
            break;
        case Line::TO_DO::NOTHING:
            break;
        }
        if (cols[j].getTotalNbrPossible(val) == 2)
        {
            updateXWingCol(val, cols[j], cellsToLock);
        }
    }

    //square
    if (blocks[iBlock][jBlock].getTotalNbrPossible(val) != -3)
    {
        switch (blocks[iBlock][jBlock].decreaseNbr(val, iBlock, jBlock, i, j))
        {
        case Square::TO_DO::SETVALUE:
            cellsToAdd.push_back(findNewInSquare(val, iBlock, jBlock));
            break;
        case Square::TO_DO::LOCK_ROW0:
            cellsToLock.push_back(std::make_pair(val, getRowLockedSquare(iBlock, jBlock, 0)));
            break;
        case Square::TO_DO::LOCK_ROW1:
            cellsToLock.push_back(std::make_pair(val, getRowLockedSquare(iBlock, jBlock, 1)));
            break;
        case Square::TO_DO::LOCK_ROW2:
            cellsToLock.push_back(std::make_pair(val, getRowLockedSquare(iBlock, jBlock, 2)));
            break;

        case Square::TO_DO::LOCK_COL0:
            cellsToLock.push_back(std::make_pair(val, getColLockedSquare(iBlock, jBlock, 0)));
            break;
        case Square::TO_DO::LOCK_COL1:
            cellsToLock.push_back(std::make_pair(val, getColLockedSquare(iBlock, jBlock, 1)));
            break;
        case Square::TO_DO::LOCK_COL2:
            cellsToLock.push_back(std::make_pair(val, getColLockedSquare(iBlock, jBlock, 2)));
            break;
        case Square::TO_DO::NOTHING:
            break;
        }
    }
}

void Matrix::updateXWingRow(int val, Line &row, std::vector<CellToLock> &cellsToLock)
{
    int j1 = -1, j2 = -1;
    int iRow = row.idLine;
    for (int j = 0; j < 9; j++)
    {
        if (cells[iRow][j].possible[val])
        {
            if (j1 == -1)
                j1 = j;
            else
            {
                j2 = j;
                break;
            }
        }
    }
    int i2 = xWing[val][j1][j2]; //upper part of xWing, j1 < j2
    if (i2 != -1)
    {
        xWing[val][j1][j2] = -1;
        cellsToLock.push_back(std::make_pair(val, getColWithoutPair(j1, i2, iRow)));
        cellsToLock.push_back(std::make_pair(val, getColWithoutPair(j2, i2, iRow)));
    }
    else
    {
        xWing[val][j1][j2] = iRow;
    }
}
void Matrix::updateXWingCol(int val, Line &col, std::vector<CellToLock> &cellsToLock)
{
    int i1 = -1, i2 = -1;
    int jCol = col.idLine;
    for (int i = 0; i < 9; i++)
    {
        if (cells[i][jCol].possible[val])
        {
            if (i1 == -1)
                i1 = i;
            else
            {
                i2 = i;
                break;
            }
        }
    }
    int j2 = xWing[val][i2][i1]; //lower part of xWing, i2 > i1
    if (j2 != -1)
    {
        xWing[val][i2][i1] = -1;
        cellsToLock.push_back(std::make_pair(val, getRowWithoutPair(i1, j2, jCol)));
        cellsToLock.push_back(std::make_pair(val, getRowWithoutPair(i2, j2, jCol)));
    }
    else
    {
        xWing[val][i2][i1] = jCol;
    }
}

void Matrix::setValue(CellToAdd c)
{
    cells[c.iRow][c.jCol].setValue(c.val);
}

void Matrix::update(int valRestrict, std::vector<Cell *> cellsUpdated)
{ // the value has just been updated. We update the restrictions on the neighbors

    //cells[5][4].dispPossible();
    //rows[5].dispLinePossible(7, "Row");
    //blocks[1][1].dispSquarePossible(7);
    //disp();
    std::vector<CellToAdd> cellsToAdd; // it the nbr of possible candidates in a cell or in a block becomes one, we update it
    std::vector<CellToLock> cellsToLock;
    std::vector<Cell *> cellsToPair;

    for (auto c : cellsUpdated)
    {
        if (c->possible[valRestrict])
        {
            decreaseNbr(valRestrict, c, cellsToAdd, cellsToLock, cellsToPair);
        }
    }

    for (auto c : cellsToLock)
    {
        update(c.first, c.second);
    }
    for (auto c : cellsToAdd)
    {
        int i = c.iRow, j = c.jCol;
        int i0 = int(i / 3), j0 = int(j / 3);
        setValue(c);
        rows[i].setValue(c.val);
        cols[j].setValue(c.val);
        blocks[i0][j0].setValue(c.val);
        eraseRemainingCandidates(i, j);
        update(c.val, getAllNeighbors(c.iRow, c.jCol));
    }

    for (auto c : cellsToPair)
    {
        if (c->nbrPossible == 2)
        { // it might have change with all the other updates
            int k1 = -1, k2 = -1;
            for (int k = 0; k < 9; k++)
            {
                if (c->possible[k])
                {
                    if (k1 == -1)
                        k1 = k;
                    else
                    {
                        k2 = k;
                        break;
                    }
                }
            }

            //row
            for (int j = 0; j < 9; j++)
            {
                if (j != c->idCol)
                {
                    if (cells[c->idRow][j].nbrPossible == 2)
                    {
                        if (cells[c->idRow][j].possible[k1] && cells[c->idRow][j].possible[k2])
                        {
                            std::vector<Cell *> rowPair = getRowWithoutPair(c->idRow, c->idCol, j);
                            update(k1, rowPair);
                            update(k2, rowPair);
                            break;
                        }
                    }
                }
            } /*
            //square
            int i0 = int(c->idRow/3), j0 = int(c->idCol/3);
            for(int i=3*i0; i<3*(i0+1);i++){
                for(int j=3*j0; j<3*(j0+1);j++){
                    if(j != c->idCol){
                        if(i != c->idRow){
                            if(cells[i][j].nbrPossible == 2){
                                if(cells[i][j].possible[k1] && cells[i][j].possible[k2]){
                                    std::vector<Cell*> squarePair = getSquareWithoutPair(i0, j0, c->idRow, c->idCol, i, j);
                                    update(k1, squarePair);
                                    update(k2, squarePair);
                                    break;
                                }
                            }
                        }
                    }
                }
            }*/
        }
    }
}

std::vector<Cell *> Matrix::getSquareWithoutPair(int i0, int j0, int idRow, int idCol, int i1, int j1)
{
    std::vector<Cell *> cellsUpdated;
    for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
        for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
            if ((j != idCol) || (i != idRow))
                if ((j != j1) || (i != i1))
                    if (cells[i][j].isFree())
                        cellsUpdated.push_back(&cells[i][j]);

    return cellsUpdated;
}

std::vector<Cell *> Matrix::getRowWithoutPair(int iRow, int j1, int j2)
{
    std::vector<Cell *> cellsUpdated;
    for (int j = 0; j < 9; j++)
        if (j != j1 && j != j2 && cells[iRow][j].isFree())
            cellsUpdated.push_back(&cells[iRow][j]);

    return cellsUpdated;
}
std::vector<Cell *> Matrix::getColWithoutPair(int jCol, int i1, int i2)
{
    std::vector<Cell *> cellsUpdated;
    for (int i = 0; i < 9; i++)
        if (i != i1 && i != i2 && cells[i][jCol].isFree())
            cellsUpdated.push_back(&cells[i][jCol]);

    return cellsUpdated;
}

std::vector<Cell *> Matrix::getAllNeighbors(int iRow, int jCol)
{
    std::vector<Cell *> cellsUpdated;
    int i0 = int(iRow / 3), j0 = int(jCol / 3);
    for (int k = 0; k < 9; k++)
        if (cells[iRow][k].isFree())
            cellsUpdated.push_back(&cells[iRow][k]); //row

    for (int k = 0; k < 9; k++)
        if (cells[k][jCol].isFree())
            cellsUpdated.push_back(&cells[k][jCol]); //col

    for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
        for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
            if (cells[i][j].isFree())
                cellsUpdated.push_back(&cells[i][j]); //square

    return cellsUpdated;
}

std::vector<Cell *> Matrix::getSquareLockedRow(int i0, int j0, int iRow)
{
    std::vector<Cell *> cellsUpdated;
    for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
        if (i != iRow)
            for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
                if (cells[i][j].isFree())
                    cellsUpdated.push_back(&cells[i][j]); //square

    return cellsUpdated;
}
std::vector<Cell *> Matrix::getSquareLockedCol(int i0, int j0, int jCol)
{
    std::vector<Cell *> cellsUpdated;
    for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
        if (j != jCol)
            for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
                if (cells[i][j].isFree())
                    cellsUpdated.push_back(&cells[i][j]); //square

    return cellsUpdated;
}

std::vector<Cell *> Matrix::getRowLockedSquare(int i0, int j0, int idHoriz)
{
    std::vector<Cell *> cellsUpdated;
    int iRow = 3 * i0 + idHoriz;
    for (int j = 0; j < 9; j++)
        if (int(j / 3) != j0)
            if (cells[iRow][j].isFree())
                cellsUpdated.push_back(&cells[iRow][j]); //row

    return cellsUpdated;
}

std::vector<Cell *> Matrix::getColLockedSquare(int i0, int j0, int idVert)
{
    std::vector<Cell *> cellsUpdated;
    int jCol = 3 * j0 + idVert;
    for (int i = 0; i < 9; i++)
        if (int(i / 3) != i0)
            if (cells[i][jCol].isFree())
                cellsUpdated.push_back(&cells[i][jCol]); //col

    return cellsUpdated;
}

void Matrix::save()
{
    // value + 1 because the value is shifted to 0:8
    std::ofstream file("sudokuSolved.csv");
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            file << cells[i][j].value + 1 << ";";
        }
        file << cells[i][8].value + 1 << "\n";
    }
    file.close();
}
void Matrix::disp()
{
    // value + 1 because the value is shifted to 0:8
    std::cout << " ______ _______ _______\n";
    for (int i0 = 0; i0 < 3; i0++)
    {
        for (int i = 3 * i0; i < 3 * (i0 + 1); i++)
        {
            std::cout << "|";
            for (int j0 = 0; j0 < 3; j0++)
            {
                for (int j = 3 * j0; j < 3 * (j0 + 1); j++)
                {
                    int val = cells[i][j].value + 1;
                    if (val == 0)
                        std::cout << " ";
                    else
                        std::cout << val;
                    if (j == 3 * j0 + 2)
                        std::cout << " | ";
                    else
                        std::cout << " ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "______ _______ _______\n";
    }
    std::cout << "\n\n";
}

void Matrix::load(std::string name)
{
    std::ifstream file(name);
    std::string line;
    for (int i = 0; i < 9; i++)
    {
        std::getline(file, line);
        std::stringstream ss(line);
        std::string field;
        for (int j = 0; j < 9; j++)
        {
            std::getline(ss, field, ';');
            std::stringstream fs(field);
            int f = 0;
            fs >> f;
            if (f != 0)
            {
                initCells.push_back(CellToAdd(i, j, f - 1));
                // f-1 because the value is shifted to 0:8
            }
        }
    }
}

void Matrix::solve()
{
    for (auto c : initCells)
    {
        int i = c.iRow, j = c.jCol;
        int i0 = cells[i][j].idRowBlock, j0 = cells[i][j].idColBlock, val = c.val;

        cells[i][j].setValue(val);
        rows[i].setValue(val);        //row
        cols[j].setValue(val);        //col
        blocks[i0][j0].setValue(val); //square
        eraseRemainingCandidates(i, j);

        update(val, getAllNeighbors(c.iRow, c.jCol));
    }
}

void Matrix::eraseRemainingCandidates(int i, int j)
{
    // if there are still possible candidates when we set the value.
    // if the choice of the value is caused by a line or a square,
    // the set of candidates is not reduced to a single value.
    for (int k = 0; k < 9; k++)
    {
        if (cells[i][j].possible[k])
        {
            std::vector<Cell *> cellToAddInit;
            cellToAddInit.push_back(&cells[i][j]);
            update(k, cellToAddInit);
        }
    }
}

void Matrix::doColoring(int val)
{
    int coloring = 1;
    std::vector<std::vector<int>> coloringPaths(9, std::vector<int>(9));
    std::vector<int> longPathsColour;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (coloringPaths[i][j] == 0 && cells[i][j].possible[val])
            {
                bool realPath = false;
                int lengthPath = 0;
                spreadColoringPath(lengthPath, realPath, i, j, val, coloring, coloring, coloringPaths);
                if (realPath)
                {
                    if (lengthPath > 3)
                        longPathsColour.push_back(coloring);

                    //when we started with this cell, it wasn't sure that it was a real path
                    coloringPaths[i][j] = coloring;
                    coloring += 2;
                }
            }
        }
    }
    std::vector<CellToAdd> cellsToAdd;
    for (int colour : longPathsColour)
    {
        bool valid1 = true;
        for (int i = 0; i < 9; i++)
        {
            int countRow = 0, countCol = 0;
            for (int j = 0; j < 9; j++)
            {
                if (coloringPaths[i][j] == colour)
                    countRow++;
                if (coloringPaths[j][i] == colour)
                    countCol++;
            }
            if (countRow > 1 || countCol > 1)
                valid1 = false;
        }
        bool valid2 = true;
        if (valid1)
        {
            for (int i = 0; i < 9; i++)
            {
                int countRow = 0, countCol = 0;
                for (int j = 0; j < 9; j++)
                {
                    if (coloringPaths[i][j] == colour + 1)
                        countRow++;
                    if (coloringPaths[j][i] == colour + 1)
                        countCol++;
                }
                if (countRow > 1 || countCol > 1)
                    valid2 = false;
            }
        }
        if (!valid1 || !valid2)
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    if ((coloringPaths[i][j] == colour && valid1) || (coloringPaths[i][j] == colour + 1 && valid2))
                    {
                        cellsToAdd.push_back(CellToAdd(i, j, val));
                    }
                }
            }
        }
    }

    for (auto c : cellsToAdd)
    {
        int i = c.iRow, j = c.jCol;
        int i0 = int(i / 3), j0 = int(j / 3);
        setValue(c);
        rows[i].setValue(c.val);
        cols[j].setValue(c.val);
        blocks[i0][j0].setValue(c.val);
        eraseRemainingCandidates(i, j);
        update(c.val, getAllNeighbors(c.iRow, c.jCol));
    }

    /*
std::cout << "Coloring for " << val+1 <<"\n";


    for(int i = 0; i < 9; i++){
    std::cout << "| ";
    for(int j0 = 0; j0<3;j0++){
        if(coloringPaths[i][3*j0] ==0)
             std::cout << "  ";
        else
            std::cout << coloringPaths[i][3*j0] << " ";
        if(coloringPaths[i][3*j0+1]==0)
             std::cout << "  ";
        else
            std::cout << coloringPaths[i][3*j0+1] << " ";
        if(coloringPaths[i][3*j0+2] ==0)
             std::cout << " | ";
        else
            std::cout << coloringPaths[i][3*j0+2] << "| ";
    }
    std::cout << "\n";
    if( int((i+1)/3)*3 == i+1)
        std::cout << "______ _______ _______\n";
    }
    std::cout << "\n\n";

    */
}

void Matrix::spreadColoringPath(int &lengthPath, bool &realPath, int iRow, int jCol, int val, int initColour, int currentColour, std::vector<std::vector<int>> &coloringPaths)
{
    std::vector<std::pair<int, int>> coloringNeighbors;

    if (getColoringNeighbors(iRow, jCol, val, coloringNeighbors))
    {
        coloringPaths[iRow][jCol] = currentColour;
        currentColour = (currentColour == initColour ? initColour + 1 : initColour);
        for (auto c : coloringNeighbors)
        {
            if (coloringPaths[c.first][c.second] == 0)
            {
                realPath = true;
                lengthPath++;
                spreadColoringPath(lengthPath, realPath, c.first, c.second, val, initColour, currentColour, coloringPaths);
            }
        }
    }
}

bool Matrix::getColoringNeighbors(int iRow, int jCol, int val, std::vector<std::pair<int, int>> &coloringNeighbors)
{
    //rows
    if (rows[iRow].getTotalNbrPossible(val) == 2)
        for (int j = 0; j < 9; j++)
        {
            if (j != jCol && cells[iRow][j].possible[val])
            {
                coloringNeighbors.push_back(std::make_pair(iRow, j));
            }
        }
    //cols
    if (cols[jCol].getTotalNbrPossible(val) == 2)
        for (int i = 0; i < 9; i++)
        {
            if (i != iRow && cells[i][jCol].possible[val])
            {
                coloringNeighbors.push_back(std::make_pair(i, jCol));
            }
        }
    //block
    int iBlock = int(iRow / 3), jBlock = int(jCol / 3);
    if (blocks[iBlock][jBlock].getTotalNbrPossible(val) == 2)
        for (int i = 3 * iBlock; i < 3 * (iBlock + 1); i++)
        {
            for (int j = 3 * jBlock; j < 3 * (jBlock + 1); j++)
            {
                if ((j != jCol || i != iRow) && cells[i][j].possible[val])
                {
                    coloringNeighbors.push_back(std::make_pair(i, j));
                }
            }
        }
    return coloringNeighbors.size() > 0;
}

void Matrix::dispAllLinesPossible()
{
    dispAllLinesPossible(rows, "Rows");
    dispAllLinesPossible(cols, "Cols");
}

void Matrix::dispAllLinesPossible(std::array<Line, 9> &lines, std::string name)
{
    std::cout << name << "\n";
    std::cout << "|";
    for (int val = 1; val < 10; val++)
    {
        std::cout << "   " << val << "   |";
    }
    std::cout << "\n";

    for (int idLine = 0; idLine < 9; idLine++)
    {
        std::cout << "| ";
        auto poss = lines[idLine].nbrPossible;
        for (int val = 0; val < 9; val++)
        {
            if (poss[val][0] == -1)
                std::cout << "      | ";
            else
            {
                std::cout << poss[val][0] << ",";
                std::cout << poss[val][1] << ",";
                std::cout << poss[val][2] << " | ";
            }
        }
        std::cout << "\n";
    }
}

void Matrix::dispAllSquaresPossible()
{
    std::cout << "Squares Horiz\n";
    std::cout << "|";
    for (int val = 1; val < 10; val++)
    {
        std::cout << "   " << val << "   |";
    }
    std::cout << "\n";

    for (int i0 = 0; i0 < 3; i0++)
    {
        for (int j0 = 0; j0 < 3; j0++)
        {
            std::cout << "| ";
            auto poss = blocks[i0][j0].nbrPossibleHoriz;
            for (int val = 0; val < 9; val++)
            {
                if (poss[val][0] == -1)
                    std::cout << " ,";
                else
                    std::cout << poss[val][0] << ",";

                if (poss[val][1] == -1)
                    std::cout << " ,";
                else
                    std::cout << poss[val][1] << ",";

                if (poss[val][2] == -1)
                    std::cout << "  | ";
                else
                    std::cout << poss[val][2] << " | ";
            }
            std::cout << "\n";
        }
    }
    std::cout << "Squares Vert\n";
    std::cout << "|";
    for (int val = 1; val < 10; val++)
    {
        std::cout << "   " << val << "   |";
    }
    std::cout << "\n";

    for (int i0 = 0; i0 < 3; i0++)
    {
        for (int j0 = 0; j0 < 3; j0++)
        {
            std::cout << "| ";
            auto poss = blocks[i0][j0].nbrPossibleVert;
            for (int val = 0; val < 9; val++)
            {
                if (poss[val][0] == -1)
                    std::cout << " ,";
                else
                    std::cout << poss[val][0] << ",";

                if (poss[val][1] == -1)
                    std::cout << " ,";
                else
                    std::cout << poss[val][1] << ",";

                if (poss[val][2] == -1)
                    std::cout << "  | ";
                else
                    std::cout << poss[val][2] << " | ";
            }
            std::cout << "\n";
        }
    }
}

void Matrix::dispAllNbrPossible()
{
    std::cout << "Nbr of possibilities per cell (cell.nbrPossible)"
              << "\n";

    for (int i = 0; i < 9; i++)
    {
        std::cout << "| ";
        for (int j0 = 0; j0 < 3; j0++)
        {
            if (cells[i][3 * j0].nbrPossible <= 0)
                std::cout << "  ";
            else
                std::cout << cells[i][3 * j0].nbrPossible << " ";
            if (cells[i][3 * j0 + 1].nbrPossible <= 0)
                std::cout << "  ";
            else
                std::cout << cells[i][3 * j0 + 1].nbrPossible << " ";
            if (cells[i][3 * j0 + 2].nbrPossible <= 0)
                std::cout << " | ";
            else
                std::cout << cells[i][3 * j0 + 2].nbrPossible << "| ";
        }
        std::cout << "\n";
        if (int((i + 1) / 3) * 3 == i + 1)
            std::cout << "______ _______ _______\n";
    }
    std::cout << "\n\n"; /*
    std::cout << "Nbr of possibilities per cell (sum of cell.possible)" << "\n";
    for(int i = 0; i < 9; i++){
        std::cout << "| ";
        for(int j0 = 0; j0<3;j0++){
            auto v = cells[i][3*j0].possible;
            int k = std::count(v.begin(), v.end(), true);
            if(k==0)
                std::cout << "  ";
            else
                std::cout << k << " ";
            v = cells[i][3*j0+1].possible;
            k = std::count(v.begin(), v.end(), true);
            if(k==0)
                std::cout << "  ";
            else
                std::cout << k << " ";
            v = cells[i][3*j0+2].possible;
            k = std::count(v.begin(), v.end(), true);
            if(k==0)
                std::cout << " | ";
            else
                std::cout << k << "| ";
        }
        std::cout << "\n";
        if( int((i+1)/3)*3 == i+1)
            std::cout << "______ _______ _______\n";
    }*/
}

void Matrix::dispGridVal(int val)
{
    std::cout << "Possible cases for the value " << val << "\nand total number of candidates in the cell.\n";
    val--;

    for (int i = 0; i < 9; i++)
    {
        std::cout << "| ";
        for (int j0 = 0; j0 < 3; j0++)
        {
            if (!cells[i][3 * j0].possible[val])
                std::cout << "  ";
            else
                std::cout << cells[i][3 * j0].nbrPossible << " ";
            if (!cells[i][3 * j0 + 1].possible[val])
                std::cout << "  ";
            else
                std::cout << cells[i][3 * j0 + 1].nbrPossible << " ";
            if (!cells[i][3 * j0 + 2].possible[val])
                std::cout << " | ";
            else
                std::cout << cells[i][3 * j0 + 2].nbrPossible << "| ";
        }
        std::cout << "\n";
        if (int((i + 1) / 3) * 3 == i + 1)
            std::cout << "______ _______ _______\n";
    }
    std::cout << "\n\n";
}
