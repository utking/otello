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

int onEvent(SDL_Event *event)
{
	if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_q)
		return 0;
	else if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		printf("%d / %d | %d / %d\n", 
				event->button.x, event->button.y,
				event->button.x / 80, event->button.y / 80
				);
	}
	return 1;
}

void onRender(const Field* board)
{
	return;
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

SDL_Surface* loadBlack()
{
	SDL_Surface* tmp = IMG_Load("./images/black.png");
	if (tmp == NULL)
	{
		printf("Load black file [%s] failed\n", "./images/black.png");
		return NULL;
	}
	SDL_Surface* surface = SDL_DisplayFormatAlpha(tmp);
	if (surface == NULL)
	{
		printf("SDL_DisplayFormat failed: %s\n", SDL_GetError());
		return NULL;
	}
	return surface;
}

SDL_Surface* loadWhite()
{
	SDL_Surface* tmp = IMG_Load("./images/white.png");
	if (tmp == NULL)
	{
		printf("Load black file [%s] failed\n", "./images/white.png");
		return NULL;
	}
	SDL_Surface* surface = SDL_DisplayFormatAlpha(tmp);
	if (surface == NULL)
	{
		printf("SDL_DisplayFormat failed: %s\n", SDL_GetError());
		return NULL;
	}
	return surface;
}

void drawItem(SDL_Surface* srcSurf, SDL_Surface* destSurf, unsigned int X, unsigned int Y)
{
	SDL_Rect destRect;
	destRect.x = X * 80;
	destRect.y = Y * 80;
	SDL_BlitSurface(srcSurf, NULL, destSurf, &destRect);
}

