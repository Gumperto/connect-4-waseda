#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"
#define MAX_ROWS 6
#define MAX_COLS 7
#define PLAYER_1 1
#define PLAYER_2 2

int main() {
   char player1[20], player2[20];
   int player1_choice, player2_choice;
   int player1_index, player2_index;

   coordinate* recent_coords = malloc(sizeof(coordinate));

   boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
   fill_board(game_board);

   printf("===Welcome to Connect 4!===\n");

   printf("What's Player 1's name? >>> ");
   scanf("%s", player1);
   printf("Hello, %s! Your tile is: X\n", player1);

   printf("What's Player 2's name? >>> ");
   scanf("%s", player2);
   printf("Hello, %s! Your tile is: O\n", player2);

   // Add driver logic based on pseudocode
   while (1) {
      printf("Player %d choice: ", PLAYER_1);
      scanTiles(game_board, PLAYER_1);
      print_board(*game_board);
      // Add check logic

      printf("Player %d choice: ", PLAYER_2);
      scanTiles(game_board, PLAYER_2);
      print_board(*game_board);
      // Add check logic
   }

   free_board(game_board);
   free(recent_coords);
}
