#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct_headers.h"
#include "macros.h"

#define MINMAX_INF 1000000
#define WIN_NUMBER 4
#define DEPTH 3

// Simple Bot Mode
int simpleBot() {
    return rand() % 7 + 1;
}

// Final Boss Mode using Min-max Algorithm

typedef struct best_move {
    int move;
    int score;
} best_move;

/* Helper Functions */
// Check if column is empty
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
    else  game_board->board[row][col] = PLAYER_1_SYMBOL;
}

// Win Check
bool winning_move(boardObject *game_board, int player) {
    char piece;
    if(player == BOSS) piece = OPPONENT_SYMBOL;
    else piece = PLAYER_1_SYMBOL;

    // Check horizontal locations for a win
    for (int r = 0; r < game_board->rows; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            if (game_board->board[r][c] == piece && 
                game_board->board[r][c + 1] == piece && 
                game_board->board[r][c + 2] == piece && 
                game_board->board[r][c + 3] == piece) 
                return true;
        }
    }

    // Check vertical locations for a win
    for (int c = 0; c < game_board->cols; c++) {
        for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
            if (game_board->board[r][c] == piece && 
                game_board->board[r + 1][c] == piece && 
                game_board->board[r + 2][c] == piece && 
                game_board->board[r + 3][c] == piece)
                return true;
        }
    }

    // Check positively sloped diagonals
    for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            if (game_board->board[r][c] == piece && 
                game_board->board[r + 1][c + 1] == piece && 
                game_board->board[r + 2][c + 2] == piece && 
                game_board->board[r + 3][c + 3] == piece) 
                return true;
        }
    }

    // Check negatively sloped diagonals
    for (int r = WIN_NUMBER- 1; r < game_board->rows; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            if (game_board->board[r][c] == piece && 
                game_board->board[r - 1][c + 1] == piece && 
                game_board->board[r - 2][c + 2] == piece && 
                game_board->board[r - 3][c + 3] == piece) 
                return true;
        }
    }

    return 0;
}

// Check if Terminal Node  (Win, Lose or Draw)
bool is_terminal_node(boardObject *game_board) {
    return winning_move(game_board, BOSS) ||  winning_move(game_board, PLAYER_1) || check_board(game_board); 
}

// Maximum
int max(int n1, int n2) {
    return n1 > n2 ? n1 : n2;
}

boardObject *copy_board(boardObject *game_board) {
    boardObject *copy_board = malloc(sizeof(boardObject));
    copy_board->cols = game_board->cols;
    copy_board->rows = game_board->rows;

    copy_board->board = malloc(copy_board->rows * sizeof(char *));
    for (int r = 0; r < copy_board->rows; r++) {
        // CRITICAL FIX: Allocate memory for each individual row
        copy_board->board[r] = malloc(copy_board->cols * sizeof(char));
        for (int c = 0; c < copy_board->cols; c++) {
            copy_board->board[r][c] = game_board->board[r][c];
        }
    }
    return copy_board;
}

int min(int n1, int n2) {
    return n1 < n2 ? n1 : n2;
}

/* Main Functions*/
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

    // Assign scores
    if (piece_count == 4) {
        score += 100;
    } else if (piece_count == 3 && empty_count == 1) {
        score += 5;
    } else if (piece_count == 2 && empty_count == 2) {
        score += 2;
    }

    // Penalize the opponent for having a strong position
    if (opp_piece_count == 3 && empty_count == 1) {
        score -= 4;
    }

    return score;
}

// Scoring System
int score(boardObject *game_board, int player) {
    int score = 0;
    int window[WIN_NUMBER];

    // --- Score Horizontal ---
    for (int r = 0; r < game_board->rows; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r][c + i];
            }
            score += evaluate(window, player);
        }
    }

    // --- Score Vertical ---
    for (int c = 0; c < game_board->cols; c++) {
        for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r + i][c];
            }
            score += evaluate(window, player);
        }
    }

    // --- Score Positive Sloped Diagonal ---
    for (int r = 0; r <= game_board->rows - WIN_NUMBER; r++) {
        for (int c = 0; c <= game_board->cols - WIN_NUMBER; c++) {
            for (int i = 0; i < WIN_NUMBER; i++) {
                window[i] = game_board->board[r + i][c + i];
            }
            score += evaluate(window, player);
        }
    }

    // --- Score Negative Sloped Diagonal ---
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
best_move* minimax (boardObject *game_board, int depth, int alpha, int beta, bool maximizingPlayer) {
    // Base Case
    best_move *move_eval = malloc(sizeof(best_move));
    boardObject *b_copy;
    int temp_score, temp_row;
    bool terminal = is_terminal_node(game_board);

    if(depth == 0 || terminal) {
        if (terminal){
            if(winning_move(game_board, BOSS)) {
                move_eval->move = -1;
                move_eval->score = 1000;
            } else if (winning_move(game_board, PLAYER_1)) {
                move_eval->move = -1;
                move_eval->score = -1000;
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
                temp_row = available_row(game_board, temp_col);
                b_copy = copy_board(game_board);

                place_piece(b_copy, temp_row, temp_col, BOSS);
                best_move *recursive_eval = minimax(b_copy, depth - 1, alpha, beta, false);
                temp_score = recursive_eval->score;
                free(recursive_eval);
                free_board(b_copy);

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
                temp_row = available_row(game_board, temp_col);
                b_copy = copy_board(game_board);

                place_piece(b_copy, temp_row, temp_col, PLAYER_1);
                best_move *recursive_eval = minimax(b_copy, depth - 1, alpha, beta, true);
                temp_score = recursive_eval->score;
                free(recursive_eval);
                free_board(b_copy);

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
    best_move *result = minimax (game_board, 3, -MINMAX_INF, MINMAX_INF, true);
    int best_col = result->move + 1;

    free(result);
    return best_col;
}