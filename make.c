#include "board.h"
#include "move.h"

void make_move(BOARD *board, MOVE move)
{
	board->board[MOVE_FROM(move)] = EMPTY;
	board->board[MOVE_TO(move)] = board->stm;
	board->board[MOVE_ARROW(move)] = ARROW;
	for (int i = 0; i < 4; i++) {
		if (board->amazon[board->stm][i] == MOVE_FROM(move))
			board->amazon[board->stm][i] = MOVE_TO(move);
	}
	board->stm = (PIECE)((int)board->stm ^ 1);
}

void unmake_move(BOARD *board, MOVE move)
{
	board->stm = (PIECE)((int)board->stm ^ 1);
	board->board[MOVE_FROM(move)] = board->stm;
	board->board[MOVE_TO(move)] = EMPTY;
	board->board[MOVE_ARROW(move)] = EMPTY;
	for (int i = 0; i < 4; i++) {
		if (board->amazon[board->stm][i] == MOVE_TO(move))
			board->amazon[board->stm][i] = MOVE_FROM(move);
	}
}
