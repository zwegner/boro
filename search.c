#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "board.h"
#include "move.h"
#include "gen.h"
#include "search.h"
#include "print.h"

typedef struct NODE NODE;

struct NODE
{
	MOVE move;
	struct NODE *child;
	struct NODE *sibling;
	int wins;
	int games;
	int solved;
};

NODE *node_list = NULL;
int node_count;

#define ARRAY_LENGTH(n)	(sizeof(n)/sizeof(n[0]))

int expand(BOARD *board, NODE *parent)
{
	static MOVE moves[2200];
	MOVE *last_move, *move;
	NODE *child;

	last_move = gen_moves(board, moves);

	for (move = moves; move < last_move; move++) {
		child = &node_list[node_count++];
		child->sibling = parent->child;
		parent->child = child;

		child->move = *move;
	}
	return last_move != moves;
}

void init_search(BOARD *board)
{
	NODE *parent;
	static int node_size = 0;
	if (node_size == 0) {
		node_size = 1<<24;
		node_list = (NODE *)malloc(node_size *sizeof(NODE));
		if (!node_list) {
			perror("malloc failed");
			exit(1);
		}
	}

	for (int i = 0; i < node_size; i++) {
		node_list[i].move = 0;
		node_list[i].child = NULL;
		node_list[i].sibling = NULL;
		node_list[i].wins = 0;
		node_list[i].games = 0;
		node_list[i].solved = 0;
	}
	node_count = 1;
}

int backup_wins;
int backup_games;

void playout(BOARD *board)
{
	static MOVE move_list[2200];
	MOVE *move, *last;
	PIECE root_stm = board->stm;
	int move_nb;

	while (1) {
		last = gen_moves(board, move_list);
		if (last == move_list) {
			backup_wins = (board->stm != root_stm);
			backup_games = 1;
			return;
		}
		move_nb = random() % (last - move_list);
		make_move(board, move_list[move_nb]);
	}
}

void uct_search(BOARD *board, NODE *node)
{
	NODE *child, *best;
	float uct, best_uct;
	int solved_value;
	int solved;

	best = node->child;
	if (node->games < 20) {
		playout(board);
		node->games += backup_games;
		node->wins += backup_wins;
		return;
	}
	if (node->child == NULL) {
		if (!expand(board, node)) {
			backup_wins = 0;
			backup_games = 0;
			node->wins = 0;
			node->games = 1e9;
			node->solved = 1;
			return;
		}
	}

	best_uct = -1;
	best = NULL;
	solved_value = 0;
	solved = 1;
	for (child = node->child; child; child = child->sibling) {
		if (child->solved) {
			if (child->wins ^ 1 > solved_value)
				solved_value = child->wins ^ 1;
			continue;
		} else
			solved = 0;
		if (child->games == 0) {
			best = child;
			break;
		}
		uct = child->wins / child->games + .4 * sqrt(logf(node->games) /
				child->games);
		if (uct > best_uct) {
			best_uct = uct;
			best = child;
		}
	}
	if (solved) {
		node->solved = 1;
		node->wins = solved_value;
		return;
	}
	if (!best) {
        *NULL;
		return;
	}
	make_move(board, best->move);
	uct_search(board, best);
	if (backup_games) {
		node->wins += backup_wins;
		node->games += backup_games;
		backup_wins ^= 1;
	}
}

MOVE print_pv(BOARD *board)
{
	NODE *node = node_list;
	NODE *best, *child;
	float best_win;
	MOVE first = 0;
	int c = 0;

	while (1) {
		best_win = 0;
		best = NULL;
		for (child = node->child; child; child = child->sibling) {
			if (child->solved && child->wins > 0) {
				best = child;
				goto found;
			}
			if ((float)child->wins / child->games > best_win) {
				best = child;
				best_win = (float)child->wins / child->games;
			}
		}
found:
		if (!best)
			break;
		if (!first)
			first = best->move;
		printf("(%i/%i)", best->wins, best->games);
		print_move(best->move);
		make_move(board, best->move);
		if (c++ > 10)
			break;
		node = best;
	}
	printf("\n");
	return first;
}

void search(BOARD *board)
{
	BOARD backup;

	init_search(board);

	for (int playouts = 0; playouts < 100000; playouts++) {
		backup = *board;
		uct_search(&backup, &node_list[0]);
		if (!(playouts & 4095)) {
			backup = *board;
			printf("playouts: %10i   (%i/%i)", playouts,
					node_list->wins, node_list->games);
			print_pv(&backup);
		}
	}
	backup = *board;
	MOVE best = print_pv(&backup);
	make_move(board, best);
}
