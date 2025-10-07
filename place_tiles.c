#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"

coordinate *placeTiles(boardObject *game_board, int columnN, int player) {
    coordinate *p_coordinate = malloc(sizeof(coordinate));
    p_coordinate->x = columnN;
    if(player == 1) {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '-') {
                game_board->board[i][columnN] = 'X';
                p_coordinate->y = i;
                break;
            }
        }
    }
    else {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '-') {
                game_board->board[i][columnN] = 'O';
                p_coordinate->y = i;
                break;
            }
        }
    }
    return p_coordinate;
}

coordinate* scanTiles(boardObject *game_board, int player) {
    int player_choice, player_index;
    coordinate* recent_coords = malloc(sizeof(coordinate));
    scanf("%d", &player_choice);
    player_index = player_choice - 1;
    recent_coords = placeTiles(game_board, player_index, player);

    while ((player_choice > game_board->cols || player_choice < 0)
            || (recent_coords->y >= game_board->rows || recent_coords->y < 0)) {
      if (player_choice > game_board->cols || player_choice < 0) {
          printf("Invalid range! Range is 1 -> %d\n", game_board->cols);
          printf("Player %d choice? [column number]: ", player);
          scanf("%d", &player_choice);
          player_index = player_choice - 1;
          recent_coords = placeTiles(game_board, player_index, player);
      }

      else if (recent_coords->y >= game_board->rows || recent_coords->y < 0) {
          printf("No valid slot in column!\n");
          printf("Player %d choice? [column number]: ", player);
          scanf("%d", &player_choice);
          player_index = player_choice - 1;
          recent_coords = placeTiles(game_board, player_index, player);
      }
    }
    return recent_coords;
}
