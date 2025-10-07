#include <stdlib.h>
#include <stdio.h>
#include "struct_headers.h"
#define KYEL  "\x1B[33m"
#define KMAG  "\x1B[35m"
#define RESET "\x1B[0m"

boardObject *create_board(int rows, int cols) {
    // Add logic to create to a pointer to boardObject
    boardObject *game_board = malloc(sizeof(boardObject));
    game_board->rows = rows;
    game_board->cols = cols;
    game_board->board = malloc(sizeof(*(game_board->board)) * rows);
    for (int i = 0; i < rows; i++)
        (game_board->board[i]) = malloc(sizeof(char) * cols);
    return game_board;
}

// Test function
void fill_board(boardObject *game_board) {
    for (int i = 0; i < game_board->rows; i++)
        for (int j = 0; j < game_board->cols; j++) 
            game_board->board[i][j] = '0';
}

void print_board(boardObject game_board) {
    for (int i = 0; i < game_board.rows; i++)
        for (int j = 0; j < game_board.cols; j++) {
            if (j == game_board.cols - 1) {
                if (game_board.board[i][j] == 'X')
                    printf(KMAG "%c\n" RESET, game_board.board[i][j]);
                else if (game_board.board[i][j] == 'O')
                    printf(KYEL "%c\n" RESET, game_board.board[i][j]);
                else
                    printf("%c\n", game_board.board[i][j]);
            }
            else {
                if (game_board.board[i][j] == 'X') {
                    printf(KMAG "%c " RESET, game_board.board[i][j]);
                    printf("| ");
                }
                else if (game_board.board[i][j] == 'O') {
                    printf(KYEL "%c " RESET, game_board.board[i][j]);
                    printf("| ");
                }
                else {
                    printf("%c ", game_board.board[i][j]);
                    printf("| ");
                }
            }
        }

    for (int i = 0; i < game_board.rows; i++)
        printf("────");
    printf("─\n");

    for (int i = 1; i <= game_board.rows + 1; i++) {
        if (i == game_board.rows + 1)
            printf("%d\n", i);
        else {
            printf("%d ",i);
            printf("| ");
        }
    }
}

void free_board(boardObject *game_board) {
    for (int i = 0; i < game_board->rows; i++)
        free(game_board->board[i]);
    free(game_board->board);
    free(game_board);
}
