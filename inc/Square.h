#ifndef SQUARE_H
#define SQUARE_H

#include <string>
#include <iostream>
#include <array>
#include <vector>

class Square
{
    public:

        enum TO_DO { NOTHING, SETVALUE,
			LOCK_ROW0, LOCK_ROW1, LOCK_ROW2,
			LOCK_COL0, LOCK_COL1, LOCK_COL2,
			 };

        Square();
        virtual ~Square();

		std::array<std::array<int, 3>, 9> nbrPossibleHoriz;
        std::array<std::array<int, 3>, 9> nbrPossibleVert;
        int idRowBlock, idColBlock;

		int getTotalNbrPossible(int val){return nbrPossibleHoriz[val][0]+nbrPossibleHoriz[val][1]+nbrPossibleHoriz[val][2];}


		void dispSquarePossible(int valFromOneToNine);
        TO_DO decreaseNbr(int val, int i0, int j0, int idRow, int idCol);
        void decreaseHorizNbrInit(int val, int idHoriz);
        void decreaseVertNbrInit(int val, int idVert);
        void setValue(int val);

};

#endif // SQUARE_H
