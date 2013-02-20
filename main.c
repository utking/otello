#include <stdio.h>
#include <stdlib.h>

#include "otello.h"

Field *board;

int main(int argc, char *argv[])
{
	printf("%s\n", "Otello. The Game");
	board = createBoard();
	if (!board)
	{
		printf("%s\n", "Can't create board. Terminate.");
		return 1;
	}
	printBoard(board);
	setField(board, 3, 3, WHITE);
	setField(board, 4, 4, BLACK);
	printBoard(board);

	destroyBoard(board);
	return 0;
}

