#ifndef OLD_CELL_H
#define OLD_CELL_H

#include <string>
#include <iostream>
#include <array>
#include <vector>

class Cell
{
public:
    friend std::ostream &operator<<(std::ostream &ostr, const Cell &cell);

    enum TO_DO
    {
        NOTHING,
        SETVALUE,
        PAIR
    };

    Cell();
    ~Cell() = default;

    int findNew();
    TO_DO decreaseNbr(int val);
    void setValue(int val);

    bool isFree();

    int value;
    int nbrPossible;

    int idRow, idCol;           //0,1,2,3,...7,8
    int idRowBlock, idColBlock; //0,1,2

    std::array<bool, 9> possible; // 9 possible values: 0,1, ... 8

private:
};

inline std::ostream &operator<<(std::ostream &ostr, const Cell &cell)
{
    ostr << "Cell(" << cell.idRow << "," << cell.idCol << ") : ";
    for (int i = 0; i < 9; i++)
    {
        if (cell.possible[i])
            ostr << i + 1 << "/";
    }
    return ostr;
}

#endif // OLD_CELL_H
