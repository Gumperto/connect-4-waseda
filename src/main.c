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

   int turn = 1;

   while (1) {
       printf("=============\n");
       printf("===Turn %d===\n",turn);
       printf("=============\n");
       print_board(*game_board);

       printf("Player %s choice? [column number]: ", player1);
       recent_coords = scanTiles(game_board, PLAYER_1);
       printf("\n");
       print_board(*game_board);
       printf("%s state: %d\n", player1, check_connect_4(game_board, recent_coords->x, recent_coords->y, PLAYER_1));
       // Add check logic
       
       printf("Player %s choice? [column number]: ", player2);
       recent_coords = scanTiles(game_board, PLAYER_2);
       printf("\n");
       print_board(*game_board);
       printf("%s state: %d\n", player2, check_connect_4(game_board, recent_coords->x, recent_coords->y, PLAYER_2));
       // Add check logic

       turn++;
   }

   free_board(game_board);
   free(recent_coords);
   free(player1);
   free(player2);
}
