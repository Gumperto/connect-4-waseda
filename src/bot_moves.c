#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct_headers.h"
#include "macros.h"

#define MINMAX_INF 1000000
#define WIN_NUMBER 4

// Simple Bot Mode
int simpleBot() {
    return rand() % 7;
}

// Final Boss Mode using Min-max Algorithm
typedef struct best_move {
    int move;
    int score;
} best_move;

/* Helper Functions */
bool is_column_empty(boardObject *game_board, int col) {
    return col >= 0 && col < game_board->cols && game_board->board[0][col] == EMPTY;
}

// Check the available row in column
int available_row(boardObject *game_board, int col) {
    for (int r = game_board->rows - 1; r >= 0; r--) {
        if (game_board->board[r][col] == EMPTY)
            return r;
    }
    return -1;
}

// Placing Hypothetical Piece
void place_piece(boardObject *game_board, int row, int col, int player) {
    if(player == BOSS) game_board->board[row][col] = OPPONENT_SYMBOL;
    else game_board->board[row][col] = PLAYER_1_SYMBOL;
}

// Check if Terminal Node = Win, Lose or Draw
bool is_terminal_node(boardObject *game_board, coordinate *c) {
    if (c->x != -1 && c->y != -1) {
        if (check_connect_4(game_board, c->x, c->y, BOSS)) {
            printf("BOSS WIN\n"); 
            return true;
        } else if (check_connect_4(game_board, c->x, c->y, PLAYER_1)) {
            printf("PLAYER WIN\n");
            return true;
        }
    }

    if(check_board(game_board)) {
            printf("DRAW\n");
            return true;
    }
    
    return false;
}

// Maximum
int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

int min(int n1, int n2) {
    return n1 < n2 ? n1 : n2;
}

boardObject *copy_board(boardObject *game_board) {
    boardObject *copy_board = malloc(sizeof(boardObject));
    copy_board->cols = game_board->cols;
    copy_board->rows = game_board->rows;

    copy_board->board = malloc(copy_board->rows * sizeof(char *));
    for (int r = 0; r < copy_board->rows; r++) {
        copy_board->board[r] = malloc(copy_board->cols * sizeof(char));
        for (int c = 0; c < copy_board->cols; c++) {
            copy_board->board[r][c] = game_board->board[r][c];
        }
    }
    return copy_board;
}

/* Main Functions*/
// Scoring System
int score(boardObject *game_board, int player) {
    int piece = OPPONENT_SYMBOL, 
        opp_piece = PLAYER_1_SYMBOL,
        opp_score = 0, 
        my_score = 0;

    if(player == PLAYER_1) {
        opp_piece = OPPONENT_SYMBOL;
        piece = PLAYER_1_SYMBOL;
    }
    int window[WIN_NUMBER];
    int weighting[6][7] = {{3, 4, 5, 7, 5, 4, 3},
                            {4, 6, 7, 10, 7, 6, 4},
                            {5, 7, 11, 13, 11, 7, 5},
                            {5, 7, 11, 13, 11, 7, 5}, 
                            {4, 6, 8, 10, 8, 6, 4},
                            {3, 4, 5, 7, 5, 4, 3}};

    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 7; j++) {
            if(game_board->board[i][j] == piece) my_score += weighting[i][j];
            else if(game_board->board[i][j] == opp_piece) opp_score += weighting[i][j];
        }
    }
    return my_score - opp_score;
}

// Min Max 
best_move* minimax (boardObject *game_board, int depth, int alpha, int beta, bool maximizingPlayer, coordinate *temp) {
    // Base Case
    best_move *move_eval = malloc(sizeof(best_move));
    boardObject *b_copy;
    int temp_score, temp_row;
    bool terminal = is_terminal_node(game_board, temp);

    if(depth == 0 || terminal == true) {
        if (terminal){
            printf("what\n");
            if (temp->x != -1) { 
                if(check_connect_4(game_board, temp->x, temp->y, BOSS)) {
                    move_eval->move = -1;
                    move_eval->score = 1000;
                } else if (check_connect_4(game_board, temp->x, temp->y, PLAYER_1)) {
                    move_eval->move = -1;
                    move_eval->score = -1000;
                } else {
                    move_eval->move = -1;
                    move_eval->score = 0;
                }
            } else {
                move_eval->move = -1;
                move_eval->score = 0;
            }
        } else {
            move_eval->move = -1;
            move_eval->score = score(game_board, BOSS);
        }
        
        return move_eval;
    }

    // Maximising Player
    if(maximizingPlayer) {
        move_eval->score = -MINMAX_INF;
        move_eval->move = -1;
        
        // Checking through the columns
        for(int temp_col = 0; temp_col < game_board->cols; temp_col++) {
            if(is_column_empty(game_board, temp_col)) {
                // temp_row = available_row(game_board, temp_col);
                b_copy = copy_board(game_board);


                coordinate *temp_1 = placeTiles(b_copy, temp_col, BOSS);
                // place_piece(b_copy, temp_row, temp_col, BOSS);
                best_move *recursive_eval = minimax(b_copy, depth - 1, alpha, beta, false, temp_1);
                temp_score = recursive_eval->score;
                free(recursive_eval);
                free_board(b_copy);
                free(temp_1);

                if(temp_score > move_eval->score) {
                    move_eval->score = temp_score;
                    move_eval->move = temp_col;
                }

                alpha = max(alpha, move_eval->score);

                if(alpha >= beta) break;
            }
        }
    } else { // Minimising Player
        move_eval->score = MINMAX_INF;
        move_eval->move = -1;
        
        // Checking through the columns
        for(int temp_col = 0; temp_col < game_board->cols; temp_col++) {
            if(is_column_empty(game_board, temp_col)) {
                // temp_row = available_row(game_board, temp_col);
                b_copy = copy_board(game_board);

                coordinate *temp_1 = placeTiles(b_copy, temp_col, PLAYER_1);
                // place_piece(b_copy, temp_row, temp_col, PLAYER_1);
                best_move *recursive_eval = minimax(b_copy, depth - 1, alpha, beta, true, temp_1);
                temp_score = recursive_eval->score;
                free(recursive_eval);
                free_board(b_copy);
                free(temp_1);

                if(temp_score < move_eval->score) {
                    move_eval->score = temp_score;
                    move_eval->move = temp_col;
                }
                beta = min(beta, move_eval->score);
                if(alpha >= beta) break;
            }                
        }
    }
    return move_eval;
}

int finalBoss(boardObject *game_board) {
    coordinate *test = malloc(sizeof(coordinate));
    test->x = -1;
    test->y = -1;
    best_move *result = minimax (game_board, 4, -MINMAX_INF, MINMAX_INF, true, test);
    int best_col = result->move;

    free(test);
    free(result);
    return best_col;
}
