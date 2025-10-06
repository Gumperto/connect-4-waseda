#ifndef struct_headers
#define struct_headers

typedef struct boardObjectTag{
   int rows;
   int cols;
   char** board;
}boardObject;

extern boardObject *create_board(int rows, int cols);
extern void free_board(boardObject *game_board);
extern void print_board(boardObject game_board);
extern void fill_board(boardObject *game_board);

#endif
