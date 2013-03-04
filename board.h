typedef enum { WHITE, BLACK, ARROW, EMPTY } PIECE;

typedef unsigned char SQUARE;

#define BOARD_WIDTH 10
#define BOARD_SIZE ((BOARD_WIDTH+2)*(BOARD_WIDTH+2))

typedef struct
{
	PIECE board[BOARD_SIZE];
	SQUARE amazon[2][4];
	PIECE stm;
} BOARD;
