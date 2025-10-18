#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "windows.h"
#include "struct_headers.h"
#include "greeter.h"
#include "macros.h"
#include "leaderboard.h"

#define ARRAY_SIZE 4

int playAgain(WINDOW* window, int game_mode){
    int window_height, window_width;
    getmaxyx(window, window_height, window_width);

    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    box(window, 0, 0);
    print_ascii_art(window, 2, 0, window_width, "./assets/ending.txt", COLOR_PAIR(3));
    wrefresh(window);

    int switch_char;
    int return_index;

    const char* menu_text[ARRAY_SIZE + 1][2] = {
        "Restart", "Restart the game mode",
        "Main menu", "Exit to main menu",
        "Leaderboard", "Check out leaderboard",
        "Quit", "Quit the game",
        (char*) NULL,
    };

    ITEM **menu_items = (ITEM **)calloc(ARRAY_SIZE + 1, sizeof(ITEM *));
    for (int i = 0; i < ARRAY_SIZE; i++) 
        menu_items[i] = new_item(menu_text[i][0], menu_text[i][1]);
    MENU *menu = new_menu(menu_items);

    int display_height = ARRAY_SIZE + 5;
    int display_width = window_width / 3;
    int begin_y = (window_height - display_height) / 2;
    int begin_x = (window_width - display_width) / 2;

    WINDOW* menu_window = derwin(window, display_height, display_width, begin_y, begin_x);
    PANEL* menu_panel = new_panel(menu_window);

    top_panel(menu_panel);

    set_menu_win(menu, menu_window);
    set_menu_sub(menu, derwin(menu_window, ARRAY_SIZE, display_width - 2, 3, 2));

    set_menu_mark(menu, ">>>");

    win_show(menu_window, "End screen", COLOR_PAIR(3));

    post_menu(menu);
    wrefresh(menu_window);

    update_panels();
    doupdate();

	while(1)
	{   
        switch_char = wgetch(window);
        switch(switch_char)
	    {	case KEY_DOWN:
		        menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
            case 10: // Which is <ENTER>
            {
                ITEM* current;

                current = current_item(menu);
                return_index = item_index(current);
                goto cleanup;
            }
            break;
		}
        wrefresh(menu_window);
	}
    
    cleanup:
        unpost_menu(menu);
        for(int i = 0; i < ARRAY_SIZE; ++i)
            free_item(menu_items[i]);
        free_menu(menu);
        del_panel(menu_panel);
        destroy_win(menu_window);
        wclear(window);
        wrefresh(window);
        if (return_index == 0) return game_mode;
        else if (return_index == 1) return MAIN_MENU;
        else if (return_index == 2) return LEADERBOARD;
        else if (return_index == 3) return QUIT;
        else
            return ERROR;
}

void playerPlay(playerData* player, boardObject* game_board, coordinate* recent_coords,
                WINDOW* board_window) {
    int start_y, start_x, end_y, end_x;

    getmaxyx(board_window, end_y, end_x);

    int begin_y = (end_y / 2 + game_board->rows / 2) + 1;
    int begin_x = end_x / 2 - ((game_board->cols * (PADDING_SIZE + 1) - PADDING_SIZE)) / 2;

    coordinate* temp = scanTiles(game_board, player, board_window, begin_y, begin_x);
    recent_coords->x = temp->x;
    recent_coords->y = temp->y;
    player->has_won = check_connect_4(game_board, recent_coords->x, recent_coords->y, player);
    free(temp);
}

void printWrapper(boardObject* game_board, WINDOW* board_window, char* buffer, char* player_name, int turn, bool overlay) {
    if(!overlay){
        wclear(board_window);
    }
    
    if (player_name != NULL) {
        if (turn != -1) {
            snprintf(buffer, MAX_NAME_SIZE, "Turn %d, Player %s", turn, player_name);
            if(!overlay){
                print_board(*game_board, board_window);
            }
        } else {
            snprintf(buffer, MAX_NAME_SIZE, "Player %s won! q to exit", player_name);
        }
    } else {
        if(!overlay){
            print_board(*game_board, board_window);
        }
        
    }

    win_show(board_window, buffer, COLOR_PAIR(3));
}

