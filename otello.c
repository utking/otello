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

int setField(Field* board, int X, int Y, 
		Owner owner)
{
	if (isPermittedField(board[HEIGHT * Y + X]))
	{
		//	*********** find and flip others ***********
		if (flipLines(&(board[HEIGHT * Y + X])))
		{
			board[HEIGHT * Y + X].owner = owner;
			return 1;
		}
	}
	return 0;
}

int putField(Field* board, int X, int Y, Owner owner)
{
	board[HEIGHT * Y + X].owner = owner;
	return 1;
}

int isPermittedField(const Field field)
{
	return 
		(field.owner == NONE) && 
		hasNieghbors(field) && 
		canFlipLines(field);
}

int hasNieghbors(const Field field)
{
	int x = field.X;
	int y = field.Y;
	return
		isGoodNeighbor(x - 1, y - 1) || // Top left
		isGoodNeighbor(x - 0, y - 1) || // Top center
		isGoodNeighbor(x + 1, y - 1) || // Top right
		isGoodNeighbor(x - 1, y - 0) || // Middle left
		isGoodNeighbor(x + 1, y - 0) || // Middle right
		isGoodNeighbor(x - 1, y + 1) || // Bottom left
		isGoodNeighbor(x - 0, y + 1) || // Bottom center
		isGoodNeighbor(x + 1, y + 1);	// Bottom right
}

int isGoodNeighbor(const int x, const int y)
{
	if (x >= 0 && y >= 0 && 
			(board[y * HEIGHT + x].owner != NONE && 
			 board[y * HEIGHT + x].owner != currentOwner))
	{
		return 1;
	}
	return 0;
}

int isFieldFree(const Field field)
{
	return field.owner != NONE;
}

