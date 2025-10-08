#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"
#include "macros.h"

coordinate *placeTiles(boardObject *game_board, int columnN, playerData* player) {
    coordinate *p_coordinate = malloc(sizeof(coordinate));
    p_coordinate->x = columnN;
    if(player->player_id == PLAYER_1) {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '-') {
                game_board->board[i][columnN] = PLAYER_1_SYMBOL;
                p_coordinate->y = i;
                break;
            }
        }
    }
    else {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '-') {
                game_board->board[i][columnN] = OPPONENT_SYMBOL;
                p_coordinate->y = i;
                break;
            }
        }
    }
    return p_coordinate;
}

coordinate* scanTiles(boardObject *game_board, playerData* player) {
    int player_choice, player_index;
    coordinate* recent_coords = malloc(sizeof(coordinate));

    if (player->player_id != BOT)
        scanf("%d", &player_choice);
    else
        player_choice = rand() % 7 + 1;
        
    player_index = player_choice - 1;
    recent_coords = placeTiles(game_board, player_index, player);

    while ((player_choice > game_board->cols || player_choice <= 0) || 
           (recent_coords->y >= game_board->rows || recent_coords->y < 0)) {
      if (player_choice > game_board->cols || player_choice <= 0) {
          if (player->player_id != BOT) {
              printf("Invalid range! Range is 1 -> %d\n", game_board->cols);
              printf("Player %s choice? [column number]: ", player->player_name);
              scanf("%d", &player_choice);
          }
          else
              player_choice = rand() % 7 + 1;
          player_index = player_choice - 1;
          recent_coords = placeTiles(game_board, player_index, player);
      }

      else if (recent_coords->y >= game_board->rows || recent_coords->y < 0) {
          if (player->player_id != BOT) {
              printf("No valid slot in column!\n");
              printf("Player %s choice? [column number]: ", player->player_name);
              scanf("%d", &player_choice);
          }
          else {
              player_choice = rand() % 7 + 1;
          }
          player_index = player_choice - 1;
          recent_coords = placeTiles(game_board, player_index, player);
      }
    }
    return recent_coords;
}
