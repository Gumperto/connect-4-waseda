#include "greeter.h"
#include "windows.h"
#include "macros.h"
#include <menu.h>
#include <panel.h>
#include <ncurses.h>
#include <stdlib.h>

#define MAX_NAME_SIZE 128
#define ARRAY_SIZE 4
#define TEMP_HACK_FOR_LONGEST_IN_MENU 36

int greeter(WINDOW *window, int window_height, int window_width, int window_startx, int window_starty) {
    int switch_char;
    char game_mode = -1;
    int half_height = window_height / 2;
    curs_set(0);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    print_ascii_art(window, 1, 0, window_width, "./assets/splash.txt", COLOR_PAIR(1));

    const char* menu_text[ARRAY_SIZE + 1][2] = {
        "PvP", "Play against a player",
        "PvBot", "Play against a bot",
        "Leaderboard", "Check your wins",
        "Exit", "Exit game",
        (char*) NULL,   
    };

    ITEM **menu_items = (ITEM **)calloc(ARRAY_SIZE, sizeof(ITEM *));
    for (int i = 0; i < ARRAY_SIZE; i++) 
        menu_items[i] = new_item(menu_text[i][0], menu_text[i][1]);
    MENU *menu = new_menu(menu_items);

    set_menu_win(menu, window);
    set_menu_sub(menu, derwin(window, ARRAY_SIZE + 1, TEMP_HACK_FOR_LONGEST_IN_MENU, half_height, (window_width - TEMP_HACK_FOR_LONGEST_IN_MENU) / 2));

    set_menu_mark(menu, ">>>");

    mvwprintw(window, window_height - 2, 2, "Press <ENTER> to select");
    post_menu(menu);
    wrefresh(window);


	while((switch_char = wgetch(window)) != KEY_F(1))
	{   switch(switch_char)
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
                game_mode = item_index(current);
                wprintw(window, "%d", game_mode);
                goto cleanup;
            }
            break;
		}
        wrefresh(window);
	}
    
    cleanup:
        unpost_menu(menu);
        for(int i = 0; i < ARRAY_SIZE; ++i)
            free_item(menu_items[i]);
        free_menu(menu);
        wclear(window);
        wrefresh(window);
        return game_mode;
}

void fetch_names(char *player1, char *player2, WINDOW* window, 
                 int window_height, int window_width, 
                 int window_startx, int window_starty) {
    echo();
    box(window, 0, 0);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    print_ascii_art(window, 2, 0, window_width, "./assets/name.txt", COLOR_PAIR(3));
    wrefresh(window);
    int display_height = 5;
    int horizontal_length = window_width * 2 / 3;
    int horizontal_begin = (window_width - horizontal_length) / 2;

    if (player1 != NULL) {
        WINDOW* win_player_1 = derwin(window, display_height, horizontal_length,
                                     (window_height - display_height) / 2, horizontal_begin);
        init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
        win_show(win_player_1, "Player 1 name", COLOR_PAIR(1));
        mvwprintw(win_player_1, display_height - 2, 2, "Input Player 1's (%c) name >>> ", PLAYER_1_SYMBOL);
        wrefresh(win_player_1);
        curs_set(1);
        wscanw(win_player_1, "%s", player1);
        curs_set(0);
        destroy_win(win_player_1);
    }

    if (player2 != NULL) {
        WINDOW* win_player_2 = derwin(window, display_height, horizontal_length,
                                     (window_height - display_height) / 2, horizontal_begin);
        init_pair(2, COLOR_YELLOW, COLOR_BLACK);
        win_show(win_player_2, "Player 2 name", COLOR_PAIR(2));
        mvwprintw(win_player_2, display_height - 2, 2, "Input Player 2's (%c) name >>> ", OPPONENT_SYMBOL);
        wrefresh(win_player_2);
        curs_set(1);
        wscanw(win_player_2, "%s", player2);
        curs_set(0);
        destroy_win(win_player_2);
    }

    noecho();
}
