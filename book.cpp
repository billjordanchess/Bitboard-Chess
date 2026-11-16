#include "globals.h"

extern int move_start,move_dest;

char lines[100][80];
move book_list[MOVE_STACK];
int total;

void LoadBook();
void InitBookboard();

int start_bb[64] =
{
	3,1,2,4,5,2,1,3,
	0,0,0,0,0,0,0,0,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	0,0,0,0,0,0,0,0,
	3,1,2,4,5,2,1,3
};
	
int start_bc[64] = 
{
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	6,6,6,6,6,6,6,6,
	1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1
};

int bb[64],bc[64];

int Book()
{
	if(hply>5)
		return 0;
	int r;
	move_start = -1;
	r = Random(100);
	if(hply==0)
	{
		if(r<50)
		{
			move_start=D2; move_dest=D4;
		}
		else
		{
			move_start=E2; move_dest=E4;
		}
		return 1;
	}

	int start,dest,match,m=0;
	int matches[100][2];
	int y;

	for(int x=0;x<total;x++)
	{
		InitBookboard();
		for(y=0;y<hply*5;y+=5)
		{
		   start = lines[x][y] - 'a';
		   start += ((lines[x][y+1] - '0') - 1)*8;
           dest =  lines[x][y+2] - 'a';
           dest += ((lines[x][y+3] - '0') - 1)*8;
		   bb[dest] = bb[start];
		   bc[dest] = bc[start];
		   bb[start] = 6;
		   bc[start] = 6;
		}
		match = 1;
	for(int z=0;z<64;z++)
	{
		if((board[z] != bb[z]))// || (color[z] != bc[z]))
		{
			match=0;break;
		}
	}
	if(match==1)
	{
	   start = lines[x][y] - 'a';
	   start += ((lines[x][y+1] - '0') - 1)*8;
       dest =  lines[x][y+2] - 'a';
       dest += ((lines[x][y+3] - '0') - 1)*8;
		matches[m][0] = start;
		matches[m][1] = dest;
		m++;
	}

	}

	if(m==0)
		return 0;
	else
	{
		r = Random(m);
		move_start = matches[r][0]; 
		move_dest = matches[r][1];
		return 1;
	}
}

void LoadBook()
{
	int c = 0;
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 b1c3 g8f6 ");
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 b1c3 f8b4 ");
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 b1c3 d5e4 ");
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 b1d2 g8f6 ");
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 b1d2 c7c5 ");
	strcpy_s(lines[c++],"e2e4 e7e6 d2d4 d7d5 e4e5 c7c5 ");
	strcpy_s(lines[c++],"e2e4 c7c6 d2d4 d7d5 e4e5 c8f5 ");
	strcpy_s(lines[c++],"e2e4 c7c6 d2d4 d7d5 e4d5 c6d5 ");
	strcpy_s(lines[c++],"e2e4 c7c6 d2d4 d7d5 b1c3 d5e4 ");
	strcpy_s(lines[c++],"e2e4 c7c6 d2d4 d7d5 b1c3 g7g6 ");
	strcpy_s(lines[c++],"e2e4 c7c5 g1f3 b8c6 d2d4 c5d4 ");
	strcpy_s(lines[c++],"e2e4 c7c5 g1f3 d7d6 d2d4 c5d4 ");
	strcpy_s(lines[c++],"e2e4 c7c5 g1f3 e7e6 d2d4 c5d4 ");
	strcpy_s(lines[c++],"e2e4 c7c5 g1f3 g7g6 d2d4 c5d4 ");
	strcpy_s(lines[c++],"e2e4 c7c5 c2c3 g8f6 e4e5 f6d5 ");
	strcpy_s(lines[c++],"e2e4 c7c5 c2c3 d7d5 e4d5 d8d5 ");
	strcpy_s(lines[c++],"e2e4 c7c5 c2c3 b7b6 d2d4 c8b7 ");
	strcpy_s(lines[c++],"e2e4 c7c5 b1c3 b8c6 f2f4 g7g6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 f1c4 g8f6 d2d3 c7c6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 d2d4 e5d4 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 d2d4 e5d4 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 d2d4 e5d4 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 d2d4 e5d4 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1c4 g8f6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1c4 f8c5 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1c4 f8e7 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1b5 f8c5 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1b5 g8f6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 f1b5 a7a6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 b8c6 b1c3 g8f6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 g8f6 f3e5 d7d6 ");
	strcpy_s(lines[c++],"e2e4 e7e5 g1f3 d7d6 d2d4 b8d7 ");
	strcpy_s(lines[c++],"e2e4 e7e5 b1c3 g8f6 f2f4 d7d5 ");
	strcpy_s(lines[c++],"e2e4 e7e5 b1c3 g8f6 f1c4 b8c6 ");
	strcpy_s(lines[c++],"e2e4 d7d5 e4d5 d8d5 b1c3 d8a5 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 e7e6 b1c3 g8f6 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 c7c6 b1c3 g8f6 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 c7c6 g1f3 g8f6 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 d5c4 e2e3 g8f6 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 c7c6 c4d5 c6d5 ");
	strcpy_s(lines[c++],"d2d4 d7d5 c2c4 e7e6 c4d5 e6d5 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 c7c5 d4d5 g7g6 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 e7e6 b1c3 f8b4 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 e7e6 b1c3 c7c5 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 e7e6 g1f3 f8b4 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 e7e6 g1f3 b7b6 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 e7e6 g1f3 c7c5 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 g7g6 b1c3 d7d5 ");
	strcpy_s(lines[c++],"d2d4 g8f6 c2c4 g7g6 b1c3 f8g7 ");
	strcpy_s(lines[c++],"d2d4 g8f6 g1f3 g7g6 c1f4 f8g7 ");
	strcpy_s(lines[c++],"d2d4 f7f5 c2c4 g8f6 g1f3 e7e6 ");
	strcpy_s(lines[c++],"d2d4 f7f5 c2c4 g8f6 g1f3 e7e6 ");
	strcpy_s(lines[c++],"d2d4 d7d4 g1f3 g8f6 e2e3 g7g6 ");

	total = c;
}

void InitBookboard()
{
for(int x=0;x<64;x++)
{
	bb[x] = start_bb[x];
	bc[x] = start_bc[x];
}
}

