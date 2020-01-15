#include <memory.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <time.h>

void ShowAll(int ply);//

#define U64 unsigned __int64 
#define BITBOARD unsigned __int64 

#define A1	0
#define B1	1
#define C1	2
#define D1	3
#define E1	4
#define F1  5
#define G1	6
#define H1	7

#define A2	8
#define B2	9
#define C2	10
#define D2	11
#define E2	12
#define F2	13
#define G2	14
#define H2	15

#define A3	16
#define B3	17
#define C3	18
#define D3	19
#define E3	20
#define F3	21
#define G3	22
#define H3	23

#define A4	24
#define B4	25
#define C4	26
#define D4	27
#define E4	28
#define F4	29
#define G4	30
#define H4	31

#define A5	32
#define B5	33
#define C5	34
#define D5	35
#define E5	36
#define F5	37
#define G5	38
#define H5	39

#define A6	40
#define B6	41
#define C6	42
#define D6	43
#define E6	44
#define F6	45
#define G6	46
#define H6	47

#define A7	48
#define B7	49
#define C7	50
#define D7	51
#define E7	52
#define F7	53
#define G7	54
#define H7	55

#define A8	56
#define B8	57
#define C8	58
#define D8	59
#define E8	60
#define F8	61
#define G8	62
#define H8	63

#define NORTH 0
#define NE 1
#define EAST 2
#define SE 3
#define SOUTH 4
#define SW 5
#define WEST 6
#define NW 7

#define P 0
#define N 1
#define B 2
#define R 3
#define Q 4
#define K 5
#define EMPTY 6

#define White 0
#define Black 1

#define MAX_PLY 64
#define MOVE_STACK 4000//2000
#define GAME_STACK 2000

#define HASH_SCORE    100000000
#define CAPTURE_SCORE 10000000

typedef struct {
	int start;
	int dest;
	int promote;
	int score;
  } move;

typedef struct {
	int start;
	int dest;
	int promote;
	int capture;
	int fifty;
	int castle;
	U64 hash;
	U64 lock;
} game;

extern int piece_value[6];
extern int pawn_mat[2];
extern int piece_mat[2];

extern int king_endgame[2][64];
extern int passed[2][64];

extern char piece_char[6];

extern int side,xside;
extern int fifty;
extern int ply,hply;
extern int castle;
extern int castle_mask[64];

extern int nodes;

extern int board[64];
extern int init_color[64];
extern int init_board[64];

extern int history[64][64];

extern int square_score[2][6][64];

extern const int row[64];
extern const int col[64];
extern const int nwdiag[64];
extern const int nediag[64];

extern int first_move[MAX_PLY];
extern move move_list[MOVE_STACK];
extern game game_list[GAME_STACK];

extern int hash_start,hash_dest;
extern U64 currentkey,currentlock;

extern int fixed_time;
extern int fixed_depth;
extern int max_time;
extern int start_time;
extern int stop_time;
extern int max_depth;

extern int qrb_moves[64][9];
extern int knight_moves[64][9];
extern int king_moves[64][9];

extern U64 collisions;

extern int turn;

extern int Flip[64];

extern int rank[2][64];
extern int OtherSide[2];
extern int ForwardSquare[2];
extern int Double[2];

extern U64 vectorbits[64][64];
extern U64 mask_vectors[64][8];
extern BITBOARD bit_between[64][64];
extern int pawndouble[2][64];
extern int pawnplus[2][64];
extern int pawnleft[2][64];
extern int pawnright[2][64];

extern BITBOARD bit_pawncaptures[2][64];
extern BITBOARD bit_pawndefends[2][64];
extern BITBOARD bit_left[2][64];
extern BITBOARD bit_right[2][64];
extern BITBOARD bit_pawnmoves[2][64];
extern BITBOARD bit_knightmoves[64];
extern BITBOARD bit_bishopmoves[64];
extern BITBOARD bit_rookmoves[64];
extern BITBOARD bit_queenmoves[64];
extern BITBOARD bit_kingmoves[64];

//current position
extern BITBOARD bit_pieces[2][7];
extern BITBOARD bit_units[2];
extern BITBOARD bit_all;

extern BITBOARD mask_passed[2][64];
extern BITBOARD mask_path[2][64];

extern BITBOARD mask_cols[64];

extern BITBOARD mask[64];
extern BITBOARD not_mask[64];

extern BITBOARD mask_isolated[64];

extern BITBOARD not_a_file;
extern BITBOARD not_h_file;

//init.cpp
void InitBoard();
void SetTables();
void NewPosition();
void Alg(int a,int b);
void Algebraic(int a);
void SetMoves();

//search.cpp
void think();
int Search(int alpha, int beta, int depth);
int CaptureSearch(int alpha,int beta);
int ReCaptureSearch(int,const int);
int reps2();
int Sort(const int from);
void CheckUp();

//gen.cpp
void Gen(int,int);
void GenEp();
void GenCastle();
void AddMove(const int x,const int sq);
void AddMove(const int x,const int sq,const int score);
int GenRecapture(int);
void GenCaptures(const int,const int);
void AddCapture(const int x,const int sq,const int score);

//attack.cpp
bool Attack(const int s,const int sq);
int LowestAttacker(const int s,const int x);

//update.cpp
void UpdatePiece(const int s,const int p,const int start,const int dest);
void RemovePiece(const int s,const int p,const int sq);
void AddPiece(const int s,const int p,const int sq);
int MakeMove(const int,const int);
void TakeBack();
int MakeRecapture(const int,const int);
void UnMakeRecapture();

//eval.cpp
int Eval();

//hash.cpp
void RandomizeHash();
int Random(const int x);
void AddKey(const int,const int,const int);
U64 GetKey();
U64 GetLock();
void Free();
void AddHash(const int s, const move m);
bool LookUp(const int s);

void AddPawnHash(const int s1, const int s2, const int wq, const int wk, const int bq, const int bk);
int GetHashPawn0();
int GetHashPawn1();
int LookUpPawn();
void AddPawnKey(const int s,const int x);

//main.cpp
int main();
int GetTime();
char *MoveString(int from,int to,int promote);
void NewPosition();
int ParseMove(char *s);
void DisplayBoard();
int reps();

int GetHashDefence(const int s,const int half);

int NextBit(BITBOARD bb);

void PrintBitBoard(BITBOARD bb);




