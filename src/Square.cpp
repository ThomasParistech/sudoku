#include "Square.h"

Square::Square()
{
    //ctor
    for(int i=0; i<9;i++){
        for(int j=0; j<3;j++){
            nbrPossibleHoriz[i][j] = 3;
            nbrPossibleVert[i][j] = 3;
        }
    }

}

Square::~Square()
{
    //dtor
}

int getIFullSquare(std::array<int,3> nbrPossibleBlock, int idBlock){
	int iEmpty = -1, iFull = -1;

	for(int i=0; i<3;i++){
		if(i != idBlock){
			if(nbrPossibleBlock[i] == 0)
				iEmpty = i;
			else
				iFull = i;
		}
	}

	if(iEmpty != -1)	//if there is another empty
		return iFull;
	return -1;
}

void Square::dispSquarePossible(int valFromOneToNine){
    int val = valFromOneToNine-1;
    std::cout << "Square" << "(" << idRowBlock << ","<< idColBlock << "), value = " << valFromOneToNine << ":\n";
    std::cout << "   Horiz : " << nbrPossibleHoriz[val][0] << "/" << nbrPossibleHoriz[val][1] << "/" << nbrPossibleHoriz[val][2] << "\n";
    std::cout << "    Vert : " << nbrPossibleVert[val][0] << "/" << nbrPossibleVert[val][1] << "/" << nbrPossibleVert[val][2] << "\n";
}

Square::TO_DO Square::decreaseNbr(int val, int i0, int j0, int idRow, int idCol){  // updates and looks for locked cells
    int idHoriz = idRow - 3*i0;
	int idVert = idCol - 3*j0;
    decreaseHorizNbrInit(val, idHoriz);
	decreaseVertNbrInit(val, idVert);


    if(getTotalNbrPossible(val) == 1){
        return SETVALUE;
    }
    else if (nbrPossibleHoriz[val][idHoriz] == 0){
		switch(getIFullSquare(nbrPossibleHoriz[val],idHoriz)) {
			case 0 : return LOCK_ROW0;
			case 1 : return LOCK_ROW1;
			case 2 : return LOCK_ROW2;
		}
    }
	else if (nbrPossibleVert[val][idVert] == 0){
		switch(getIFullSquare(nbrPossibleVert[val],idVert)) {
			case 0 : return LOCK_COL0;
			case 1 : return LOCK_COL1;
			case 2 : return LOCK_COL2;
		}
    }
    return NOTHING;
}


 void Square::decreaseHorizNbrInit(int val, int idHoriz){ //  int idHoriz = idRow - 3*i0;
	 nbrPossibleHoriz[val][idHoriz]--;
 }
 void Square::decreaseVertNbrInit(int val, int idVert){ //int idVert = idCol - 3*j0;
	 nbrPossibleVert[val][idVert]--;
 }

 void Square::setValue(int val){
    for(int j=0; j<3;j++){
        nbrPossibleHoriz[val][j] = -1;
        nbrPossibleVert[val][j] = -1;
    }
 }







