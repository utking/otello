#ifndef _OTELLO_H
#define _OTELLO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

typedef enum GAME_MODE 
{
	MODE_PC = 10, MODE_HUMAN = 20 
} GameMode;

typedef enum GAME_TACTIC
{
	T_BEST = 21, T_WORST = 31, T_FIRST = 41, T_RANDOM = 51
} GameTactic;

unsigned int WND_WIDTH;
unsigned int WND_HEIGHT;

unsigned int WIDTH;
unsigned int HEIGHT;
unsigned int FIELD_WIDTH;

GameMode gameMode;
GameTactic gameTactic;

char whiteScoreText[16];
char blackScoreText[16];

SDL_Surface* blackSurface;
SDL_Surface* whiteSurface;
SDL_Surface* SurfDisplay;
SDL_Surface* boardSurface;
SDL_Surface* textSurface;

SDL_Surface* whiteScoreSurface;
SDL_Surface* blackScoreSurface;

/* Score font and text */
TTF_Font *font;
SDL_Color fontColor;
SDL_Rect textDestRect;
SDL_Rect whiteScoreRect;
SDL_Rect blackScoreRect;

/* Score image rect */
SDL_Rect scoreSrcRect;

/* Game over rect */
SDL_Rect gameOverSrcRect;
SDL_Rect gameOverDestRect;

/* On mode switcher rect */
SDL_Rect swOnModeSrcRect;
SDL_Rect swOnModeDestRect;

/* Off mode switcher rect */
SDL_Rect swOffModeSrcRect;
SDL_Rect swOffModeDestRect;

/* Human mode rect */
SDL_Rect humanModeSrcRect;
SDL_Rect humanModeDestRect;

/* PC mode rect */
SDL_Rect pcModeSrcRect;
SDL_Rect pcModeDestRect;

/* Exit button rect */
SDL_Rect exitSrcRect;
SDL_Rect exitDestRect;

/* 'Current' text rect */
SDL_Rect currentSrcRect;
SDL_Rect currentDestRect;

/* Current owner on panel rect */
SDL_Rect curOwnerDestRect;

/* New game button rect */
SDL_Rect newSrcRect;
SDL_Rect newDestRect;

/* CS50 text rect */
SDL_Rect cs50SrcRect;
SDL_Rect cs50DestRect;

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

/* Set field with checks */
int setField(Field* board, 
		int X, int Y, 
		Owner owner);
/* Set field without checks */
void putField(Field* board, 
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
Field findNextMove(Owner owner);

Field findBestMove(Owner owner);
Field findWorstMove(Owner owner);
Field findFirstAvailMove(Owner owner);
Field findRandomMove(Owner owner);

void restoreState();
void saveState();
void setInitialFields();
void newGame();

int isInRect(int x, int y, SDL_Rect rect);

void print_usage(char **argv);
void cleanup();
TTF_Font* makeFont(const char* fileName, const unsigned int size);

/* ------- SDL routines ---- */

int onEvent(SDL_Event *event, Field* board);
void onRender(const Field* board);
SDL_Surface* loadColor(const char *);
SDL_Surface* loadBackground();
SDL_Surface* loadBlack();
SDL_Surface* loadWhite();
SDL_Surface* loadTextSurface();

void drawItem(SDL_Surface* srcSurf, SDL_Surface* destSurf, 
		int X, int Y);

/* -------------------------- */

/* linked list rutines */

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

#endif
