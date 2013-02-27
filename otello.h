#ifndef _OTELLO_H
#define _OTELLO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

typedef enum GAME_MODE { 
	MODE_PC = 10, MODE_HUMAN = 20 
} GameMode;

static unsigned int WND_WIDTH  = 890;
static unsigned int WND_HEIGHT = 640;

static unsigned int WIDTH = 8;
static unsigned int HEIGHT = 8;
static unsigned int FIELD_WIDTH = 80;

static int isFullScreen = 0;
static GameMode gameMode = MODE_PC;

static char whiteScoreText[16];
static char blackScoreText[16];

SDL_Surface* blackSurface;
SDL_Surface* whiteSurface;
SDL_Surface* SurfDisplay;
SDL_Surface* boardSurface;
SDL_Surface* textSurface;

SDL_Surface* scoreSurface;
SDL_Surface* whiteScoreSurface;
SDL_Surface* blackScoreSurface;

// Score font and text
TTF_Font *font;
static SDL_Color fontColor = {20, 20, 20, 0};
static SDL_Rect textDestRect   = {670, 100, 210,  44};
static SDL_Rect whiteScoreRect = {670, 130, 210,  44};
static SDL_Rect blackScoreRect = {670, 160, 210,  44};

// Game over rect
static SDL_Rect gameOverSrcRect  = {  0,   0, 210,  35};
static SDL_Rect gameOverDestRect = {660,  40, 210,  35};

// Exit button rect
static SDL_Rect exitSrcRect  = {  0, 108, 210,  44};
static SDL_Rect exitDestRect = {660, 246, 210,  44};

// Current owner on panel rect
static SDL_Rect curOwnerDestRect = {730, 390, 0, 0};

// New game button rect
static SDL_Rect newSrcRect   = {  0, 152, 210,  44};
static SDL_Rect newDestRect  = {660, 200, 210,  44};

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
SDL_Event event;

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

int hasNieghbors(const Field field);
int isGoodNeighbor(const int x, const int y);
int hasNextMove(Owner owner);
int flipLines(Field* field);
int flipLine(Field* field, int dx, int dy);
int canFlipLines(Field field);
int canFlipLine(Field field, int dx, int dy);
int scoreForOwner(Owner owner);
Field findNextMove();

void restoreState();
void saveState();
void setInitialFields();
void newGame(GameMode mode);

int isInRect(int x, int y, SDL_Rect rect);

void print_usage(char **argv);
void cleanup();
TTF_Font* makeFont(const char* fileName, const unsigned int size);

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
