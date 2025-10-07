#include <stdlib.h>
#include <stdio.h>
#include "struct_headers.h"


int check_horizontal(boardObject *game_board, int x, int y){
    int count = 0;
    //if the newest placement is left-most column
    if(x == 0){
        for(int a = 1; a<4; a++){
            if(game_board->board[x][y] == game_board->board[x+a][y]){
                count++;
            } else {
                //doesn't win this round
                return 0;
            }
        }
    }

    //if the newest placement is right-most column
    if(x == game_board->cols-1){
                for(int a = 1; a<4; a++){
            if(game_board->board[x][y] == game_board->board[x-a][y]){
                count++;
            } else {
                //doesn't win this round
                return 0;
            }
        }
    } 
    
    //if the placement is anywhere else in the row
    else if(game_board->board[x][y] == (game_board->board[x-1][y]||game_board->board[x+1][y])){
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
        //you win this round
        return 1;
    }
    

}


int check_vertical(boardObject *game_board, int x, int y){
    int count = 0;
    //if the newest placement is left-most column
    if(y == 0){
        for(int a = 1; a<4; a++){
            if(game_board->board[x][y] == game_board->board[x][y+a]){
                count++;
            } else {
                //doesn't win this round
                return 0;
            }
        }
    }

    //if the newest placement is right-most column
    if(y == game_board->rows-1){
                for(int a = 1; a<4; a++){
            if(game_board->board[x][y] == game_board->board[x][y-a]){
                count++;
            } else {
                //doesn't win this round
                return 0;
            }
        }
    } 
    
    //if the placement is anywhere else in the row
    else if(game_board->board[x][y] == (game_board->board[x][y-1]||game_board->board[x][y+1])){
        if(game_board->board[x][y-1] && game_board->board[x][y+1]){
            count += 3;
            if(game_board->board[x][y-2] || game_board->board[x][y+2]){
                count += 1;
            }
        } else {
            count += 2;
        }
        
    }
    if (count == 4){
        //you win this round
        return 1;
    }
}