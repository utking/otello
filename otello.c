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
			board[HEIGHT * i + j].X = j;
			board[HEIGHT * i + j].Y = i;
		}
	}
	return board;
}

void destroyBoard(Field *board)
{
	if (board)
	{
		free(board);
		board = NULL;
	}
}

int setField(Field* board, unsigned int X, unsigned int Y, Owner owner)
{
	// Check if one of initial coins
	int initial = (X == 3 && Y == 3) || (X == 4 && Y == 4)
		|| (X == 3 && Y == 4) || (X == 4 && Y == 3);
	// Put coin
	if (initial || isPermittedField(board[HEIGHT * Y + X]))
	{
		board[HEIGHT * Y + X].owner = owner;
		//	*********** find and flip others ***********
		return 1;
	}
	return 0;
}

int isPermittedField(const Field field)
{
	printf("NONE: %d\n", field.owner == NONE) && 
		printf("neighbors: %d\n", hasNieghbors(field));
	return (field.owner == NONE) && hasNieghbors(field);
}

int hasNieghbors(const Field field)
{
	unsigned int x = field.X;
	unsigned int y = field.Y;
	return
		isGoorNeighbor(x - 1, y - 1) || // Top left
		isGoorNeighbor(x - 0, y - 1) || // Top center
		isGoorNeighbor(x + 1, y - 1) || // Top right
		isGoorNeighbor(x - 1, y - 0) || // Middle left
		isGoorNeighbor(x + 1, y - 0) || // Middle right
		isGoorNeighbor(x - 1, y + 1) || // Bottom left
		isGoorNeighbor(x - 0, y + 1) || // Bottom center
		isGoorNeighbor(x + 1, y + 1);	// Bottom right
}

int isGoorNeighbor(const int x, const int y)
{
	if (x >= 0 && y >= 0 && board[y * 8 + x].owner == BLACK)
		return 1;
	return 0;
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

int onEvent(SDL_Event *event, Field* board)
{
	if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_q)
		return 0;
	if (event->type == SDL_QUIT)
		return 0;

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		Field field;
		field.X = event->button.x / 80;
		field.Y = event->button.y / 80;
		field = board[event->button.y / 80 * HEIGHT + event->button.x / 80];
		printf("X:%d Y:%d Owner:%d\n", field.X, field.Y, field.owner);

		if (isPermittedField(field))
		{
			setField(board, field.X, field.Y, WHITE);
		}
	}
	return 1;
}

void onRender(const Field* board)
{
	if (!whiteSurface || !blackSurface || !SurfDisplay)
	{
		printf("onRender: %s\n", "on of a surfaces is NULL");
		return;
	}

	if (board)
	{
		SDL_Surface* surf = NULL;

		SDL_Rect destRect;

		SDL_BlitSurface(boardSurface, NULL, SurfDisplay, &destRect);
		for (int i = 0; i < HEIGHT; ++i)
		{
			for (int j = 0; j < WIDTH; ++j)
			{
				switch (board[8 * i + j].owner)
				{
					case WHITE:
						surf = whiteSurface;
						break;
					case BLACK:
						surf = blackSurface;
						break;
					default:
						surf = NULL;
						break;
				}
				drawItem(surf, SurfDisplay, j, i);
			}
		}
		SDL_Flip(SurfDisplay);
	}
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

