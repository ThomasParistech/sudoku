#include "Cell.h"

Cell::Cell()
{
    //ctor
    value = -1; // between 0 and 8. it's shifted
    nbrPossible = 9;
    possible.fill(true);
}

int Cell::findNew()
{ // get the value to set
    for (int val = 0; val < 9; val++)
        if (possible[val])
            return val;

    return -1;
}

Cell::TO_DO Cell::decreaseNbr(int val)
{ // removes val from the candidates and returns true if there's only one candidate left
    possible[val] = false;
    nbrPossible--;
    if (nbrPossible == 1)
        return SETVALUE;
    else if (nbrPossible == 2)
        return PAIR;

    return NOTHING;
}

void Cell::setValue(int val)
{ // set value
    value = val;
    nbrPossible = 0;
}

bool Cell::isFree()
{
    return nbrPossible >= 2;
}
