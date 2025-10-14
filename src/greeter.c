#include "greeter.h"
#include "macros.h"
#include "windows.h"
#include <menu.h>
#include <panel.h>
#include <string.h>
#include <ncurses.h>
#include <stdlib.h>

#define MAX_NAME_SIZE 128
#define ARRAY_SIZE 4
#define TEMP_HACK_FOR_LONGEST_IN_MENU 36

char greeter(WINDOW *window, int window_height, int window_width, int window_startx, int window_starty) {
    int switch_char;
    char game_mode;
    int half_height = window_height / 2;
    curs_set(0);
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    print_ascii_art(window, 1, 0, window_width, "ascii_art.txt", COLOR_PAIR(1));
    wrefresh(window);

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
            case 10:
            {
                ITEM* current;
                void (*ptr)(char *);

                current = current_item(menu);
                ptr = item_userptr(current);
                ptr((char*)item_name(current));
                pos_menu_cursor(menu);
                break;
            }
            break;
		}
        wrefresh(window);
	}

    unpost_menu(menu);
    for(int i = 0; i < ARRAY_SIZE; ++i)
            free_item(menu_items[i]);
    free_menu(menu);
    return game_mode;
}

void fetch_names(char *player1, char *player2) {
    printw("===NAME SELECTION===\n");
    printw("\n");
    refresh();

    printw("What's Player 1's name? >>> ");
    refresh();
    scanw("%s",player1);
    refresh();
    player1[strcspn(player1, "\n")] = 0;
    while (strcspn(player1, ",") != strlen(player1))
        player1[strcspn(player1, ",")] = ' ';
    printw("Hello, %s! Your tile is: %c\n", player1, PLAYER_1_SYMBOL);
    refresh();

    printw("\n");

    printw("What's Player 2's name? >>> ");
    refresh();
    scanw("%s",player2);
    refresh();
    player2[strcspn(player2, "\n")] = 0;
    while (strcspn(player2, ",") != strlen(player2))
        player2[strcspn(player2, ",")] = ' ';
    printw("Hello, %s! Your tile is: %c\n", player2, OPPONENT_SYMBOL);    
    refresh();
}
