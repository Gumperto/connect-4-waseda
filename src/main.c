#include "greeter.h"
#include "gamemodes.h"
#include "windows.h"
#include "leaderboard.h"
#include "macros.h"
#include <stdbool.h>
#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    initscr();
    noecho();
    start_color();

    int ch;
    int window_height, window_width, window_starty, window_startx;

    window_height = LINES * 4 / 5;
    window_width = COLS * 4 / 5;
    window_starty = (LINES - window_height) / 2;
    window_startx = (COLS - window_width) / 2;

    WINDOW* game_window = create_newwin(window_height, window_width, window_starty, window_startx);
    PANEL* game_panel = new_panel(game_window);
    keypad(game_window, TRUE);

    int game_mode = MAIN_MENU;

    while(1){
        if (game_mode == MAIN_MENU)
            game_mode = greeter(game_window, window_height, window_width, window_startx, window_starty);
        else if (game_mode == PLAYER_2) {
            game_mode = modeChooser(PLAYER_2, game_window, window_height, window_width, window_startx, window_starty);
        }
        else if (game_mode == BOT) {
            game_mode = modeChooser(BOT, game_window, window_height, window_width, window_startx, window_starty);
        }
        else if (game_mode == BOSS) {
            game_mode = modeChooser(BOSS, game_window, window_height, window_width, window_startx, window_starty);
        }
        else if (game_mode == LEADERBOARD) {
            print_leaderboard(game_window, "", 0);
            game_mode = 0;
            continue;
        }
        else if (game_mode == QUIT) {
            break;
        }
        else if (game_mode == RESTART) {
            continue;
        }
        else {
            endwin();
            del_panel(game_panel);
            destroy_win(game_window);
            exit(EXIT_FAILURE);
        }
    }
    del_panel(game_panel);
    destroy_win(game_window);
    endwin();
    exit(EXIT_SUCCESS);
}