int onEvent(SDL_Event *event, Field* board)
{
	if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_q)
		return 0;

	if (event->type == SDL_MOUSEBUTTONUP && 
			event->button.button == SDL_BUTTON_LEFT)
	{
		int x = event->button.x;
		int y = event->button.y;

		if (isInRect(x, y, exitDestRect))
		{
			return 0;
		}
		else if (isInRect(x, y, newDestRect))
		{
			newGame();
			return 1;
		}
	}
	if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_b)
	{
		restoreState();
		return 1;
	}
	if (event->type == SDL_QUIT)
		return 0;

	if (currentOwner == NONE)
		return 1;

	// Check if in PC game mode
	if (gameMode == MODE_PC && event->type == SDL_USEREVENT && event->user.code == 127)
	{
		while (currentOwner == WHITE)
		{
			onRender(board);
			usleep(1000000);
			Field nextMove = findNextMove(WHITE);
			if (setField(board, nextMove.X, nextMove.Y, currentOwner))
			{
				int hasWhiteMoves = hasNextMove(WHITE);
				int hasBlackMoves = hasNextMove(BLACK);
				if (!hasBlackMoves && !hasWhiteMoves)
				{
					currentOwner = NONE;
				}
				else if (hasBlackMoves) 
				{
					currentOwner = BLACK;
				}
				else 
				{
					currentOwner = WHITE;
				}
			}
		}
	}
	// Human turn or game in human mode
	else if (event->type == SDL_MOUSEBUTTONUP && 
			event->button.button == SDL_BUTTON_LEFT)
	{
		int x = event->button.x;
		int y = event->button.y;

		if (x < WIDTH * FIELD_WIDTH && y < HEIGHT * FIELD_WIDTH)
		{
			Field field;
			field.X = event->button.x / FIELD_WIDTH;
			field.Y = event->button.y / FIELD_WIDTH;
			field = board[
				event->button.y / FIELD_WIDTH * HEIGHT + 
				event->button.x / FIELD_WIDTH];

			if (isPermittedField(field))
			{
				prevOwner = currentOwner;
				saveState();
				if (setField(board, field.X, field.Y, currentOwner))
				{
					if (currentOwner == BLACK)
						currentOwner = WHITE;
					else if (currentOwner == WHITE)
						currentOwner = BLACK;

				}
			}

			int hasWhiteMoves = hasNextMove(WHITE);
			int hasBlackMoves = hasNextMove(BLACK);
			if (!hasBlackMoves && !hasWhiteMoves)
			{
				currentOwner = NONE;
			}
			else if (!hasWhiteMoves) 
			{
				currentOwner = BLACK;
			}
			else if (!hasBlackMoves)
			{
				currentOwner = WHITE;
			}
			if (currentOwner == BLACK)
				;
			// Send event only for PC game mode
			else if (gameMode == MODE_PC && currentOwner == WHITE)
			{
				SDL_Event event;
				event.type = SDL_USEREVENT;
				event.user.code = 127;
				event.user.data1 = 0;
				event.user.data2 = 0;
				SDL_PushEvent(&event);
			}
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
		SDL_Rect srcRect;

		memset(&destRect, 0, sizeof(SDL_Rect));
		memset(&srcRect, 0, sizeof(SDL_Rect));

		SDL_BlitSurface(boardSurface, NULL, SurfDisplay, &destRect);

		// Draw 'This is cs50' text
		{
			srcRect = cs50SrcRect;
			destRect = cs50DestRect;
			SDL_BlitSurface(textSurface, &srcRect, 
					SurfDisplay, &destRect);
		}
		// Draw 'Exit' and 'New game' buttons
		{
			srcRect = exitSrcRect;
			destRect = exitDestRect;
			SDL_BlitSurface(textSurface, &srcRect, 
					SurfDisplay, &destRect);

			srcRect = newSrcRect;
			destRect = newDestRect;
			SDL_BlitSurface(textSurface, &srcRect, 
					SurfDisplay, &destRect);
		}

		// Draw current owner field on panel
		{
			destRect = curOwnerDestRect;
			switch (currentOwner)
			{
				case WHITE:
					SDL_BlitSurface(whiteSurface, NULL, 
							SurfDisplay, &destRect);
					break;
				case BLACK:
					SDL_BlitSurface(blackSurface, NULL, 
							SurfDisplay, &destRect);
					break;
				default:
					break;
			}
		}

		// Draw score
		{
			destRect = textDestRect;
			SDL_BlitSurface(scoreSurface, NULL, SurfDisplay, &destRect);

			SDL_FreeSurface(whiteScoreSurface);
			SDL_FreeSurface(blackScoreSurface);

			snprintf(whiteScoreText, 16, 
					"    WHITE - %d\0", scoreForOwner(WHITE));
			snprintf(blackScoreText, 16, 
					"    BLACK - %d\0", scoreForOwner(BLACK));

			whiteScoreSurface = 
				TTF_RenderText_Solid(font, whiteScoreText, fontColor);
			blackScoreSurface = 
				TTF_RenderText_Solid(font, blackScoreText, fontColor);

			destRect = whiteScoreRect;
			SDL_BlitSurface(whiteScoreSurface, NULL, 
					SurfDisplay, &destRect);
			destRect = blackScoreRect;
			SDL_BlitSurface(blackScoreSurface, NULL, 
					SurfDisplay, &destRect);
		}

		// Draw 'Game over' text
		if (currentOwner == NONE)
		{
			srcRect = gameOverSrcRect;
			destRect = gameOverDestRect;
			SDL_BlitSurface(textSurface, &srcRect, SurfDisplay, &destRect);
		}

		for (int i = 0; i < HEIGHT; ++i)
		{
			for (int j = 0; j < WIDTH; ++j)
			{
				switch (board[HEIGHT * i + j].owner)
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

SDL_Surface* loadTextSurface()
{
	SDL_Surface* tmp = IMG_Load("./images/text.png");
	if (tmp == NULL)
	{
		printf("Load text file [%s] failed\n", "./images/text.png");
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

void drawItem(SDL_Surface* srcSurf, SDL_Surface* destSurf, int X, int Y)
{
	SDL_Rect destRect;
	destRect.x = X * FIELD_WIDTH;
	destRect.y = Y * FIELD_WIDTH;
	SDL_BlitSurface(srcSurf, NULL, destSurf, &destRect);
}

int hasNextMove(Owner owner)
{
	Field result = findNextMove(owner);
	return result.X != -1;
}

int flipLines(Field* field)
{
	return 
		flipLine(field, -1, -1) | // Top left
		flipLine(field,  0, -1) | // Top center
		flipLine(field,  1, -1) | // Top right
		flipLine(field, -1,  0) | // Middle left
		flipLine(field,  1,  0) | // Middle right
		flipLine(field, -1,  1) | // Bottom left
		flipLine(field,  0,  1) | // Bottom center
		flipLine(field,  1,  1); // Bottom right
}

int flipLine(Field* field, int dx, int dy)
{
	int curX = field->X + dx;
	int curY = field->Y + dy;
	Field* lastInLine = NULL;
	Field* curField = field;
	Field* foeField = NULL;

	if ((curX >= 0 && curY >= 0)
			&& (curX < WIDTH && curY < HEIGHT))
	{
		if (board[curY * HEIGHT + curX].owner == currentOwner)
			return 0;
	}

	list* l = NULL;
	while ((curX >= 0 && curY >= 0)
			&& (curX < WIDTH && curY < HEIGHT))
	{
		curField = &(board[curY * HEIGHT + curX]);
		if (curField->owner != NONE)
		{
			list* el = makeElement(curField);
			if (!l)
				l = el;
			else
				addElement(l, el);
		}
		else
			break;

		curX += dx;
		curY += dy;
	}

	list* tail = getTail(l);
	if (tail)
	{
		int startToFlip = 0;
		int isFlipped = 0;
		while (tail)
		{
			if (!startToFlip && tail->val->owner == currentOwner)
				startToFlip = 1;
			if (startToFlip)
			{
				putField(board, 
						tail->val->X, tail->val->Y,
						currentOwner);
				isFlipped = 1;
			}

			tail = tail->prev;
		}
		removeList(l);
		return isFlipped;
	}

	return 0;
}

void addElement(list* l, list* el)
{
	if (!el)
		return;

	if (!l)
		l = el;
	else
	{
		list* top = l;
		while (top->next)
			top = top->next;
		top->next = el;
		el->prev = top;
	}
}

list* makeElement(Field* val)
{
	list* element = (list*)malloc(sizeof(list));
	if (element)
	{
		element->val = val;
		element->next = NULL;
		element->prev = NULL;
	}
	return element;
}

void removeList(list* l)
{
	list* top = l;
	while (top->next)
	{
		list* cur = top;
		top = top->next;
		free(cur);
	}
	l = NULL;
}

list* getTail(list* l)
{
	if (!l)
		return NULL;
 	list *top = l;
	while (top->next)
		top = top->next;
	return top;
}

int scoreForOwner(Owner owner)
{
	int score = 0;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (board[HEIGHT * i + j].owner == owner)
				score++;
		}
	}
	return score;
}

Field findNextMove(Owner owner)
{
	Field result;
	result.X = -1;

	Owner prevOwner = currentOwner;
	currentOwner = owner;

	int numFlipped = 0;
	int numPrevFlipped = 0;

	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (board[HEIGHT * i + j].owner != NONE)
				continue;
			if ((numFlipped = canFlipLines(board[HEIGHT * i + j])))
			{
				if (numFlipped > numPrevFlipped)
				{
					result = board[HEIGHT * i + j];
					numPrevFlipped = numFlipped;
				}
			}
		}
	}

	currentOwner = prevOwner;

	return result;
}

int canFlipLines(Field field)
{
	return 
		canFlipLine(field, -1, -1) + // Top left
		canFlipLine(field,  0, -1) + // Top center
		canFlipLine(field,  1, -1) + // Top right
		canFlipLine(field, -1,  0) + // Middle left
		canFlipLine(field,  1,  0) + // Middle right
		canFlipLine(field, -1,  1) + // Bottom left
		canFlipLine(field,  0,  1) + // Bottom center
		canFlipLine(field,  1,  1); // Bottom right
}

int canFlipLine(Field field, int dx, int dy)
{
	int curX = field.X + dx;
	int curY = field.Y + dy;
	Field* lastInLine = NULL;
	Field* curField = &field;
	Field* foeField = NULL;

	if ((curX >= 0 && curY >= 0)
			&& (curX < WIDTH && curY < HEIGHT))
	{
		if (board[curY * HEIGHT + curX].owner == currentOwner)
			return 0;
	}

	list* l = NULL;
	while ((curX >= 0 && curY >= 0)
			&& (curX < WIDTH && curY < HEIGHT))
	{
		curField = &(board[curY * HEIGHT + curX]);
		if (curField->owner != NONE)
		{
			list* el = makeElement(curField);
			if (!l)
				l = el;
			else
				addElement(l, el);
		}
		else
			break;

		curX += dx;
		curY += dy;
	}

	list* tail = getTail(l);
	if (tail)
	{
		int startToFlip = 0;
		int numFlipped = 0;
		while (tail)
		{
			if (!startToFlip && tail->val->owner == currentOwner)
				startToFlip = 1;
			if (startToFlip)
			{
				numFlipped++;
			}

			tail = tail->prev;
		}
		removeList(l);
		return numFlipped;
	}

	return 0;
}

void restoreState()
{
	currentOwner = prevOwner;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
				board[HEIGHT * i + j] = prevBoard[HEIGHT * i + j];
		}
	}
}

