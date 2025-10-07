#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"

coordinate *placeTiles(boardObject *game_board, int columnN, int player) {
    coordinate *p_coordinate = malloc(sizeof(coordinate));
    p_coordinate->x = columnN;
    if(player == 1) {
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '0') {
                game_board->board[i][columnN] = 'X';
                p_coordinate->y = i;
                break;
            }
        }
    } else {
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == '0') {
                game_board->board[i][columnN] = 'O';
                p_coordinate->y = i;
                break;
            }
        }
    }
    return p_coordinate;
}
