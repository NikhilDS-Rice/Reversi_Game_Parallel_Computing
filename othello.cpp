#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>
#include <climits>

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

int NegaMaxAlgo(Board b, int color, int depth) {
    if (depth == 0) return EvaluateBoard(b, color);
    
    Board legal_moves;
    int num_moves = EnumerateLegalMoves(b, color, &legal_moves);
    if (num_moves == 0) return -NegaMaxAlgo(b, OTHERCOLOR(color), depth - 1);
    
    cilk::reducer_max<int> maxScore(INT_MIN);
    
    cilk_for(int row = 8; row >= 1; row--) {
        for (int col = 8; col >= 1; col--) {
            if (legal_moves.disks[color] & BOARD_BIT(row, col)) {
                Board next = b;
                Move m1 = {row, col};
                FlipDisks(m1, &next, color, 0, 1);
                int score = -NegaMaxAlgo(next, OTHERCOLOR(color), depth - 1);
                maxScore.calc_max(score);
            }
        }
    }
    return maxScore.get_value();
}

int CompTurn(Board *b, int color, int depth) {
    cilk::reducer_max<int> bestScore(INT_MIN);
    Move bestMove = {-1, -1};
    Board legal_moves;

    int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);
    if (num_moves == 0) {
        printf("Computer has no valid moves.\n");
        return 0;
    }

    cilk_for(int row = 8; row >= 1; row--) {
        for (int col = 8; col >= 1; col--) {
            if (legal_moves.disks[color] & BOARD_BIT(row, col)) {
                Board nextBoard = *b;
                Move m2 = {row, col};
                FlipDisks(m2, &nextBoard, color, 0, 1);
                int score = -NegaMaxAlgo(nextBoard, OTHERCOLOR(color), depth - 1);
                bestScore.calc_max(score);
                if (score == bestScore.get_value()) {
                    bestMove = m2;
                }
            }
        }
    }

    if (bestMove.row != -1 && bestMove.col != -1) {
        printf("Computer places %c at %d, %d.\n", "XO"[color], bestMove.row, bestMove.col);
        FlipDisks(bestMove, b, color, 0, 1);
        PrintBoard(*b);
    } else {
        printf("Computer has no valid moves.\n");
    }
    return 1;
}

int main() {
    Board gameboard = start;
    int depth = 7; // Lookahead depth for AI
    PrintBoard(gameboard);
    int move_possible;
    do {
        move_possible = CompTurn(&gameboard, X_BLACK, depth) | CompTurn(&gameboard, O_WHITE, depth);
    } while (move_possible);
    
    return 0;
}
