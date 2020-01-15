#include "globals.h"

int px[6] = {0,10,20,30,40,0};
int nx[6] = {-3,7,17,27,37,0};
int bx[6] = {-3,7,17,27,37,0};
int rx[6] = {-5,5,15,25,35,0};
int qx[6] = {-9,1,11,21,31,0};
int kx[6] = {0,10,20,30,40,0};

int ForwardSquare[2] = {8,-8};
int Double[2] = {16,-16};
int OtherSide[2] = {1,0};

int mc;
int castle;

void Gen(const int s,const int sx);
void AddMove(const int x,const int sq);
void AddCapture(const int x,const int sq,const int score);

/*

Gen sees if an en passant capture or castling is possible.
It then loops through the board searching for pieces of one 
side and generates
moves for them.

*/

void Gen(const int s,const int xs)
{
	int i, j, n;
	
	BITBOARD b1,b2,b3;

	mc = first_move[ply];

	GenEp();
	GenCastle();

	if(s==0)
	{
	  b1 = bit_pieces[0][0] & ((bit_units[1] & not_h_file)>>7);
	  b2 = bit_pieces[0][0] & ((bit_units[1] & not_a_file)>>9);
	  b3 = bit_pieces[0][0] & ~(bit_all>>8);
	}
	else
	{
      b1 = bit_pieces[1][0] & ((bit_units[0] & not_h_file)<<9);
	  b2 = bit_pieces[1][0] & ((bit_units[0] & not_a_file)<<7);
	  b3 = bit_pieces[1][0] & ~(bit_all<<8);
	}
	while(b1)
	{
      i = NextBit(b1);
	  b1 &= not_mask[i];
	  n = pawnleft[s][i];
	  AddCapture(i,n,px[board[n]]);
	}
	while(b2)
	{
      i = NextBit(b2);
	  b2 &= not_mask[i];
	  n = pawnright[s][i];
	  AddCapture(i,n,px[board[n]]);
	}
	while(b3)
	{
		i = NextBit(b3);
		b3 &= not_mask[i];
		AddMove(i, pawnplus[side][i]);
		if(rank[s][i]==1 && board[pawndouble[s][i]] == 6)
			AddMove(i,pawndouble[s][i]);
	}

b1 = bit_pieces[s][1];
while(b1)
{
	i = NextBit(b1);
	b1 &= not_mask[i];
	b2 = bit_knightmoves[i] & bit_units[xs];
	while(b2)
	{
		n = NextBit(b2);
		AddCapture(i,n,nx[board[n]]);
		b2 &= not_mask[n];
	}
	b2 = bit_knightmoves[i] & ~bit_all;
	while(b2)
	{
		n = NextBit(b2);
		AddMove(i,n);
		b2 &= not_mask[n];
	}
}

b1 = bit_pieces[s][2];
while(b1)
{
	i = NextBit(b1);
	b1 &= not_mask[i];	
	for (j = 1; j < 8; j+=2)
if(mask_vectors[i][j] & bit_all)
{
	for (n = i;;) 
	{
		n = qrb_moves[n][j];
		if(mask[n] & bit_all)
		{
		if(mask[n] & bit_units[xs])
		{
			AddCapture(i,n,bx[board[n]]);
		}
		break;
		}
		AddMove(i,n);
	}
}
else
{
	for (n = i;;) 
	{
		n = qrb_moves[n][j];
		if (n == -1)
			break;
		AddMove(i,n);
	}
}
}

b1 = bit_pieces[s][3];
while(b1)
{
	i = NextBit(b1);
	b1 &= not_mask[i];
	for (j = 0; j < 8; j+=2)
		if(mask_vectors[i][j] & bit_all)
		{
		for (n = i;;)
		{
			n = qrb_moves[n][j];
			if(mask[n] & bit_all)
			{
			if(mask[n] & bit_units[xs])
			{
				AddCapture(i,n,rx[board[n]]);
			}
			break;
			}
			AddMove(i,n);
		}
		}
		else
		{
		for (n = i;;)
		{
			n = qrb_moves[n][j];
			if (n == -1)
				break;
			AddMove(i,n);
		}
		}
}

b1 = bit_pieces[s][4];
while(b1)
{
	i = NextBit(b1);
	b1 &= not_mask[i];
	for (j = 0; j < 8; j++)
	if(mask_vectors[i][j] & bit_all)
	{
		for (n = i;;) 
		{
			n = qrb_moves[n][j];
			if(mask[n] & bit_all)
			{
				if(mask[n] & bit_units[xs])
				{
					AddCapture(i,n,qx[board[n]]);
					break;
				}
				break;
			}
			AddMove(i,n);
		}
	}
	else
	{
	for (n = i;;) 
		{
			n = qrb_moves[n][j];
			if (n == -1)
				break;
			AddMove(i,n);
		}
	}
}

i = NextBit(bit_pieces[s][5]);
b1 = bit_kingmoves[i] & bit_units[xs];
while(b1)
{
	n = NextBit(b1);
	AddCapture(i,n,kx[board[n]]);
	b1 &= not_mask[n];
}
b1 = bit_kingmoves[i] & ~bit_all;
while(b1)
{
	n = NextBit(b1);
	AddMove(i,n);
	b1 &= not_mask[n];
}
first_move[ply + 1] = mc;
//ShowAll(ply);
}

