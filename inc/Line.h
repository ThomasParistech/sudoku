#ifndef LINE_H
#define LINE_H

#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <string>
class Line
{
public:
    enum TO_DO
    {
        NOTHING,
        SETVALUE,
        LOCK_SQUARE0,
        LOCK_SQUARE1,
        LOCK_SQUARE2
    };

    Line();
    ~Line() = default;

    std::array<std::array<int, 3>, 9> nbrPossible; // For each value:
                                                   // number of possibilties per block

    int idLine; // 0,1, ... 8
    bool isRow;

    int getTotalNbrPossible(int val) { return nbrPossible[val][0] + nbrPossible[val][1] + nbrPossible[val][2]; }

    void dispLinePossible(int val);
    TO_DO decreaseNbr(int val, int idBlock);

    void setValue(int val);

private:
};

#endif // LINE_H
