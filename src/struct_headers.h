#ifndef struct_headers
#define struct_headers

typedef struct boardObjectTag{
   int rows;
   int cols;
   char** board;
}boardObject;

typedef struct turncoordinate{
   int x;
   int y;
} coordinate;

extern boardObject *create_board(int rows, int cols);
extern void free_board(boardObject *game_board);
extern void print_board(boardObject game_board);
extern void fill_board(boardObject *game_board);
extern coordinate *placeTiles(boardObject *game_board, int columnN, int player);
extern coordinate *scanTiles(boardObject *game_board, int player);

#endif
