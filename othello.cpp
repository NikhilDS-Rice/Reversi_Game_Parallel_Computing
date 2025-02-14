#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>
#include <cilk/reducer_opadd.h>

#define BIT 0x1

#define X_BLACK 0
#define O_WHITE 1
#define OTHERCOLOR(c) (1-(c))

/* 
    represent game board squares as a 64-bit unsigned integer.
    these macros index from a row,column position on the board
    to a position and bit in a game board bitvector
*/
#define BOARD_BIT_INDEX(row,col) ((8 - (row)) * 8 + (8 - (col)))
#define BOARD_BIT(row,col) (0x1LL << BOARD_BIT_INDEX(row,col))
#define MOVE_TO_BOARD_BIT(m) BOARD_BIT(m.row, m.col)

/* all of the bits in the row 8 */
#define ROW8 \
  (BOARD_BIT(8,1) | BOARD_BIT(8,2) | BOARD_BIT(8,3) | BOARD_BIT(8,4) |	\
   BOARD_BIT(8,5) | BOARD_BIT(8,6) | BOARD_BIT(8,7) | BOARD_BIT(8,8))
              
/* all of the bits in column 8 */
#define COL8 \
  (BOARD_BIT(1,8) | BOARD_BIT(2,8) | BOARD_BIT(3,8) | BOARD_BIT(4,8) |	\
   BOARD_BIT(5,8) | BOARD_BIT(6,8) | BOARD_BIT(7,8) | BOARD_BIT(8,8))

/* all of the bits in column 1 */
#define COL1 (COL8 << 7)

#define IS_MOVE_OFF_BOARD(m) (m.row < 1 || m.row > 8 || m.col < 1 || m.col > 8)
#define IS_DIAGONAL_MOVE(m) (m.row != 0 && m.col != 0)
#define MOVE_OFFSET_TO_BIT_OFFSET(m) (m.row * 8 + m.col)

typedef unsigned long long ull;

/* 
    game board represented as a pair of bit vectors: 
    - one for x_black disks on the board
    - one for o_white disks on the board
*/
typedef struct { ull disks[2]; } Board;

typedef struct { int row; int col; } Move;

Board start = { 
    BOARD_BIT(4,5) | BOARD_BIT(5,4) /* X_BLACK */, 
    BOARD_BIT(4,4) | BOARD_BIT(5,5) /* O_WHITE */
};
 
Move offsets[] = {
  {0,1}		/* right */,		{0,-1}		/* left */, 
  {-1,0}	/* up */,		{1,0}		/* down */, 
  {-1,-1}	/* up-left */,		{-1,1}		/* up-right */, 
  {1,1}		/* down-right */,	{1,-1}		/* down-left */
};

int noffsets = sizeof(offsets)/sizeof(Move);
char diskcolor[] = { '.', 'X', 'O', 'I' };


void PrintDisk(int x_black, int o_white)
{
  printf(" %c", diskcolor[x_black + (o_white << 1)]);
}

void PrintBoardRow(int x_black, int o_white, int disks)
{
  if (disks > 1) {
    PrintBoardRow(x_black >> 1, o_white >> 1, disks - 1);
  }
  PrintDisk(x_black & BIT, o_white & BIT);
}

void PrintBoardRows(ull x_black, ull o_white, int rowsleft)
{
  if (rowsleft > 1) {
    PrintBoardRows(x_black >> 8, o_white >> 8, rowsleft - 1);
  }
  printf("%d", rowsleft);
  PrintBoardRow((int)(x_black & ROW8),  (int) (o_white & ROW8), 8);
  printf("\n");
}

void PrintBoard(Board b)
{
  printf("  1 2 3 4 5 6 7 8\n");
  PrintBoardRows(b.disks[X_BLACK], b.disks[O_WHITE], 8);
}

/* 
    place a disk of color at the position specified by m.row and m,col,
    flipping the opponents disk there (if any) 
*/
void PlaceOrFlip(Move m, Board *b, int color) 
{
  ull bit = MOVE_TO_BOARD_BIT(m);
  b->disks[color] |= bit;
  b->disks[OTHERCOLOR(color)] &= ~bit;
}

