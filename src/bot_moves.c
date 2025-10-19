#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct_headers.h"
#include "macros.h"

#define MINMAX_INF 1000000
#define WIN_NUMBER 4

/* Simple Bot Mode */ 
int simpleBot() {
    return rand() % 7;
}

/* Final Boss Mode using Min-max Algorithm*/ 
typedef struct best_move {
    int move;
    int score;
} best_move;

/* Helper Functions */
// Check Column
bool is_column_empty(boardObject *game_board, int col) {
    return col >= 0 && col < game_board->cols && game_board->board[0][col] == EMPTY;
}

// Placing Hypothetical Piece
void place_piece(boardObject *game_board, int row, int col, int player) {
    if(player == BOSS) game_board->board[row][col] = OPPONENT_SYMBOL;
    else game_board->board[row][col] = PLAYER_1_SYMBOL;
}

// Check if Terminal Node = Win, Lose or Draw
bool is_terminal_node(boardObject *game_board, coordinate *c) {
    if (c->x != -1 && c->y != -1) {
        if (check_connect_4(game_board, c->x, c->y, BOSS)) 
            return true;
        else if (check_connect_4(game_board, c->x, c->y, PLAYER_1))
            return true;
    }

    if(check_board(game_board)) 
        return true;

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
int evaluate(int window[], int player) {
    int piece = OPPONENT_SYMBOL, 
        opp_piece = PLAYER_1_SYMBOL;

    if(player == PLAYER_1) {
        opp_piece = OPPONENT_SYMBOL;
        piece = PLAYER_1_SYMBOL;
    }

    int score = 0, 
        piece_count = 0, 
        opp_piece_count = 0,
        empty_count = 0;

    for (int i = 0; i < WIN_NUMBER; i++) {
        if (window[i] == piece) {
            piece_count++;
        } else if (window[i] == opp_piece) {
            opp_piece_count++;
        } else {
            empty_count++;
        }
    }

    if (piece_count == 4) {
        score += 1000;
    } else if (piece_count == 3 && empty_count == 1) {
        score += 50;
    } else if (piece_count == 2 && empty_count == 2) {
        score += 20;
    }

    // Losing Position
    if (opp_piece_count == 3 && empty_count == 1) {
        score -= 1000;
    }

    return score;
}

int score(boardObject *game_board, int player) {
    int score = 0;

    int window[WIN_NUMBER];

    // Prioritise Center
    int center_count = 0;
    for (int r = 0; r < game_board->rows; r++) {
        if ((game_board->board[r][game_board->cols / 2] == OPPONENT_SYMBOL) || 
            (game_board->board[r][game_board->cols / 2 - 1] == OPPONENT_SYMBOL) || 
            (game_board->board[r][game_board->cols / 2 + 1] == OPPONENT_SYMBOL)) {
            center_count++;
        }       
    }
    score += center_count * 30;

    // Checking Pieces Position Advantage
    // Horizontal
    for (int r = 0; r < game_board->rows; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r][c + i];
            }
            score += evaluate(window, player);
        }
    }

    // Vertical
    for (int c = 0; c < game_board->cols; c++) {
        for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r + i][c];
            }
            score += evaluate(window, player);
        }
    }

    // Positive Diagonal
    for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r + i][c + i];
            }
            score += evaluate(window, player);
        }
    }

    // Negative Diagonal
    for (int r = WIN_NUMBER - 1; r < game_board->rows; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r - i][c + i];
            }
            score +=evaluate(window, player);
        }
    }
     
    return score;
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
        
        // Checking All Available Columns
        for(int temp_col = 0; temp_col < game_board->cols; temp_col++) {
            if(is_column_empty(game_board, temp_col)) {
                b_copy = copy_board(game_board);


                coordinate *temp_1 = placeTiles(b_copy, temp_col, BOSS);
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
        
        // Checking All Available Columns
        for(int temp_col = 0; temp_col < game_board->cols; temp_col++) {
            if(is_column_empty(game_board, temp_col)) {
                b_copy = copy_board(game_board);

                coordinate *temp_1 = placeTiles(b_copy, temp_col, PLAYER_1);
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
