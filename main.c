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
   while (1) {
    print_board(*game_board);
    // ADD STUFF
   }

   free_board(game_board);
}
