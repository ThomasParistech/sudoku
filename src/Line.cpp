#include "Line.h"

Line::Line()
{
    //ctor
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 3; j++)
            nbrPossible[i][j] = 3;
}

/// Add a constraint on a given value on one of the three blocks of the line
Line::TO_DO Line::decreaseNbr(int val, int idBlock)
{ // updates and looks for locked cells
    nbrPossible[val][idBlock]--;

    if (getTotalNbrPossible(val) == 1)
        return SETVALUE;

    // If this block can't contain the value
    if (nbrPossible[val][idBlock] == 0)
    {
        int iEmpty = -1, iFull = -1;

        for (int i = 0; i < 3; i++)
        {
            if (i != idBlock)
            {
                if (nbrPossible[val][i] == 0)
                    iEmpty = i;
                else
                    iFull = i;
            }
        }
        // If the 2 other blocks have a possibility, then iEmpty = -1
        // But if there's only ONE that has the possibility (if there is another empty), it's iFull
        if (iEmpty != -1)
        {
            // Lock the only one that contains the val
            switch (iFull)
            {
            case 0:
                return LOCK_SQUARE0;
            case 1:
                return LOCK_SQUARE1;
            case 2:
                return LOCK_SQUARE2;
            }
        }
    }

    return NOTHING;
}

void Line::setValue(int val)
{
    for (int j = 0; j < 3; j++)
        nbrPossible[val][j] = -1;
}

void Line::dispLinePossible(int valFromOneToNine)
{
    const int val = valFromOneToNine - 1;

    std::cout << (isRow ? "Row" : "Column")
              << "(" << idLine << "), value = " << valFromOneToNine << ": ";
    std::cout << nbrPossible[val][0] << "/" << nbrPossible[val][1] << "/" << nbrPossible[val][2] << "\n";
}
