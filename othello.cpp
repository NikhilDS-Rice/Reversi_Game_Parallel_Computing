#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>

#define BIT 0x1
#define X_BLACK 0
#define O_WHITE 1
#define OTHERCOLOR(c) (1-(c))

#define BOARD_BIT_INDEX(row,col) ((8 - (row)) * 8 + (8 - (col)))
#define BOARD_BIT(row,col) (0x1LL << BOARD_BIT_INDEX(row,col))
#define MOVE_TO_BOARD_BIT(m) BOARD_BIT(m.row, m.col)

typedef unsigned long long ull;

typedef struct { ull disks[2]; } Board;
typedef struct { int row; int col; } Move;

Board start = { 
    BOARD_BIT(4,5) | BOARD_BIT(5,4), /* X_BLACK */
    BOARD_BIT(4,4) | BOARD_BIT(5,5)  /* O_WHITE */
};

Move offsets[] = {
    {0,1}, {0,-1}, {-1,0}, {1,0},
    {-1,-1}, {-1,1}, {1,1}, {1,-1}
};

int noffsets = sizeof(offsets)/sizeof(Move);

void PrintBoard(Board b);
int FlipDisks(Move m, Board *b, int color, int verbose, int domove);
int EnumerateLegalMoves(Board b, int color, Board *legal_moves);

int EvaluateBoard(Board b, int color) {
    return __builtin_popcountll(b.disks[color]) - __builtin_popcountll(b.disks[OTHERCOLOR(color)]);
}

int Negamax(Board b, int color, int depth) {
    if (depth == 0) return EvaluateBoard(b, color);
    
    Board legal_moves;
    int num_moves = EnumerateLegalMoves(b, color, &legal_moves);
    if (num_moves == 0) return -Negamax(b, OTHERCOLOR(color), depth - 1);
    
    cilk::reducer_max<int> best_score(-10000);
    
    for (int row = 1; row <= 8; row++) {
        for (int col = 1; col <= 8; col++) {
            Move m = {row, col};
            ull movebit = MOVE_TO_BOARD_BIT(m);
            if (movebit & legal_moves.disks[color]) {
                Board new_board = b;
                FlipDisks(m, &new_board, color, 0, 1);
                cilk_spawn {
                    int score = -Negamax(new_board, OTHERCOLOR(color), depth - 1);
                    best_score.calc_max(score);
                }
            }
        }
    }
    cilk_sync;
    return best_score.get_value();
}

Move BestMove(Board b, int color, int depth) {
    Board legal_moves;
    EnumerateLegalMoves(b, color, &legal_moves);
    
    cilk::reducer_max<int> best_score(-10000);
    Move best_move = {-1, -1};
    
    for (int row = 1; row <= 8; row++) {
        for (int col = 1; col <= 8; col++) {
            Move m = {row, col};
            ull movebit = MOVE_TO_BOARD_BIT(m);
            if (movebit & legal_moves.disks[color]) {
                Board new_board = b;
                FlipDisks(m, &new_board, color, 0, 1);
                cilk_spawn {
                    int score = -Negamax(new_board, OTHERCOLOR(color), depth - 1);
                    if (score > best_score.get_value()) {
                        best_move = m;
                        best_score.calc_max(score);
                    }
                }
            }
        }
    }
    cilk_sync;
    return best_move;
}

int ComputerTurn(Board *b, int color, int depth) {
    Move best_move = BestMove(*b, color, depth);
    if (best_move.row != -1) {
        FlipDisks(best_move, b, color, 1, 1);
        return 1;
    }
    return 0;
}

int main() {
    Board gameboard = start;
    int depth = 5; // Lookahead depth for AI
    PrintBoard(gameboard);
    int move_possible;
    do {
        move_possible = ComputerTurn(&gameboard, X_BLACK, depth) | ComputerTurn(&gameboard, O_WHITE, depth);
    } while (move_possible);
    
    return 0;
}
