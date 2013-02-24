#ifndef _OTELLO_H
#define _OTELLO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

static unsigned int WIDTH = 8;
static unsigned int HEIGHT = 8;

SDL_Surface* blackSurface;
SDL_Surface* whiteSurface;
SDL_Surface* SurfDisplay;
SDL_Surface* boardSurface;
SDL_Surface* textSurface;

// Exit button rect
static SDL_Rect exitSrcRect  = {  0, 108, 210,  44};
static SDL_Rect exitDestRect = {660, 346, 210,  44};

// New game button rect
static SDL_Rect newSrcRect   = {  0, 152, 210,  44};
static SDL_Rect newDestRect  = {660, 300, 210,  44};

// CS50 text rect
static SDL_Rect cs50SrcRect  = { 10,  70, 180,  38};
static SDL_Rect cs50DestRect = {670, 580,   0,   0};

typedef enum OWNER { 
	NONE = 0, 
	BLACK = 101, 
	WHITE = 102
} Owner;

typedef struct Field {
	int X, Y;
	char owner;
} Field;

Field* board;
Field* prevBoard;
Owner currentOwner;
Owner prevOwner;


Field* createBoard();

void destroyBoard(Field *board);

// Set field with checks
int setField(Field* board, 
		int X, int Y, 
		Owner owner);
// Set field without checks
int putField(Field* board, 
		int X, int Y, 
		Owner owner);

int isPermittedField(const Field field);

int isFieldFree(const Field field); 

void printBoard(Field *board);

int hasNieghbors(const Field field);
int isGoorNeighbor(const int x, const int y);
int hasNextMove(Owner owner);
int flipLines(Field* field);
int flipLine(Field* field, int dx, int dy);
int canFlipLines(Field field);
int canFlipLine(Field field, int dx, int dy);
int scoreForOwner(Owner owner);
Field findNextMove();

void restoreState();
void saveState();

int isInRect(int x, int y, SDL_Rect rect);

// ------- SDL routines ----

int onEvent(SDL_Event *event, Field* board);
void onRender(const Field* board);
SDL_Surface* loadBackground();
SDL_Surface* loadBlack();
SDL_Surface* loadWhite();
SDL_Surface* loadTextSurface();

void drawItem(SDL_Surface* srcSurf, SDL_Surface* destSurf, 
		int X, int Y);

//--------------------------

// linked list rutines

typedef struct list
{
	Field* val;
	struct list* next;
	struct list* prev;
} list;

list* makeElement(Field* val);
void addElement(list* l, list* el);
void removeList(list* l);
list* getTail(list* l);

//

#endif
