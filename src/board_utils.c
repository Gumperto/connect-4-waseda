#include <stdlib.h>
#include <ncurses.h>
#include "struct_headers.h"
#include "macros.h"
#define MAX_NAME_SIZE 128

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

void fill_board(boardObject *game_board) {
    for (int i = 0; i < game_board->rows; i++)
        for (int j = 0; j < game_board->cols; j++) 
            game_board->board[i][j] = EMPTY;
}

void print_buffer_to_cursor(WINDOW* window) {
    waddch(window, ' ');
    waddch(window, ACS_VLINE);
    waddch(window, ' ');
}

void print_board(boardObject game_board, WINDOW *window) {
    int start_y, start_x, end_y, end_x;
    char buffer[PADDING_SIZE + 1] = "   ";

    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    getmaxyx(window, end_y, end_x);

    int begin_y = end_y / 2 - game_board.rows / 2;
    int begin_x = end_x / 2 - ((game_board.cols * (PADDING_SIZE + 1) - PADDING_SIZE)) / 2;

    for (int i = 0; i < game_board.rows; i++)
        for (int j = 0; j < game_board.cols; j++) {
            if (j == 0) {
                if (game_board.board[i][j] == PLAYER_1_SYMBOL) {
                    wattron(window, COLOR_PAIR(1));
                    mvwaddch(window, begin_y + i, begin_x + j, game_board.board[i][j]);
                    wattroff(window, COLOR_PAIR(1));
                    print_buffer_to_cursor(window);
                }
                else if (game_board.board[i][j] == OPPONENT_SYMBOL) {
                    wattron(window, COLOR_PAIR(2));
                    mvwaddch(window, begin_y + i, begin_x + j, game_board.board[i][j]);
                    wattroff(window, COLOR_PAIR(2));
                    print_buffer_to_cursor(window);
                }
                else {
                    mvwaddch(window, begin_y + i, begin_x + j, game_board.board[i][j]);
                    print_buffer_to_cursor(window);
                }
            }
            else {
                if (j != game_board.cols - 1) {
                    if (game_board.board[i][j] == PLAYER_1_SYMBOL) {
                        wattron(window, COLOR_PAIR(1));
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                        wattroff(window, COLOR_PAIR(1));
                        print_buffer_to_cursor(window);
                    }
                    else if (game_board.board[i][j] == OPPONENT_SYMBOL) {
                        wattron(window, COLOR_PAIR(2));
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                        wattroff(window, COLOR_PAIR(2));
                        print_buffer_to_cursor(window);
                    }
                    else {
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                        print_buffer_to_cursor(window);
                    }
                }
                else {
                    if (game_board.board[i][j] == PLAYER_1_SYMBOL) {
                        wattron(window, COLOR_PAIR(1));
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                        wattroff(window, COLOR_PAIR(1));
                    }
                    else if (game_board.board[i][j] == OPPONENT_SYMBOL) {
                        wattron(window, COLOR_PAIR(2));
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                        wattroff(window, COLOR_PAIR(2));
                    }
                    else {
                        mvwaddch(window, begin_y + i, begin_x + j * (PADDING_SIZE + 1), game_board.board[i][j]);
                    }                    
                }
            }
        }
    wrefresh(window);
}

void free_board(boardObject *game_board) {
    for (int i = 0; i < game_board->rows; i++)
        free(game_board->board[i]);
    free(game_board->board);
    free(game_board);
}
