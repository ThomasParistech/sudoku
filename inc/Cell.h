#ifndef CELL_H
#define CELL_H

#include <string>
#include <iostream>
#include <array>
#include <vector>

class Cell
{
public:
    Cell();
    virtual ~Cell();

    enum TO_DO
    {
        NOTHING,
        SETVALUE,
        PAIR
    };

    int value;
    int nbrPossible;

    int idRow, idCol;           //0,1,2,3,...7,8
    int idRowBlock, idColBlock; //0,1,2

    std::array<bool, 9> possible;

    void dispPossible();
    int findNew();
    TO_DO decreaseNbr(int val);
    void setValue(int val);
    void init(int val);

    bool isFree();
};

#endif // CELL_H