/* 
    try to flip disks along a direction specified by a move offset.
    the return code is 0 if no flips were done.
    the return value is 1 + the number of flips otherwise.
*/
int TryFlips(Move m, Move offset, Board *b, int color, int verbose, int domove)
{
  Move next;
  next.row = m.row + offset.row;
  next.col = m.col + offset.col;

  if (!IS_MOVE_OFF_BOARD(next)) {
    ull nextbit = MOVE_TO_BOARD_BIT(next);
    if (nextbit & b->disks[OTHERCOLOR(color)]) {
      int nflips = TryFlips(next, offset, b, color, verbose, domove);
      if (nflips) {
    if (verbose) printf("flipping disk at %d,%d\n", next.row, next.col);
    if (domove) PlaceOrFlip(next,b,color);
    return nflips + 1;
      }
    } else if (nextbit & b->disks[color]) return 1;
  }
  return 0;
} 

int FlipDisks(Move m, Board *b, int color, int verbose, int domove)
{
    int nflips = 0;

    cilk::reducer_opadd<int> reducer_nflips(0);

    cilk_for (int i = 0; i < noffsets; i++) {
        int flipresult = TryFlips(m, offsets[i], b, color, verbose, domove);
        reducer_nflips += (flipresult > 0) ? flipresult - 1 : 0;
    }

    nflips = reducer_nflips.get_value();
    return nflips;
}

void ReadMove(int color, Board *b)
{
  Move m;
  ull movebit;
  for(;;) {
    printf("Enter %c's move as 'row,col': ", diskcolor[color+1]);
    scanf("%d,%d",&m.row,&m.col);
        
    /* if move is not on the board, move again */
    if (IS_MOVE_OFF_BOARD(m)) {
      printf("Illegal move: row and column must both be between 1 and 8\n");
      PrintBoard(*b);
      continue;
    }
    movebit = MOVE_TO_BOARD_BIT(m);
        
    /* if board position occupied, move again */
    if (movebit & (b->disks[X_BLACK] | b->disks[O_WHITE])) {
      printf("Illegal move: board position already occupied.\n");
      PrintBoard(*b);
      continue;
    }
        
    /* if no disks have been flipped */ 
    {
      int nflips = FlipDisks(m, b,color, 1, 1);
      if (nflips == 0) {
    printf("Illegal move: no disks flipped\n");
    PrintBoard(*b);
    continue;
      }
      PlaceOrFlip(m, b, color);
      printf("You flipped %d disks\n", nflips);
      PrintBoard(*b);
    }
    break;
  }
}

/*
    return the set of board positions adjacent to an opponent's
    disk that are empty. these represent a candidate set of 
    positions for a move by color.
*/
Board NeighborMoves(Board b, int color)
{
  int i;
  Board neighbors = {0,0};
  for (i = 0;i < noffsets; i++) {
    ull colmask = (offsets[i].col != 0) ? 
      ((offsets[i].col > 0) ? COL1 : COL8) : 0;
    int offset = MOVE_OFFSET_TO_BIT_OFFSET(offsets[i]);

    if (offset > 0) {
      neighbors.disks[color] |= 
    (b.disks[OTHERCOLOR(color)] >> offset) & ~colmask;
    } else {
      neighbors.disks[color] |= 
    (b.disks[OTHERCOLOR(color)] << -offset) & ~colmask;
    }
  }
  neighbors.disks[color] &= ~(b.disks[X_BLACK] | b.disks[O_WHITE]);
  return neighbors;
}

/*
    return the set of board positions that represent legal
    moves for color. this is the set of empty board positions  
    that are adjacent to an opponent's disk where placing a
    disk of color will cause one or more of the opponent's
    disks to be flipped.
*/
int EnumerateLegalMoves(Board b, int color, Board *legal_moves)
{
    static Board no_legal_moves = {0,0};
    Board neighbors = NeighborMoves(b, color);
    ull my_neighbor_moves = neighbors.disks[color];
    int num_moves = 0;
    *legal_moves = no_legal_moves;

    cilk::reducer_opadd<int> reducer_num_moves(0);
    cilk::reducer<cilk::op_add<ull>> reducer_legal_moves(0);

    cilk_for (int row = 8; row >= 1; row--) {
        ull thisrow = my_neighbor_moves & ROW8;
        for (int col = 8; thisrow && (col >= 1); col--) {
            if (thisrow & COL8) {
                Move m = { row, col };
                if (FlipDisks(m, &b, color, 0, 0) > 0) {
                    reducer_legal_moves->value |= BOARD_BIT(row, col);
                    reducer_num_moves++;
                }
            }
            thisrow >>= 1;
        }
        my_neighbor_moves >>= 8;
    }

    legal_moves->disks[color] = reducer_legal_moves.get_value();
    num_moves = reducer_num_moves.get_value();

    return num_moves;
}


