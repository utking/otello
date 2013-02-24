#include "otello.h"
#include <unistd.h>

void cleanup();

int main(int argc, char *argv[])
{
	int running = 1;

	atexit(cleanup);

	board = createBoard();
	prevBoard = createBoard();
	if (!board)
	{
		printf("%s\n", "Can't create board. Terminate.");
		exit(1);
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init failed\n");
		exit(1);
	}
	SurfDisplay = SDL_SetVideoMode(
			890, 640, 32,
			SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL);

	SDL_WM_SetCaption("Otello", "Game");

	boardSurface = loadBackground();
	blackSurface = loadBlack();
	whiteSurface = loadWhite();
	textSurface = loadTextSurface();

	if (boardSurface && blackSurface && whiteSurface && textSurface)
	{
		Uint32 value = SDL_MapRGB(blackSurface->format, 0, 0, 255);
		SDL_SetColorKey(blackSurface, SDL_SRCCOLORKEY, value);

		value = SDL_MapRGB(whiteSurface->format, 0, 0, 255);
		SDL_SetColorKey(whiteSurface, SDL_SRCCOLORKEY, value);

		value = SDL_MapRGB(textSurface->format, 0, 0, 255);
		SDL_SetColorKey(textSurface, SDL_SRCCOLORKEY, value);

		// Draw initial state
		putField(board, 3, 3, WHITE);
		putField(board, 4, 4, WHITE);
		putField(board, 3, 4, BLACK);
		putField(board, 4, 3, BLACK);

		SDL_Event event;

		currentOwner = BLACK;
		saveState();

		while (running) {
			while (SDL_PollEvent(&event))
			{
				running = onEvent(&event, board);
			}
			onRender(board);
			usleep(10000);
		}
	} else {
		exit(1);
	}

	exit(0);
}

void cleanup()
{
	destroyBoard(board);
	if (blackSurface)
		SDL_FreeSurface(blackSurface);
	if (whiteSurface)
		SDL_FreeSurface(whiteSurface);
	if (boardSurface)
		SDL_FreeSurface(boardSurface);
	if (textSurface)
		SDL_FreeSurface(textSurface);

	SDL_Quit();
}
