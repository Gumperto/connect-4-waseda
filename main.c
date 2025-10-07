#include <stdio.h>
#include "struct_headers.h"
#define MAX_ROWS 6
#define MAX_COLS 7

int main() {
   char player1[20], player2[20];

   boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);

   // This is a test function to see if everything is working
   fill_board(game_board);
   print_board(*game_board);

   printf("Welcome to Connect 4!\n");
   printf("What's Player 1's name?\n");
   scanf("%s", &player1);
   printf("Hello, %s! Your tile is: ○\n", player1);
   printf("What's Player 2's name?\n");
   scanf("%s", &player2);
   printf("Hello, %s! Your tile is: ⚫︎\n", player2);
   
   // Add driver logic based on pseudocode
   while (1) {
      print_board(*game_board);

    // ADD STUFF
   }

   free_board(game_board);
}