int pvp_mode(WINDOW* window, int window_height, int window_width, 
            int window_startx, int window_starty) {
    playerData* player1 = malloc(sizeof(playerData)); 
    playerData* player2 = malloc(sizeof(playerData));
    playerData* winner = malloc(sizeof(playerData));

    player1->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);
    player2->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);

    player1->player_id = PLAYER_1;
    player2->player_id = PLAYER_2;

    fetch_names(player1->player_name, player2->player_name, window, 
                window_height, window_width, 
                window_startx, window_starty);

    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    box(window, 0, 0);
    print_ascii_art(window, 2, 0, window_width, "./assets/game.txt", COLOR_PAIR(3));
    wrefresh(window);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    WINDOW* board_window = draw_mini_box(game_board, window, window_height, window_width, window_startx, window_starty);
    PANEL* board_panel = new_panel(board_window);

    char buffer[MAX_NAME_SIZE];
    int turn = 1;

    while (1) {
        printWrapper(game_board, board_window, buffer, player1->player_name, turn, false);
        playerPlay(player1, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (player1->has_won == 1) {
            char wonTile = PLAYER_1_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, player1->player_name, turn = -1, true);
            update_leaderboard(player1->player_name);
            WINDOW* lb_win = derwin(window, 12, 40, 2, 5);
            print_leaderboard(lb_win, player1->player_name, 1);
            winner = player1;
            break;
        }

        printWrapper(game_board, board_window, buffer, player2->player_name, turn, false);
        playerPlay(player2, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (player2->has_won == 1) {
            char wonTile = OPPONENT_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, player2->player_name, turn = -1, true);
            update_leaderboard(player2->player_name);
            WINDOW* lb_win = derwin(window, 12, 40, 2, 5);
            print_leaderboard(lb_win, player2->player_name, 1);
            winner = player2;
            break;
        }
        turn++;
    }

    printWrapper(game_board, board_window, buffer, winner->player_name, turn = -1, false);

    keypad(board_window, TRUE);
    int ch;
    while((ch = wgetch(board_window)) != 'q') continue;

    wclear(board_window);
    wrefresh(board_window);

    del_panel(board_panel);
    destroy_win(board_window);

    if (winner->player_id != BOT && winner->player_id != BOSS) {
        update_leaderboard(winner->player_name);
        print_leaderboard(window, winner->player_name, 1);
    }

    free_board(game_board);
    free(recent_coords);
    free(player1->player_name);
    free(player1);
    free(player2->player_name);
    free(player2);
    wclear(window);
    return playAgain(window, PVP);
}

int pvbot_mode(WINDOW* window, int window_height, int window_width, 
               int window_startx, int window_starty) {
    srand(time(NULL));
    playerData* player = malloc(sizeof(playerData)); 
    playerData* bot = malloc(sizeof(playerData));
    playerData* winner = malloc(sizeof(playerData));

    player->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);
    bot->player_name = "CPU";

    player->player_id = PLAYER_1;
    bot->player_id = BOT;

    fetch_names(player->player_name, NULL, window, 
                window_height, window_width, 
                window_startx, window_starty);

    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    box(window, 0, 0);
    print_ascii_art(window, 2, 0, window_width, "./assets/game.txt", COLOR_PAIR(3));
    wrefresh(window);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    WINDOW* board_window = draw_mini_box(game_board, window, window_height, window_width, window_startx, window_starty);
    PANEL* board_panel = new_panel(board_window);

    char buffer[MAX_NAME_SIZE];
    int turn = 1;

    while (1) {
        printWrapper(game_board, board_window, buffer, player->player_name, turn, false);
        playerPlay(player, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (player->has_won == 1) {
            char wonTile = PLAYER_1_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, player->player_name, turn = -1, true);
            update_leaderboard(player->player_name);
            WINDOW* lb_win = derwin(window, 12, 40, 2, 5);
            print_leaderboard(lb_win, player->player_name, 1);
            winner = player;
            break;
        }
       
        printWrapper(game_board, board_window, buffer, bot->player_name, turn, false);
        playerPlay(bot, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (bot->has_won == 1) {
            char wonTile = OPPONENT_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, bot->player_name, turn = -1, true);
            winner = bot;
            break;
        }
        turn++;
    }

    printWrapper(game_board, board_window, buffer, winner->player_name, turn = -1, false);

    keypad(board_window, TRUE);
    int ch;
    while((ch = wgetch(board_window)) != 'q') continue;

    wclear(board_window);
    wrefresh(board_window);

    del_panel(board_panel);
    destroy_win(board_window);

    if (winner->player_id != BOT && winner->player_id != BOSS) {
        update_leaderboard(winner->player_name);
        print_leaderboard(window, winner->player_name, 1);
    }

    free_board(game_board);
    free(recent_coords);
    free(player->player_name);
    free(player);
    free(bot);
    wclear(window);
    return playAgain(window, PVBOT);
}

