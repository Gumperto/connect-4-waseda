#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"
#include "greeter.h"

#define MAX_ROWS 6
#define MAX_COLS 7
#define PLAYER_1 1
#define PLAYER_2 2
#define MAX_NAME_SIZE 128

int main() {
   char* player1 = malloc(sizeof(char) * MAX_NAME_SIZE);
   char* player2 = malloc(sizeof(char) * MAX_NAME_SIZE);

   greeter(player1, player2);

   coordinate* recent_coords = malloc(sizeof(coordinate));
   boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
   fill_board(game_board);

   while (1) {
      printf("Player %s choice: ", player1);
      scanTiles(game_board, PLAYER_1);
      print_board(*game_board);
      // Add check logic

      printf("Player %s choice: ", player2);
      scanTiles(game_board, PLAYER_2);
      print_board(*game_board);
      // Add check logic
   }

   free_board(game_board);
   free(recent_coords);
   free(player1);
   free(player2);
}
