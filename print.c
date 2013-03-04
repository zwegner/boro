#include <stdio.h>
#include "board.h"
#include "move.h"
#include "gen.h"
#include "print.h"

void print_board(BOARD *board)
{
	char piece_char[] = "WBx-";
	for (int i = 11; i >= 0; i--) {
		if (i == 0 || i == 11)
			printf("    ");
		else
			printf(" %2i ", i);
		for (int j = 0; j < 12; j++)
			printf("%c ", piece_char[board->board[i*12+j]]);
		printf("\n");
	}
	printf("      ");
	for (int j = 0; j < 10; j++)
		printf("%c ", j + 'a');
	printf("\n");
}

#define RANK_CHAR(x) ('a'+((x)%12)-1)
#define FILE_CHAR(x) (((x)/12))

void print_move(MOVE move)
{
	printf("%c%i-%c%i/%c%i",
			RANK_CHAR(MOVE_FROM(move)),
			FILE_CHAR(MOVE_FROM(move)),
			RANK_CHAR(MOVE_TO(move)),
			FILE_CHAR(MOVE_TO(move)),
			RANK_CHAR(MOVE_ARROW(move)),
			FILE_CHAR(MOVE_ARROW(move)));
}

void board_init(BOARD *board)
{
	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			if (i == 0 || i == 11 || j == 0 || j == 11)
				board->board[i*12+j] = ARROW;
			else
				board->board[i*12+j] = EMPTY;
		}
	}
	board->board[1*12+4] = WHITE;
	board->board[1*12+7] = WHITE;
	board->board[4*12+1] = WHITE;
	board->board[4*12+10] = WHITE;

	board->board[10*12+4] = BLACK;
	board->board[10*12+7] = BLACK;
	board->board[7*12+1] = BLACK;
	board->board[7*12+10] = BLACK;

	board->stm = WHITE;

	int idx[2] = {0,0};

	for (int i = 0; i < 12; i++) {
		for (int j = 0; j < 12; j++) {
			SQUARE s = i*12+j;
			PIECE p = board->board[s];
			if (p < 2)
				board->amazon[p][idx[p]++] = s;
		}
	}
}

void input_move(BOARD *board)
{
	MOVE move_list[2200];
	MOVE *move, *last_move;
	char str[256];
	char f1, f2, f3;
	int r1, r2, r3;
	int from, to, arrow;

	while (1) {
		printf("(enter move): ");
		fgets(str, sizeof(str), stdin);
		sscanf(str, "%c%i%c%i%c%i", &f1, &r1, &f2, &r2, &f3, &r3);
		from = (f1-'a'+1) + 12 * r1;
		to = (f2-'a'+1) + 12 * r2;
		arrow = (f3-'a'+1) + 12 * r3;
		MOVE input_move = MOVE_CONSTRUCT(from, to, arrow);

		last_move = gen_moves(board, move_list);
		for (move = move_list; move < last_move; move++) {
			if (*move == input_move) {
				printf("got move: ");
				print_move(input_move);
				puts("");
				make_move(board, input_move);
				return;
			}
		}
		printf("bad move!\n");
	}
}
