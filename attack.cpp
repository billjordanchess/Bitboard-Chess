#include "globals.h"

/* 

Attack returns true if one side attacks a given square and false if it doesn't.
It is used to tell if a King is in check, but can have other uses.

*/
bool Attack(const int s, const int sq)
{
if(bit_pawndefends[s][sq] & bit_pieces[s][0])
{
	//DisplayBoard();
	return true;
}
if(bit_knightmoves[sq] & bit_pieces[s][1])
	return true;

int i;
BITBOARD b1 = bit_rookmoves[sq] & (bit_pieces[s][3] | bit_pieces[s][4]);
b1 |= (bit_bishopmoves[sq] & (bit_pieces[s][2] | bit_pieces[s][4]));

while(b1)
{
	i = NextBit(b1);
	if(!(bit_between[i][sq] & bit_all))
		return true;
	b1 &= not_mask[i];
}

if(bit_kingmoves[sq] & bit_pieces[s][5])
	return true;
return false;
}
/*
 
LowestAttacker is similar to Attack. It returns the square the weakest attacker of the given side and given square.
It returns -1 if there are no attackers.
It is used to find the next piece that will recapture, but can have other uses.

*/
int LowestAttacker(const int s, const int xs,const int sq)
{
if(bit_left[xs][sq] & bit_pieces[s][0])
	return pawnleft[xs][sq];
if(bit_right[xs][sq] & bit_pieces[s][0])
	return pawnright[xs][sq];

U64 b1;
b1 = bit_knightmoves[sq] & bit_pieces[s][1];
if(b1)
	return NextBit(b1);

b1 = bit_bishopmoves[sq] & bit_pieces[s][2];
while(b1)
{
	int i = NextBit(b1);
    if(!(bit_between[i][sq] & bit_all))
	  return i;
    b1 &= not_mask[i];
}
b1 = bit_rookmoves[sq] & bit_pieces[s][3];
while(b1)
{
	int i = NextBit(b1);
    if(!(bit_between[i][sq] & bit_all))
	  return i;
    b1 &= not_mask[i];
}
b1 = bit_queenmoves[sq] & bit_pieces[s][4];
while(b1)
{
	int i = NextBit(b1);
    if(!(bit_between[i][sq] & bit_all))
	  return i;
    b1 &= not_mask[i];
}
return -1;
}

