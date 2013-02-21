#include "otello.h"

Field* createBoard()
{
	Field *board = malloc(WIDTH * HEIGHT * sizeof(Field));
	if (!board)
		return NULL;

	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			board[HEIGHT * i + j].owner = NONE;
		}
	}
	return board;
}

void destroyBoard(Field *board)
{
	free(board);
}

int setField(Field* board, unsigned int X, unsigned int Y, Owner owner)
{
	if (!isPermittedField(board[HEIGHT * Y + X]))
		return 0;
	board[HEIGHT * Y + X].owner = owner;
	return 1;
}

int isPermittedField(const Field field)
{
	return 1;
}

int isFieldFree(const Field field)
{
	return field.owner != NONE;
}

void printBoard(Field *board)
{
	char owner;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			switch (board[HEIGHT * i + j].owner) 
			{
				case BLACK:
					owner = '1';
					break;
				case WHITE:
					owner = '2';
					break;
				default:
					owner = '_';
					break;
			}
			printf("%c ", owner);
		}
		printf("\n");
	}
}

void onLoop()
{
	;
}

int onEvent(SDL_Event *event)
{
	if (event->type == SDL_KEYUP)
		return 0;
	return 1;
}

void onRender()
{
	;
}

SDL_Surface* loadBackground()
{
	SDL_Surface* tmp = IMG_Load("./images/board.png");
	if (tmp == NULL)
	{
		printf("Load board file [%s] failed\n", "./images/board.png");
		return NULL;
	}
	SDL_Surface* boardSurface = SDL_DisplayFormat(tmp);
	if (boardSurface == NULL)
	{
		printf("SDL_DisplayFormat failed: %s\n", SDL_GetError());
		return NULL;
	}
	return boardSurface;
}

