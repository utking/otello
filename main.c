#include "otello.h"

#undef main

unsigned int WND_WIDTH  = 890;
unsigned int WND_HEIGHT = 640;
unsigned int WIDTH = 8;
unsigned int HEIGHT = 8;
unsigned int FIELD_WIDTH = 80;
GameMode gameMode = MODE_PC;

int main(int argc, char *argv[])
{
	int running = 1;
	int isFullScreen = 0;
	blackSurface = NULL;
	whiteSurface = NULL;
	SurfDisplay = NULL;
	boardSurface = NULL;
	textSurface = NULL;

	font = NULL;

	if(TTF_Init()==-1)
	{
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	char opt[] = "fhm:";
	int rc;

	while (-1 != (rc = getopt(argc, argv, opt)))
	{
		switch (rc) 
		{
			case 'h':
				print_usage(argv);
				exit(0);
				break;
			case 'f':
				isFullScreen = 1;
				break;
			case 'm':
				if (tolower(optarg[0]) == 'h')
					gameMode = MODE_HUMAN;
				break;
			default:
				break;
		}
	}

	font = makeFont("./fonts/arial.ttf", 28);
	if(!font)
		exit(2);

	scoreSurface = TTF_RenderText_Solid(font, "Score:", fontColor);
	TTF_CloseFont(font);
	font = makeFont("./fonts/arial.ttf", 22);

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

	int sdl_flags = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_HWACCEL;
	if (isFullScreen)
		sdl_flags |= SDL_FULLSCREEN;
	SurfDisplay = SDL_SetVideoMode(WND_WIDTH, WND_HEIGHT, 32, sdl_flags);

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

		//Start first game
		newGame();

		while (running) {
			while (SDL_PollEvent(&event))
			{
				running = onEvent(&event, board);
			}
			onRender(board);
			if (currentOwner == BLACK)
				usleep(200000);
			usleep(10000);
		}
	} else {
		exit(1);
	}

	exit(0);
}
