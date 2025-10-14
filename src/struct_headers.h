#ifndef struct_headers
#define struct_headers
#include <stdbool.h>


typedef struct boardObjectTag{
   int rows;
   int cols;
   char** board;
}boardObject;

typedef struct turncoordinate{
   int x;
   int y;
} coordinate;

typedef struct playerDataTag{
    char* player_name;
    int score;
    int player_id;
    int has_won;
} playerData;

extern boardObject *create_board(int rows, int cols);
extern void free_board(boardObject *game_board);
extern void print_board(boardObject game_board);
extern void fill_board(boardObject *game_board);
extern bool check_board(boardObject *game_board);
extern coordinate *placeTiles(boardObject *game_board, int columnN, playerData *player);
extern coordinate *scanTiles(boardObject *game_board, playerData *player);
extern int check_connect_4(boardObject* game_board, int x, int y, playerData *player);

#endif
