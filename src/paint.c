#include <stdio.h>
#include <stdbool.h>
#include "struct_headers.h"
#include "macros.h"

//run w/ if(player1->has_won == 1) etc.?
/* something like...
if(player1->has_won){
    label_win_tiles(game_board, PLAYER1_SYMBOL, coordinate->x, coordinate->y, highlight[][] etc. )
}
*/

int highlight[MAX_ROWS][MAX_COLS] = {0};
void label_win_tiles(boardObject *game_board, char wonTile, int lastX, int lastY, int copyBoard[MAX_ROWS][MAX_COLS]) {
    for(int i = 0; i<game_board->rows; i++){
        for(int j = 0; j<game_board->cols; j++){
            copyBoard[i][j] = 0;
        }
    }

    if(game_board->board[lastY][lastX] != wonTile) return;
    //horizontal
    {
        int count = 1;
        int startX = lastX, endX = lastX;
        //right
        int runner = lastX - 1;
        while(runner >= 0 && game_board->board[lastY][runner] == wonTile){
            startX = runner;
            count++;
            runner--;
        }
        //left
        runner = lastX + 1;
        while(runner < game_board->cols && game_board->board[lastY][runner] == wonTile){
            endX = runner;
            count++;
            runner++;
        }
        //label won grid
        if(count>= 4){
            for(int i = startX; i <= endX; i++){
                copyBoard[lastY][i] = 1;
            }
        }

    }
    //vertical
    {
        int count = 1;
        int startY = lastY, endY = lastY;
        //up
        int runner = lastY - 1;
        while(runner >= 0 && game_board->board[runner][lastX] == wonTile){
            startY = runner;
            count++;
            runner--;
        }
        //down
        runner = lastY + 1;
        while(runner < game_board->rows && game_board->board[runner][lastX] == wonTile){
            endY = runner;
            count++;
            runner++;
        }
        //label won grid
        if(count>=4){
            for(int i = startY; i <= endY; i++){
                copyBoard[i][lastX] = 1;
            }
        }
    }
    //diag left
    {
        int count = 1;
        int startX = lastX, endX = lastX;
        int startY = lastY, endY = lastY;
        //up-left
        int runnerX = lastX - 1;
        int runnerY = lastY - 1;
        while(runnerX >= 0 && runnerY >= 0 
            && game_board->board[runnerY][runnerX] == wonTile){
            startX = runnerX;
            startY = runnerY;
            count++;
            runnerX--;
            runnerY--;
        }
        //down-right
        runnerX = lastX + 1;
        runnerY = lastY + 1;
        while(runnerX < game_board->cols 
            && runnerY < game_board->rows 
            && game_board->board[runnerY][runnerX] == wonTile){
            endX = runnerX;
            endY = runnerY;
            count++;
            runnerX++;
            runnerY++;
        }
        //label won grid
        if(count>=4){
            int x = startX, y = startY;
            while(1){
                copyBoard[y][x] = 1;
                if(x == endX && y == endY) break;
                x++;
                y++;
            }
        }
    }
    //diag right
    {
        int count = 1;
        int startX = lastX, endX = lastX;
        int startY = lastY, endY = lastY;
        //up-right
        int runnerX = lastX + 1;
        int runnerY = lastY - 1;
        while(runnerX < game_board->cols && runnerY >= 0 
            && game_board->board[runnerY][runnerX] == wonTile){
            endX = runnerX;
            endY = runnerY;
            count++;
            runnerX++;
            runnerY--;
        }
        //down-left
        runnerX = lastX - 1;
        runnerY = lastY + 1;
        while(runnerX >= 0 && runnerY < game_board->rows 
            && game_board->board[runnerY][runnerX] == wonTile){
            startX = runnerX;
            startY = runnerY;
            count++;
            runnerX--;
            runnerY++;
        }
        //label won grid
        if(count>=4){
            int x = startX, y = startY;
            while(1){
                copyBoard[y][x] = 1;
                if(x == endX && y == endY) break;
                x++;
                y--;
            }
        }
    }
}

