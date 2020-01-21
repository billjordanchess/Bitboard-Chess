#include "globals.h"

int ReverseSquare[2] = {-8,8};

game* g;
/*

UpdatePiece updates the Hash Table key, the board and the table_score (the incremental
evaluation) whenever a piece moves.

*/
void UpdatePiece(const int s,const int p,const int start,const int dest)
{
bit_units[s] &= not_mask[start];
bit_units[s] |= mask[dest];
bit_all = bit_units[0] | bit_units[1];
AddKey(s,p,start);
AddKey(s,p,dest);
board[dest] = p;
board[start] = EMPTY;
bit_pieces[s][p] &= not_mask[start];
bit_pieces[s][p] |= mask[dest];	
}
/*

RemovePiece updates the Hash Table key, the board and the table_score (the incremental
evaluation) whenever a piece is removed.

*/
void RemovePiece(const int s,const int p,const int sq)
{
AddKey(s,p,sq);
board[sq] = EMPTY;
bit_units[s] &= not_mask[sq];
bit_pieces[s][p] &= not_mask[sq];
bit_all = bit_units[0] | bit_units[1];
}
/*

AddPiece updates the Hash Table key, the board and the table_score (the incremental
evaluation) whenever a piece is added.

*/
void AddPiece(const int s,const int p,const int sq)
{
board[sq] = p;
AddKey(s,p,sq);
bit_units[s] |= mask[sq];
bit_pieces[s][p] |= mask[sq];
bit_all = bit_units[0] | bit_units[1];
}
/*

MakeMove updates the board whenever a move is made.
If the King moves two squares then it sees if castling is legal.
If a pawn moves and changes file s without making a capture, then its an en passant capture
and the captured pawn is removed.
If the move is a capture then the captured piece is removed from the board.
If castling permissions are effected then they are updated.
If a pawn moves to the last rank then its promoted. The pawn is removed and a queen is added.
If the move leaves the King in check (for example, if a pinned piece moved), then the move is taken back.

*/
int MakeMove(const int start,const int dest)
{	
if (abs(start - dest) ==2 && board[start] == K)
{
	if (Attack(xside,start)) 
			return false;
	if(dest==G1)
	{
		if (Attack(xside,F1)) 
			return false;
		UpdatePiece(side,R,H1,F1);
	}
	else if(dest==C1)
	{
		if (Attack(xside,D1)) 
			return false;
		UpdatePiece(side,R,A1,D1);
	}
	else if(dest==G8)
	{
		if (Attack(xside,F8)) 
			return false;
		UpdatePiece(side,R,H8,F8);
	}
	else if(dest==C8)
	{	
		if (Attack(xside,D8)) 
			return false;
		UpdatePiece(side,R,A8,D8);
	}
}

g = &game_list[hply];
g->start = start;
g->dest = dest;
g->capture = board[dest];
g->castle = castle;
g->fifty = fifty;
g->hash = currentkey;
g->lock = currentlock;
g->castle = castle;
castle &= castle_mask[start] & castle_mask[dest];

ply++;
hply++;
fifty++;

if (board[start] == P)
{
	fifty = 0;
	if (board[dest] == EMPTY && col[start] != col[dest])
	{
		RemovePiece(xside,P,dest + ReverseSquare[side]);
	}
}

if(board[dest]<6)
{
	fifty = 0;
    RemovePiece(xside,board[dest],dest);
}

if (board[start]==P && (row[dest]==0 || row[dest]==7))
{
    RemovePiece(side,P,start);
    AddPiece(side,Q,dest);
	g->promote = Q;
}
else
{
	g->promote = 0;
    UpdatePiece(side,board[start],start,dest);
}

side ^= 1;
xside ^= 1;
if (Attack(side,NextBit(bit_pieces[xside][K]))) 
{
	TakeBack();
	return false;
}
return true;
}
/*

TakeBack is the opposite of MakeMove.

*/
void TakeBack()
{	
	side ^= 1;
	xside ^= 1;
	ply--;
	hply--;

	game* m = &game_list[hply];
	int start = m->start;
	int dest = m->dest;
	
	castle = m->castle;
	fifty = m->fifty;

	if (board[dest]==P && m->capture == EMPTY && col[start] != col[dest])
    {
        AddPiece(xside,P,dest + ReverseSquare[side]);
	}
	if(m->promote == Q)
    {
       AddPiece(side,P,start);
       RemovePiece(side,board[dest],dest);
    }
	else
    {
       UpdatePiece(side,board[dest],dest,start);
    }
    if (m->capture != EMPTY)
    {
      AddPiece(xside,m->capture,dest);
    }
	if (abs(start - dest) == 2 && board[start] == K)
    {
		if(dest==G1)
			UpdatePiece(side,R,F1,H1);
		else if(dest==C1)
			UpdatePiece(side,R,D1,A1);
		else if(dest==G8)
			UpdatePiece(side,R,F8,H8);
		else if(dest==C8)
			UpdatePiece(side,R,D8,A8);
 	}
}
/*

MakeRecapture is simpler than MakeMove because there is no castling involved and it
doesn't include en passant capture and promotion.
It the capture is illegal it is taken back.

*/
int MakeRecapture(const int from,const int to)
{	 
	game_list[hply].start = from;
	game_list[hply].dest = to;
	game_list[hply].capture = board[to];
	
	++ply;
	++hply;

    RemovePiece(xside,board[to],to);
    UpdatePiece(side,board[from],from,to);

	side ^= 1;
	xside ^= 1;
	if (Attack(side,NextBit(bit_pieces[xside][K]))) 
	{
		UnMakeRecapture();
		return false;
	}
	return true;
}
/*

UnMakeRecapture is very similar to MakeRecapture.

*/
void UnMakeRecapture()
{
	side ^= 1;
	xside ^= 1;
	--ply;
	--hply;

    UpdatePiece(side,board[game_list[hply].dest], game_list[hply].dest, game_list[hply].start);
	AddPiece(xside, game_list[hply].capture, game_list[hply].dest);
}

