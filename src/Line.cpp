#include "Line.h"

Line::Line()
{
    //ctor
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            nbrPossible[i][j] = 3;
        }
    }
}

Line::~Line()
{
    //dtor
}

int getIFullLine(std::array<int, 3> nbrPossibleBlock, int idBlock)
{
    int iEmpty = -1, iFull = -1;

    for (int i = 0; i < 3; i++)
    {
        if (i != idBlock)
        {
            if (nbrPossibleBlock[i] == 0)
                iEmpty = i;
            else
                iFull = i;
        }
    }

    if (iEmpty != -1) //if there is another empty
        return iFull;
    return -1;
}

Line::TO_DO Line::decreaseNbr(int val, int idBlock)
{ // updates and looks for locked cells
    decreaseNbrInit(val, idBlock);
    if (getTotalNbrPossible(val) == 1)
    {
        return SETVALUE;
    }
    if (nbrPossible[val][idBlock] == 0)
    {
        switch (getIFullLine(nbrPossible[val], idBlock))
        {
        case 0:
            return LOCK_SQUARE0;
        case 1:
            return LOCK_SQUARE1;
        case 2:
            return LOCK_SQUARE2;
        }
    }

    return NOTHING;
}
void Line::decreaseNbrInit(int val, int idBlock)
{
    nbrPossible[val][idBlock]--;
}

void Line::setValue(int val)
{
    for (int j = 0; j < 3; j++)
    {
        nbrPossible[val][j] = -1;
    }
}

void Line::dispLinePossible(int valFromOneToNine, std::string name)
{
    int val = valFromOneToNine - 1;
    std::cout << name << "(" << idLine << "), value = " << valFromOneToNine << ": ";
    std::cout << nbrPossible[val][0] << "/" << nbrPossible[val][1] << "/" << nbrPossible[val][2] << "\n";
}
