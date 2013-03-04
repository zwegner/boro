#define MOVE_FROM(m)   ((m) & 255)
#define MOVE_TO(m)     (((m) >> 8) & 255)
#define MOVE_ARROW(m)  (((m) >> 16) & 255)

#define MOVE_CONSTRUCT(f,t,a) (((a)<<16)|((t)<<8)|(f))

typedef unsigned int MOVE;

void make_move(BOARD *board, MOVE move);
void unmake_move(BOARD *board, MOVE move);
