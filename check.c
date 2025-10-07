#include <stdlib.h>
#include <stdio.h>
#include "struct_headers.h"


int check_horizontal(boardObject *game_board, int x, int y){
    int count = 0;

    if(game_board->board[x][y] == (game_board->board[x-1][y]||game_board->board[x+1][y])){
        if(game_board->board[x-1][y] && game_board->board[x+1][y]){
            count += 3;
            if(game_board->board[x-2][y] || game_board->board[x+2][y]){
                count += 1;
            }
        } else {
            count += 2;
        }
        
    }
    if (count == 4){
        
    }

}