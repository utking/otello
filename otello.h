#ifndef _OTELLO_H
#define _OTELLO_H

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

static unsigned int WIDTH = 8;
static unsigned int HEIGHT = 8;

SDL_Surface* blackSurface;
SDL_Surface* whiteSurface;
SDL_Surface* SurfDisplay;
SDL_Surface* boardSurface;

typedef enum OWNER { 
	NONE = 0, 
	BLACK = 101, 
	WHITE = 102
} Owner;

typedef struct Field {
	unsigned int X, Y;
	char owner;
} Field;

Field* board;

Field* createBoard();

void destroyBoard(Field *board);

int setField(Field* board, 
		unsigned int X, unsigned int Y, 
		Owner owner);

int isPermittedField(const Field field);

int isFieldFree(const Field field); 

void printBoard(Field *board);

int hasNieghbors(const Field field);
int isGoorNeighbor(const int x, const int y);

// ------- SDL routines ----

int onEvent(SDL_Event *event, Field* board);
void onRender(const Field* board);
SDL_Surface* loadBackground();
SDL_Surface* loadBlack();
SDL_Surface* loadWhite();

void drawItem(SDL_Surface* srcSurf, SDL_Surface* destSurf, 
		unsigned int X, unsigned int Y);

//--------------------------

#endif
