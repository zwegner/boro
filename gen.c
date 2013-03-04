#include "board.h"
#include "move.h"
#include "gen.h"

const int directions[] = { -13, -12, -11, -1, 1, 11, 12, 13 };

#define FOR_EACH_DIR(d) for(int _d = 0,d; (d) = directions[_d], _d < 8; _d++)

MOVE *gen_moves(BOARD *board, MOVE *first_move)
{
	for (int i = 0; i < 4; i++) {
		SQUARE from = board->amazon[board->stm][i];
		FOR_EACH_DIR(d) {
			SQUARE to = from;
			PIECE old_piece = board->board[from];
			board->board[from] = EMPTY;
			while (1) {
				to += d;
				if (board->board[to] == EMPTY) {
					FOR_EACH_DIR(ad) {
						SQUARE arrow = to;
						while (1) {
							arrow += ad;
							if (board->board[arrow] == EMPTY)
								*first_move++ = MOVE_CONSTRUCT(from, to, arrow);
							else
								break;
						}
					}
				} else
				   	break;
			}
			board->board[from] = old_piece;
		}
	}
	return first_move;
}
