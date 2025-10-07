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
    playerData player1, player2;

    player1.player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);
    player2.player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);

    player1.player_id = PLAYER_1;
    player2.player_id = PLAYER_2;

    greeter(player1.player_name, player2.player_name);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    int turn = 1;

    while (1) {
        printf("=============\n");
        printf("===Turn %d===\n",turn);
        printf("=============\n");
        print_board(*game_board);

        printf("Player %s choice? [column number]: ", player1.player_name);
        recent_coords = scanTiles(game_board, PLAYER_1);
        printf("\n");
        print_board(*game_board);
        player1.has_won = check_connect_4(game_board, recent_coords->x, recent_coords->y, PLAYER_1);
        if (player1.has_won == 1) {
            printf("%s won!\n", player1.player_name);
            break;
        }
       
        printf("Player %s choice? [column number]: ", player2.player_name);
        recent_coords = scanTiles(game_board, PLAYER_2);
        printf("\n");
        print_board(*game_board);
        player2.has_won = check_connect_4(game_board, recent_coords->x, recent_coords->y, PLAYER_2);
        if (player2.has_won == 1) {
            printf("%s won!\n", player2.player_name);
            break;
        }

        turn++;
    }

    free_board(game_board);
    free(recent_coords);
    free(player1.player_name);
    free(player2.player_name);
}
