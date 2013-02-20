#ifndef _OTELLO_H
#define _OTELLO_H

#include <stdlib.h>
#include <stdio.h>

static unsigned int WIDTH = 8;
static unsigned int HEIGHT = 8;

typedef enum OWNER { 
	NONE = 0, 
	BLACK = 101, 
	WHITE = 102
} Owner;

typedef struct Field {
	unsigned int X, Y;
	char owner;
} Field;

Field* createBoard();

void destroyBoard(Field *board);

int setField(Field* board, 
		unsigned int X, unsigned int Y, 
		Owner owner);

int isPermittedField(const Field field);

int isFieldFree(const Field field); 

void printBoard(Field *board);

#endif
