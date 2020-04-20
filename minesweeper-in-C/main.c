#define _CRT_SECURITY_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>

#define SIZE_ROW 10
#define SIZE_COL 10

#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100
#define SPACE 32

enum _gameState
{
	IDLE,
	GAMEOVER,
	VICTORY
};

typedef struct _position
{
	unsigned int row;
	unsigned int col;
} position;

position playerPosition;
bool mineExists[SIZE_ROW][SIZE_COL] = { 0, };
bool flagExists[SIZE_ROW][SIZE_COL] = { 0, };
bool hasUncovered[SIZE_ROW][SIZE_COL] = { 0, };
unsigned int numAround[SIZE_ROW][SIZE_COL] = { 0, };


void init(int percent);
void increaseNum(int row, int col);
void render();
void showHowToPlay();
void setColor(int foreground, int background);
char getInput();
int process(char keyInput);
bool checkAllCells(void);
void autoUncover(int row, int col);

int main()
{
	char keyInput;
	int gameState = IDLE;
	init(0);
	while (gameState == IDLE)
	{
		render();
		keyInput = getInput();
		gameState = process(keyInput);
	}

	if (gameState == GAMEOVER)
	{
		printf("GAME OVER !!\n");
	}
	else if (gameState == VICTORY)
	{
		printf("VICTORY !!\n");
	}
	return 0;
}

void init(int percent)	//Create mine according to percent
{
	int row, col;
	srand(time(NULL));

	for (row = 0; row < SIZE_ROW; row++)
	{
		for (col = 0; col < SIZE_COL; col++)
		{
			if (rand() % 100 < percent)	//mine exists
			{
				mineExists[row][col] = true;
				increaseNum(row, col);
			}
		}
	}

	playerPosition.row = 0;
	playerPosition.col = 0;
}

void increaseNum(int row, int col)
{
	if (row != 0)	//UP
	{
		numAround[row - 1][col]++;
	}
	if (row != SIZE_ROW - 1)	//DOWN
	{
		numAround[row + 1][col]++;
	}
	if (col != 0)	//LEFT
	{
		numAround[row][col - 1]++;
	}
	if (col != SIZE_COL - 1)	//RIGHT
	{
		numAround[row][col + 1]++;
	}
	if (row != 0 && col != 0)	//LEFT UP
	{
		numAround[row - 1][col - 1]++;
	}
	if (row != 0 && col != SIZE_COL - 1)	//RIGHT UP
	{
		numAround[row - 1][col + 1]++;
	}
	if (row != SIZE_ROW - 1 && col != 0)	//LEFT DOWN
	{
		numAround[row + 1][col - 1]++;
	}
	if (row != SIZE_ROW - 1 && col != SIZE_COL - 1)	//LEFT DOWN
	{
		numAround[row + 1][col + 1]++;
	}
}

void render()
{
	enum color
	{
		BLACK,
		BLUE,
		GREEN,
		CYAN,
		RED,
		MAGENTA,
		BROWN,
		LIGHTGRAY,
		DARKGRAY,
		LIGHTBLUE,
		LIGHTGREEN,
		LIGHTCYAN,
		LIGHTRED,
		LIGHTMAGENTA,
		YELLOW,
		WHITE
	};
	int row, col;
	system("cls");
	for (row = 0; row < SIZE_ROW; row++)
	{
		for (col = 0; col < SIZE_COL; col++)
		{
			if (playerPosition.row == row && playerPosition.col == col)
			{
				setColor(BLACK, YELLOW);
			}

			if (flagExists[row][col])
			{
				printf(" �� ");
			}
			else if (!hasUncovered[row][col])
			{
				printf(" �� ");
			}
			else
			{
				printf(" %02d ", numAround[row][col]);
			}

			setColor(WHITE, BLACK);
		}
		printf("\n\n");
	}
}

void showHowToPlay()
{
	printf("M: flag the room\nSPACE: dig the room\n");

}

void setColor(int foreground, int background)
{
	int color = foreground + background * 16;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}


char getInput()
{
	char keyInput;
	keyInput = _getch();
	return keyInput;
}

int process(char keyInput)
{
	if (keyInput == LEFT && playerPosition.col != 0)
	{
		playerPosition.col--;
		printf("LEFT\n");
		return IDLE;
	}
	else if (keyInput == RIGHT && playerPosition.col != SIZE_COL - 1)
	{
		playerPosition.col++;
		printf("RIGHT\n");
		return IDLE;
	}
	else if (keyInput == UP && playerPosition.row != 0)
	{
		playerPosition.row--;
		printf("UP\n");
		return IDLE;
	}
	else if (keyInput == DOWN && playerPosition.row != SIZE_ROW - 1)
	{
		playerPosition.row++;
		printf("DOWN\n");
		return IDLE;
	}
	else if (keyInput == 'm' && hasUncovered[playerPosition.row][playerPosition.col] == false)	//flag
	{
		if (flagExists[playerPosition.row][playerPosition.col])
		{
			flagExists[playerPosition.row][playerPosition.col] = false;
		}
		else
		{
			flagExists[playerPosition.row][playerPosition.col] = true;
		}
	}
	else if (keyInput == SPACE && flagExists[playerPosition.row][playerPosition.col] == false)	//uncover
	{
		if (mineExists[playerPosition.row][playerPosition.col])
		{
			return GAMEOVER;
		}
		else
		{
			if (numAround[playerPosition.row][playerPosition.col] != 0)
			{
				hasUncovered[playerPosition.row][playerPosition.col] = true;
			}
			else
			{
				autoUncover(playerPosition.row, playerPosition.col);
			}

			if (checkAllCells() == true)
			{
				return VICTORY;
			}
		}
	}
	return IDLE;
}

bool checkAllCells(void)
{
	int row, col;
	for (row = 0; row < SIZE_ROW; row++)
	{
		for (col = 0; col < SIZE_COL; col++)
		{
			if (mineExists[row][col] == false)
			{
				if (hasUncovered[row][col] == false)
				{
					return false;
				}
			}
		}
	}
	return true;
}

void autoUncover(int row, int col)
{
	if (hasUncovered[row][col] == true)
	{
		return;
	}
	hasUncovered[row][col] = true;
	if (numAround[row][col] > 0)
	{
		return;
	}
	if (row != 0)	//UP
	{
		autoUncover(row - 1, col);
	}
	if (row != SIZE_ROW - 1)	//DOWN
	{
		autoUncover(row + 1, col);
	}
	if (col != 0)	//LEFT
	{
		autoUncover(row, col - 1);
	}
	if (col != SIZE_COL - 1)	//RIGHT
	{
		autoUncover(row, col + 1);
	}
	if (row != 0 && col != 0)	//LEFT UP
	{
		autoUncover(row - 1, col - 1);
	}
	if (row != 0 && col != SIZE_COL - 1)	//RIGHT UP
	{
		autoUncover(row - 1, col + 1);
	}
	if (row != SIZE_ROW - 1 && col != 0)	//LEFT DOWN
	{
		autoUncover(row + 1, col - 1);
	}
	if (row != SIZE_ROW - 1 && col != SIZE_COL - 1)	//LEFT DOWN
	{
		autoUncover(row + 1, col + 1);
	}
	return;
}