//Final Boss Bot
int pvboss_mode(WINDOW* window, int window_height, int window_width, 
                int window_startx, int window_starty) {
    playerData* player = malloc(sizeof(playerData)); 
    playerData* boss = malloc(sizeof(playerData));
    playerData* winner = malloc(sizeof(playerData));

    player->player_name = (char*)malloc(sizeof(char) * MAX_NAME_SIZE);
    boss->player_name = "CPU";

    player->player_id = PLAYER_1;
    boss->player_id = BOSS;

    fetch_names(player->player_name, NULL, window, 
                window_height, window_width, 
                window_startx, window_starty);

    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    box(window, 0, 0);
    print_ascii_art(window, 2, 0, window_width, "./assets/game.txt", COLOR_PAIR(3));
    wrefresh(window);

    coordinate* recent_coords = malloc(sizeof(coordinate));
    boardObject* game_board = create_board(MAX_ROWS,MAX_COLS);
    fill_board(game_board);

    WINDOW* board_window = draw_mini_box(game_board, window, window_height, window_width, window_startx, window_starty);
    PANEL* board_panel = new_panel(board_window);

    char buffer[MAX_NAME_SIZE];
    int turn = 1;

    while (1) {
        printWrapper(game_board, board_window, buffer, player->player_name, turn, false);
        playerPlay(player, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (player->has_won == 1) {
            char wonTile = PLAYER_1_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, player->player_name, turn = -1, true);
            update_leaderboard(player->player_name);      
            winner = player;
            break;
        }
       
        printWrapper(game_board, board_window, buffer, boss->player_name, turn, false);
        playerPlay(boss, game_board, recent_coords, board_window);
        if(check_board(game_board)) {
            printWrapper(game_board, board_window, "Board filled! q to exit", NULL, turn = -1, false);
            break;
        }
        if (boss->has_won == 1) {
            char wonTile = OPPONENT_SYMBOL;
            label_win_tiles(game_board, wonTile, recent_coords->x, recent_coords->y, highlight);
            print_won_board(*game_board, board_window, highlight);
            printWrapper(game_board, board_window, buffer, boss->player_name, turn = -1, true);
            winner = boss;
            break;
        }
        turn++;
    }

    printWrapper(game_board, board_window, buffer, winner->player_name, turn = -1, false);

    keypad(board_window, TRUE);
    int ch;
    while((ch = wgetch(board_window)) != 'q') continue;

    wclear(board_window);
    wrefresh(board_window);

    del_panel(board_panel);
    destroy_win(board_window);

    if (winner->player_id != BOT && winner->player_id != BOSS) {
        update_leaderboard(winner->player_name);
        print_leaderboard(window, winner->player_name, 1);
    }

    free_board(game_board);
    free(recent_coords);
    free(player->player_name);
    free(player);
    free(boss);
    wclear(window);
    return playAgain(window, PVBOSS);
}