int HumanTurn(Board *b, int color)
{
  Board legal_moves;
  int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);
  if (num_moves > 0) {
    ReadMove(color, b);
    return 1;
  } else return 0;
}

int CountBitsOnBoard(Board *b, int color)
{
    ull bits = b->disks[color];
    int ndisks = 0;

    cilk::reducer_opadd<int> reducer_ndisks(0);

    cilk_for (int i = 0; i < 64; i++) {
        if (bits & (1ULL << i)) {
            reducer_ndisks += 1;
        }
    }

    ndisks = reducer_ndisks.get_value();
    return ndisks;
}

void EndGame(Board b)
{
  int o_score = CountBitsOnBoard(&b,O_WHITE);
  int x_score = CountBitsOnBoard(&b,X_BLACK);
  printf("Game over. \n");
  if (o_score == x_score)  {
    printf("Tie game. Each player has %d disks\n", o_score);
  } else { 
    printf("X has %d disks. O has %d disks. %c wins.\n", x_score, o_score, 
          (x_score > o_score ? 'X' : 'O'));
  }
}

int EvaluateBoard(Board b, int color) {
    // Simple evaluation function: difference in the number of disks
    return CountBitsOnBoard(&b, color) - CountBitsOnBoard(&b, OTHERCOLOR(color));
}

bool isGameOver(Board b) {
    // Check if the game is over (no legal moves for both players)
    Board legal_moves;
    return EnumerateLegalMoves(b, X_BLACK, &legal_moves) == 0 && EnumerateLegalMoves(b, O_WHITE, &legal_moves) == 0;
}

int NegaMaxAlgo(Board b, int color, int depth) {
    if (depth == 0 || isGameOver(b)) {
        return EvaluateBoard(b, color);
    }

    Board legal_moves;
    int num_moves = EnumerateLegalMoves(b, color, &legal_moves);

    if (num_moves == 0) {
        return -NegaMaxAlgo(b, OTHERCOLOR(color), depth - 1);
    }

    cilk::reducer_max<int> maxScore(INT_MIN);

    cilk_for (int row = 8; row >= 1; row--) {
        for (int col = 8; col >= 1; col--) {
            if (legal_moves.disks[color] & BOARD_BIT(row, col)) {
                Board next = b;
                Move m1 = {row, col};
                PlaceOrFlip(m1, &next, color);
                int score = -NegaMaxAlgo(next, OTHERCOLOR(color), depth - 1);
                maxScore.calc_max(score);
            }
        }
    }
    return maxScore.get_value();
}

int CompTurn(Board *b, int color, int depth) {
    cilk::reducer_max<int> bestScore(INT_MIN);
    cilk::reducer<cilk::op_add<Move>> bestMove({-1, -1});
    Board legal_moves;

    int num_moves = EnumerateLegalMoves(*b, color, &legal_moves);
    if (num_moves == 0) {
        printf("Computer has no valid moves.\n");
        return 0;
    }

    cilk_for (int row = 8; row >= 1; row--) {
        for (int col = 8; col >= 1; col--) {
            if (legal_moves.disks[color] & BOARD_BIT(row, col)) {
                Board nextBoard = *b;
                Move m2 = {row, col};
                PlaceOrFlip(m2, &nextBoard, color);
                int score = -NegaMaxAlgo(nextBoard, OTHERCOLOR(color), depth - 1);
                if (score > bestScore.get_value()) {
                    bestScore.set_value(score);
                    bestMove.set_value(m2);
                }
            }
        }
    }

    if (bestMove.get_value().row != -1 && bestMove.get_value().col != -1) {
        printf("Computer places %c at %d, %d.\n", diskcolor[color+1], bestMove.get_value().row, bestMove.get_value().col);
        PlaceOrFlip(bestMove.get_value(), b, color);
        FlipDisks(bestMove.get_value(), b, color, 0, 1);
        PrintBoard(*b);
    } else {
        printf("Computer has no valid moves.\n");
    }
    return 1;
}


int main (int argc, const char * argv[]) 
{
  Board gameboard = start;
  int move_possible;
  PrintBoard(gameboard);
  do {
    move_possible = 
      HumanTurn(&gameboard, X_BLACK) | 
      HumanTurn(&gameboard, O_WHITE);
  } while(move_possible);
    
  EndGame(gameboard);
    
  return 0;
}