void AddMove(const int x,const int sq)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = history[x][sq];
	mc++;
}

void AddCapture(const int x,const int sq,const int score)
{
	move_list[mc].start = x;
	move_list[mc].dest = sq;
	move_list[mc].score = score + CAPTURE_SCORE;
	mc++;
}

/*

GenEp looks at the last move played and sees if it is a double pawn move.
If so, it sees if there is an opponent pawn next to it.
If there is, it adds the en passant capture to the move list.
Note that sometimes two en passant captures may be possible.

*/
void GenEp()
{
int ep = game_list[hply-1].dest;
	
if(board[ep] == 0 && abs(game_list[hply-1].start - ep) == 16)
{
	if(col[ep] > 0 && mask[ep-1] & bit_pieces[side][0])
		AddCapture(ep-1,ep+ForwardSquare[side],10);				
	if(col[ep] < 7 && mask[ep+1] & bit_pieces[side][0])
		AddCapture(ep+1,ep+ForwardSquare[side],10);
}
}
/*

GenCastle generates a castling move if the King and Rook haven't moved and
there are no pieces in the way. Attacked squares are looked at in MakeMove. 

*/
void GenCastle()
{
if(side==0)
{
if(castle & 1)
{
	if(!(bit_between[H1][E1] & bit_all))
		AddMove(E1,G1);
}
if(castle & 2) 
{
	if(!(bit_between[A1][E1] & bit_all))
		AddMove(E1,C1);
}
}
else
{
if(castle & 4) 
{
	if(!(bit_between[E8][H8] & bit_all))
		AddMove(E8,G8);
}
if(castle & 8) 
{
	if(!(bit_between[E8][A8] & bit_all))
		AddMove(E8,C8);
}

}
}
/*

GenCaptures is very similar to Gen, except that only captures 
are being generated instead of all moves.

*/
void GenCaptures(const int s, const int xs)
{
first_move[ply + 1] = first_move[ply];

mc = first_move[ply];

int sq,sq2,i,n;
BITBOARD b1,b2;

if(s==0)
{
	b1 = bit_pieces[0][0] & ((bit_units[1] & not_h_file)>>7);
	b2 = bit_pieces[0][0] & ((bit_units[1] & not_a_file)>>9);
}
else
{
    b1 = bit_pieces[1][0] & ((bit_units[0] & not_h_file)<<9);
	b2 = bit_pieces[1][0] & ((bit_units[0] & not_a_file)<<7);
}
while(b1)
{
    i = NextBit(b1);
	b1 &= not_mask[i];
	n = pawnleft[s][i];
	AddCapture(i,n,px[board[n]]);
}
while(b2)
{
    i = NextBit(b2);
	b2 &= not_mask[i];
	n = pawnright[s][i];
	AddCapture(i,n,px[board[n]]);
} 

b1 = bit_pieces[s][1];
while(b1)
{
	sq = NextBit(b1);
	b2 = bit_knightmoves[sq] & bit_units[xs];
	while(b2)
	{
		sq2 = NextBit(b2);
		AddCapture(sq,sq2,nx[board[sq2]]);
		b2 &= not_mask[sq2];
	}
	b1 &= not_mask[sq];
}

b1 = bit_pieces[s][2];
while(b1)
{
	sq = NextBit(b1);
	b2 = bit_bishopmoves[sq] & bit_units[xs];
	while(b2)
	{
		sq2 = NextBit(b2);
		if(!(bit_between[sq][sq2] & bit_all))
			AddCapture(sq,sq2,bx[board[sq2]]);
		b2 &= not_mask[sq2];
	}
	b1 &= not_mask[sq];
}

b1 = bit_pieces[s][3];
while(b1)
{
	sq = NextBit(b1);
	b2 = bit_rookmoves[sq] & bit_units[xs];
	while(b2)
	{
		sq2 = NextBit(b2);
		if(!(bit_between[sq][sq2] & bit_all))
			AddCapture(sq,sq2,rx[board[sq2]]);
		b2 &= not_mask[sq2];
	}
	b1 &= not_mask[sq];
}

b1 = bit_pieces[s][4];
while(b1)
{
	sq = NextBit(b1);
	b2 = bit_queenmoves[sq] & bit_units[xs];
	while(b2)
	{
		sq2 = NextBit(b2);
		if(!(bit_between[sq][sq2] & bit_all))
			AddCapture(sq,sq2,qx[board[sq2]]);
		b2 &= not_mask[sq2];
	}
	b1 &= not_mask[sq];
}

sq = NextBit(bit_pieces[s][5]);
b2 = bit_kingmoves[sq] & bit_units[xs];
while(b2)
{
	sq2 = NextBit(b2);
	AddCapture(sq,sq2,kx[board[sq2]]);
	b2 &= not_mask[sq2];
}
first_move[ply + 1] = mc;
}

