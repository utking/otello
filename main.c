#include "otello.h"
#include <unistd.h>

void cleanup();

int main(int argc, char *argv[])
{
	int running = 1;

	board = createBoard();
	if (!board)
	{
		printf("%s\n", "Can't create board. Terminate.");
		return 1;
	}

	atexit(cleanup);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init failed\n");
		return 1;
	}
	SurfDisplay = SDL_SetVideoMode(
			640, 640, 32,
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL);

	SDL_WM_SetCaption("Otello", "Game");

	boardSurface = loadBackground();
	blackSurface = loadBlack();
	whiteSurface = loadWhite();
	if (boardSurface && blackSurface && whiteSurface)
	{
		SDL_Rect destRect;
		destRect.x = 0;
		destRect.y = 0;
		destRect.w = 0;
		destRect.h = 0;

		SDL_BlitSurface(boardSurface, NULL, SurfDisplay, &destRect);

		Uint32 value = SDL_MapRGB(blackSurface->format, 0, 0, 255);
		SDL_SetColorKey(blackSurface, SDL_SRCCOLORKEY, value);

		value = SDL_MapRGB(whiteSurface->format, 0, 0, 255);
		SDL_SetColorKey(whiteSurface, SDL_SRCCOLORKEY, value);

		// Draw initial state
		setField(board, 3, 3, WHITE);
		setField(board, 4, 4, WHITE);
		setField(board, 3, 4, BLACK);
		setField(board, 4, 3, BLACK);

		SDL_Event event;

		while (running)
		{
			while (SDL_PollEvent(&event))
			{
				running = onEvent(&event, board);
			}
			onRender(board);
			usleep(10000);
		}
	}
	else
	{
		if (blackSurface)
			SDL_FreeSurface(blackSurface);
		if (whiteSurface)
			SDL_FreeSurface(whiteSurface);
		if (boardSurface)
			SDL_FreeSurface(boardSurface);

		return 1;
	}

	SDL_FreeSurface(boardSurface);
	SDL_FreeSurface(blackSurface);
	SDL_FreeSurface(whiteSurface);

	SDL_Quit();

	return 0;
}

void cleanup()
{
	destroyBoard(board);
}
