#include "globals.h"

#define ISOLATED 20

int EvalPawn(const int s,const int x);
int EvalRook(const int s,const int x);
int queenside_pawns[2],kingside_pawns[2];

extern U64 mask_kingside;
extern U64 mask_queenside;

const int queenside_defence[2][64]=
{
{
	0, 0, 0, 0, 0, 0, 0, 0,
	8,10, 8, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
},
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 0, 0, 0,
	8,10, 8, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
}};

const int kingside_defence[2][64]=
{
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8,10, 8,
	0, 0, 0, 0, 0, 8, 6, 8,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	8, 6, 8, 0, 0, 8, 8, 8,
	0, 0, 0, 0, 0, 0, 0, 0
},
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8, 6, 8,
	0, 0, 0, 0, 0, 8,10, 8,
	0, 0, 0, 0, 0, 0, 0, 0
}};
/*

Eval() is simple. Firstly it adds the square scores for each piece of both sides.
If the opponent does not have a queen it adds the endgame score for each king.
If the opponent has a queen it adds the pawn defence score for each king.
It turn returns the side to moves score minus the opponent's score.
There are plenty of things that could be added to the eval function.

*/
int Eval()
{
int score[2] = {0,0};

queenside_pawns[0] = 0;
queenside_pawns[1] = 0;
kingside_pawns[0] = 0;
kingside_pawns[1] = 0;

U64 b1;
int i;

for(int x=0;x<2;x++)
{
	b1 = bit_pieces[x][0];
	while(b1)
	{
		i = NextBit(b1);
		b1 &= not_mask[i];
		score[x] += square_score[x][0][i];
		score[x] += EvalPawn(x,i);
	}
	b1 = bit_pieces[x][1];
	while(b1)
	{
		i = NextBit(b1);
		b1 &= not_mask[i];
		score[x] += square_score[x][1][i];
	}
	b1 = bit_pieces[x][2];
	while(b1)
	{
		i = NextBit(b1);
		b1 &= not_mask[i];
		score[x] += square_score[x][2][i];
	}
	b1 = bit_pieces[x][3];
	while(b1)
	{
		i = NextBit(b1);
		b1 &= not_mask[i];
		score[x] += square_score[x][3][i];
		score[x] += EvalRook(x,i);
	}
	b1 = bit_pieces[x][4];
	while(b1)
	{
		i = NextBit(b1);
		b1 &= not_mask[i];
		score[x] += square_score[x][4][i];
	}
}
  if(bit_pieces[1][4]==0)
    score[0] += king_endgame[0][NextBit(bit_pieces[0][5])];
  else
  {
	if(bit_pieces[0][5] & mask_kingside)
		score[0] += kingside_pawns[0];
	else if(bit_pieces[0][5] & mask_queenside)
		score[0] += queenside_pawns[0];
  }

  if(bit_pieces[0][4]==0)
    score[1] += king_endgame[1][NextBit(bit_pieces[0][5])];
   else
  {
	if(bit_pieces[1][5] & mask_kingside)
		score[1] += kingside_pawns[1];
	else if(bit_pieces[1][5] & mask_queenside)
		score[1] += queenside_pawns[1];
  }

  return score[side] - score[xside];
}
/*

EvalPawn() evaluates each pawn and gives a bonus for passed pawns
and a minus for isolated pawns.

*/
int EvalPawn(const int s,const int x)
{
int score = 0;
int xs = OtherSide[s];

if(!(mask_passed[s][x] & bit_pieces[xs][0]) && !(mask_path[s][x] & bit_pieces[s][0]))
{
	score += passed[s][x];
}
if((mask_isolated[x] & bit_pieces[s][0])==0)
	score -= ISOLATED;
kingside_pawns[s] += kingside_defence[s][x];
queenside_pawns[s] += queenside_defence[s][x];

return score;
}
/*

EvalRook() evaluates each rook and gives a bonus for being
on an open file or half-open file.
*/
int EvalRook(const int s,const int x)
{
int score = 0;
if(!(mask_cols[x] & bit_pieces[s][0]))
{
	score = 10;
	if(!(mask_cols[x] & bit_pieces[OtherSide[s]][0]))
		score = 20;
}
return score;
}
