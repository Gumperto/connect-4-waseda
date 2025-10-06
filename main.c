#include <stdio.h>
#include "struct_headers.h"
#define MAX_ROWS 6
#define MAX_COLS 7

int main() {
   boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);

   // This is a test function to see if everything is working
   fill_board(game_board);
   print_board(*game_board);

   // Add driver logic based on pseudocode
   // while (game is not over)
   //   print_board(*game_board);
   //   play_game(...);
   //   check_state_for_win(...);
   //   ...
   //
   free_board(game_board);
}
