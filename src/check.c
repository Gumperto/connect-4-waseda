#include "struct_headers.h"
#include "macros.h"
#define WIN_NUMBER 4
#include <stdbool.h>
#include <stdlib.h>


int check_horizontal(boardObject *game_board, int x, int y, int player);
int check_vertical(boardObject *game_board, int x, int y, int player);
int check_diag_left(boardObject *game_board, int x, int y, int player);
int check_diag_right(boardObject *game_board, int x, int y, int player);

int check_connect_4(boardObject* game_board, int x, int y, playerData* player) {
    winType* win_type = (winType*)malloc(sizeof(winType));
    win_type->horizontal = false;
    win_type->vertical = false;
    win_type->diag_left = false;
    win_type->diag_right = false;
    
    if(check_horizontal(game_board, x, y, player->player_id) >= WIN_NUMBER) win_type->horizontal = true;
    if(check_vertical(game_board, x, y, player->player_id) >= WIN_NUMBER) win_type->vertical = true;
    if(check_diag_left(game_board, x, y, player->player_id) >= WIN_NUMBER) win_type->diag_left = true;
    if(check_diag_right(game_board, x, y, player->player_id) >= WIN_NUMBER) win_type->diag_right = true;
    if (win_type->horizontal == true || win_type->vertical == true || win_type->diag_left == true || win_type->diag_right == true) return 1;
    return 0;
}


int check_horizontal(boardObject *game_board, int x, int y, int player){
    char increment_char;
    if (player == PLAYER_1)
        increment_char = PLAYER_1_SYMBOL;
    else
        increment_char = OPPONENT_SYMBOL;

    int count = 0;
    // Run left
    int runner = x;
    while (runner >= 0) {
        if (game_board->board[y][runner] != increment_char) break;
        count++;
        runner--;
    }

    // Run right
    runner = x + 1;
    while (runner < game_board->cols) {
        if (game_board->board[y][runner] != increment_char) break;
        count++;
        runner++;
    }
    return count;
}

int check_vertical(boardObject *game_board, int x, int y, int player) {
    char increment_char;
    if (player == PLAYER_1)
        increment_char = PLAYER_1_SYMBOL;
    else
        increment_char = OPPONENT_SYMBOL;

    int count = 0;
    // Run up
    int runner = y;
    while (runner >= 0) {
        if (game_board->board[runner][x] != increment_char) break;
        count++;
        runner--;
    }

    // Run down
    runner = y + 1;
    while (runner < game_board->rows) {
        if (game_board->board[runner][x] != increment_char) break;
        count++;
        runner++;
    }
    return count;
}

int check_diag_left(boardObject *game_board, int x, int y, int player) {
    char increment_char;
    if (player == PLAYER_1)
        increment_char = PLAYER_1_SYMBOL;
    else
        increment_char = OPPONENT_SYMBOL;

    int count = 0;

    // Run up-left
    int runner_x = x;
    int runner_y = y;
    
    while (runner_x >= 0 && runner_y >= 0) {
        if (game_board->board[runner_y][runner_x] != increment_char) break;
        count++;
        runner_x--;
        runner_y--;
    }

    // Run down-right
    runner_x = x + 1;
    runner_y = y + 1;
    
    while (runner_x < game_board->cols && runner_y < game_board->rows) {
        if (game_board->board[runner_y][runner_x] != increment_char) break;
        count++;
        runner_x++;
        runner_y++;
    }
    return count;
}

int check_diag_right(boardObject *game_board, int x, int y, int player) {
    char increment_char;
    if (player == PLAYER_1)
        increment_char = PLAYER_1_SYMBOL;
    else
        increment_char = OPPONENT_SYMBOL;
    
    int count = 0;

    // Run up-right
    int runner_x = x;
    int runner_y = y;
    
    while (runner_x < game_board->cols && runner_y >= 0) {
        if (game_board->board[runner_y][runner_x] != increment_char) break;
        count++;
        runner_x++;
        runner_y--;
    }

    // Run down-left
    runner_x = x - 1;
    runner_y = y + 1;
    
    while (runner_x >= 0 && runner_y < game_board->rows) {
        if (game_board->board[runner_y][runner_x] != increment_char) break;
        count++;
        runner_x--;
        runner_y++;
    }
    return count;
}
