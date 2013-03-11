#include "otello.h"

#undef main

void sig_handler(int signal);

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

	char opt[] = "fhm:t:";
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
			case 't':
				if (tolower(optarg[0]) == 'w')
					gameTactic = T_WORST;
				else if (tolower(optarg[0]) == 'f')
					gameTactic = T_FIRST;
				else if (tolower(optarg[0]) == 'r')
					gameTactic = T_RANDOM;
				break;
			default:
				break;
		}
	}

	font = makeFont("./fonts/arial.ttf", 22);
	if(!font)
		exit(2);

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

	SDL_WM_SetCaption("Otello. Kitchen style", "Game");

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

		signal(SIGINT, sig_handler);

		while (running) {
			while (SDL_PollEvent(&event))
			{
				running = onEvent(&event, board);
			}
			onRender(board);
			SDL_Delay(10);
		}
	} else {
		exit(1);
	}

	exit(0);
}

void sig_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("Exit by signal\n");
		exit(0);
	}
}
