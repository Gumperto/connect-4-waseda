#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "windows.h"
#include "struct_headers.h"
#include "greeter.h"
#include "macros.h"
#include "leaderboard.h"

static int playAgain(void){
    char input = ' ';
    printw("\nRestart [r], or quit [q]?\n");
    scanw(" %c", &input);
    if(input == 'r' || input == 'R') return 1;
    else if (input == 'Q' || input == 'q') return 0;
    else {
        printw("Invalid input!\n");
        return playAgain();
    }
}

void playerPlay(playerData* player, boardObject* game_board, coordinate* recent_coords) {
    if (player->player_id != BOT) {
        printw("Player %s choice? [column number]: ", player->player_name);
        refresh();
    }

    coordinate* temp = scanTiles(game_board, player);
    refresh();
    recent_coords->x = temp->x;
    recent_coords->y = temp->y;
    printw("\n");
    print_board(*game_board);
    player->has_won = check_connect_4(game_board, recent_coords->x, recent_coords->y, player);
    free(temp);
}

int pvp_mode(WINDOW* window, int window_height, int window_width, 
            int window_startx, int window_starty) {
    playerData* player1 = malloc(sizeof(playerData)); 
    playerData* player2 = malloc(sizeof(playerData));

    player1->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);
    player2->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);

    player1->player_id = PLAYER_1;
    player2->player_id = PLAYER_2;

    fetch_names(player1->player_name, player2->player_name, window, 
                window_height, window_width, 
                window_startx, window_starty);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    int turn = 1;

    while (1) {
        printw("=============\n");
        printw("===Turn %d===\n",turn);
        printw("=============\n");
        print_board(*game_board);

        playerPlay(player1, game_board, recent_coords);
        if (player1->has_won == 1) {
            printw("\n==============\n");
            printw("%s won!\n", player1->player_name);
            printw("==============\n\n");
            update_leaderboard(player1->player_name);
            break;
        }
       
        playerPlay(player2, game_board, recent_coords);
        if (player2->has_won == 1) {
            printw("\n==============\n");
            printw("%s won!\n", player2->player_name);
            printw("==============\n\n");
            update_leaderboard(player2->player_name);
            break;
        }

        turn++;
    }

    printw("Final board:\n");
    print_board(*game_board);

    free_board(game_board);
    free(recent_coords);
    free(player1->player_name);
    free(player1);
    free(player2->player_name);
    free(player2);
    return playAgain();
}

int pvbot_mode(WINDOW* window, int window_height, int window_width, 
               int window_startx, int window_starty) {
    srand(time(NULL));
    playerData* player = malloc(sizeof(playerData)); 
    playerData* bot = malloc(sizeof(playerData));

    player->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);

    player->player_id = PLAYER_1;
    bot->player_id = BOT;

    printw("What's Player 1's name? >>> ");
    scanw("%s", player->player_name);
    refresh();
    player->player_name[strcspn(player->player_name, "\n")] = 0;
    printw("Hello, %s! Your tile is: %c\n", player->player_name, PLAYER_1_SYMBOL);

    printw("The CPU's tile is: %c\n", OPPONENT_SYMBOL);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    int turn = 1;

    while (1) {
        printw("=============\n");
        printw("===Turn %d===\n",turn);
        printw("=============\n");
        print_board(*game_board);

        playerPlay(player, game_board, recent_coords);
        if (player->has_won == 1) {
            printw("\n==============\n");
            printw("%s won!\n", player->player_name);
            printw("==============\n\n");
            update_leaderboard(player->player_name);
            break;
        }
       
        playerPlay(bot, game_board, recent_coords);
        if (bot->has_won == 1) {
            printw("\n==============\n");
            printw("CPU won!\n");
            printw("==============\n\n");
            break;
        }

        turn++;
    }

    printw("Final board:\n");
    print_board(*game_board);

    free_board(game_board);
    free(recent_coords);
    free(player->player_name);
    free(player);
    free(bot);
    return playAgain();
}
