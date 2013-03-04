#include <stdio.h>
#include "board.h"
#include "move.h"
#include "gen.h"
#include "print.h"
#include "search.h"

MOVE moves[16384];

int main(void)
{
	BOARD board[1];
	board_init(board);
	while (1) {
		print_board(board);
		input_move(board);
		search(board);
	}
}
