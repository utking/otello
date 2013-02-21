#include "otello.h"
#include <unistd.h>

Field *board;

int main(int argc, char *argv[])
{
	int running = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init failed\n");
		return 1;
	}
	SDL_Surface *SurfDisplay = SDL_SetVideoMode(
			640, 640, 32,
			SDL_HWSURFACE | SDL_DOUBLEBUF);

	SDL_WM_SetCaption("Otello", "Game");

	SDL_Surface* boardSurface = loadBackground();
	if (boardSurface)
	{
		SDL_Rect destRect;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = 0;
		destRect.h = 0;

		SDL_BlitSurface(boardSurface, NULL, SurfDisplay, &destRect);
		SDL_Flip(SurfDisplay);
		SDL_Event event;

		while (running)
		{
			while (SDL_PollEvent(&event))
			{
				running = onEvent(&event);
			}
			onLoop();
			onRender();
			usleep(1350);
		}
	}
	else
	{
		return 1;
	}

	SDL_Quit();

	printf("%s\n", "Otello. The Game");
	board = createBoard();
	if (!board)
	{
		printf("%s\n", "Can't create board. Terminate.");
		return 1;
	}
	printBoard(board);
	setField(board, 3, 3, WHITE);
	setField(board, 4, 4, BLACK);
	printBoard(board);

	destroyBoard(board);
	return 0;
}