void saveState()
{
	prevOwner = currentOwner;
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
				prevBoard[HEIGHT * i + j] = board[HEIGHT * i + j];
		}
	}
}

int isInRect(int x, int y, SDL_Rect rect)
{
	return (
			x >= rect.x && 
			y >= rect.y && 
			x <= (rect.x+rect.w) &&
			y <= (rect.y+rect.h)
		   );
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
	if (scoreSurface)
		SDL_FreeSurface(scoreSurface);
	if (blackScoreSurface)
		SDL_FreeSurface(blackScoreSurface);
	if (whiteScoreSurface)
		SDL_FreeSurface(whiteScoreSurface);

	if (font)
		TTF_CloseFont(font);

	TTF_Quit();
	SDL_Quit();
}

TTF_Font* makeFont(const char* fileName, const unsigned int size)
{
	TTF_Font *newFont = TTF_OpenFont(fileName, size);
	if(!newFont)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	return newFont;
}

void newGame()
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
				board[HEIGHT * i + j].owner = NONE;
		}
	}
	setInitialFields();
	currentOwner = BLACK;
	saveState();
	SDL_WM_SetCaption("BLACK", "Game");
}

void setInitialFields()
{
	putField(board, 3, 3, WHITE);
	putField(board, 4, 4, WHITE);
	putField(board, 3, 4, BLACK);
	putField(board, 4, 3, BLACK);
}

void print_usage(char **argv)
{
	printf("Otello. The game.\n"
			"Usage: %s [flags]\n"
			"\tFlags:\n"
			"\t-h -- print this help and exit\n"
			"\t-f -- fullscreen mode (windowed mode by default)\n"
			"\t-m arg -- game mode ('c' for PC opponent, "
			"'h' - for human)\n"
			"\nThis is CS50\t2013\n",
			argv[0]
			);
}
