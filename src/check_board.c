#include <stdio.h>
#include <stdlib.h>
#include "struct_headers.h"
#include "macros.h"
#include <stdbool.h>

bool check_board(boardObject *game_board) {
    for(int i = 0; i < game_board->cols; i++) {
        for(int j = 0; j < game_board->rows; j++) {
            if(game_board->board[i][j] == EMPTY) 
                return false;  // The board is not completely filled
        }
    }
    return true;
}