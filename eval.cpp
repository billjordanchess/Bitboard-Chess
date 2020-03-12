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
int sq;

for(int x=0;x<2;x++)
{
	b1 = bit_pieces[x][P];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[x] += square_score[x][P][sq];
		score[x] += EvalPawn(x,sq);
	}
	b1 = bit_pieces[x][N];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[x] += square_score[x][N][sq];
	}
	b1 = bit_pieces[x][B];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[x] += square_score[x][B][sq];
	}
	b1 = bit_pieces[x][R];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[x] += square_score[x][R][sq];
		score[x] += EvalRook(x,sq);
	}
	b1 = bit_pieces[x][Q];
	while(b1)
	{
		sq = NextBit(b1);
		b1 &= not_mask[sq];
		score[x] += square_score[x][Q][sq];
	}
}
  if(bit_pieces[1][Q]==0)
    score[0] += king_endgame[0][NextBit(bit_pieces[0][K])];
  else
  {
	if(bit_pieces[0][K] & mask_kingside)
		score[0] += kingside_pawns[0];
	else if(bit_pieces[0][K] & mask_queenside)
		score[0] += queenside_pawns[0];
  }

  if(bit_pieces[0][Q]==0)
    score[1] += king_endgame[1][NextBit(bit_pieces[1][K])];
   else
  {
	if(bit_pieces[1][K] & mask_kingside)
		score[1] += kingside_pawns[1];
	else if(bit_pieces[1][K] & mask_queenside)
		score[1] += queenside_pawns[1];
  }

  return score[side] - score[xside];
}
/*

EvalPawn() evaluates each pawn and gives a bonus for passed pawns
and a minus for isolated pawns.

*/
int EvalPawn(const int s,const int sq)
{
int score = 0;
int xs = s^1;

if(!(mask_passed[s][sq] & bit_pieces[xs][P]) && !(mask_path[s][sq] & bit_pieces[s][P]))
{
	score += passed[s][sq];
}
if((mask_isolated[sq] & bit_pieces[s][P])==0)
	score -= ISOLATED;
kingside_pawns[s] += kingside_defence[s][sq];
queenside_pawns[s] += queenside_defence[s][sq];

return score;
}
/*

EvalRook() evaluates each rook and gives a bonus for being
on an open file or half-open file.
*/
int EvalRook(const int s,const int sq)
{
int score = 0;
if(!(mask_cols[sq] & bit_pieces[s][P]))
{
	score = 10;
	if(!(mask_cols[sq] & bit_pieces[s^1][P]))
		score = 20;
}
return score;
}
