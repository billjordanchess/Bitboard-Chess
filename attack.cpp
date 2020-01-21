#include "globals.h"

/* 

Attack returns true if one side attacks a given square and false if it doesn't.
It is used to tell if a King is in check, but can have other uses.

*/
bool Attack(const int s, const int sq)
{
if(bit_pawndefends[s][sq] & bit_pieces[s][P])
	return true;
if(bit_knightmoves[sq] & bit_pieces[s][N])
	return true;

int sq2;
BITBOARD b1 = bit_rookmoves[sq] & (bit_pieces[s][R] | bit_pieces[s][Q]);
b1 |= (bit_bishopmoves[sq] & (bit_pieces[s][B] | bit_pieces[s][Q]));

while(b1)
{
	sq2 = NextBit(b1);
	if(!(bit_between[sq2][sq] & bit_all))
		return true;
	b1 &= not_mask[sq2];
}

if(bit_kingmoves[sq] & bit_pieces[s][K])
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
if(bit_left[xs][sq] & bit_pieces[s][P])
	return pawnleft[xs][sq];
if(bit_right[xs][sq] & bit_pieces[s][P])
	return pawnright[xs][sq];

U64 b1;
b1 = bit_knightmoves[sq] & bit_pieces[s][N];
if(b1)
	return NextBit(b1);

b1 = bit_bishopmoves[sq] & bit_pieces[s][B];
while(b1)
{
	int sq2 = NextBit(b1);
    if(!(bit_between[sq2][sq] & bit_all))
	  return sq2;
    b1 &= not_mask[sq2];
}
b1 = bit_rookmoves[sq] & bit_pieces[s][R];
while(b1)
{
	int sq2 = NextBit(b1);
    if(!(bit_between[sq2][sq] & bit_all))
	  return sq2;
    b1 &= not_mask[sq2];
}
b1 = bit_queenmoves[sq] & bit_pieces[s][Q];
while(b1)
{
	int sq2 = NextBit(b1);
    if(!(bit_between[sq2][sq] & bit_all))
	  return sq2;
    b1 &= not_mask[sq2];
}
return -1;
}

