#include "greeter.h"
#include "gamemodes.h"
#include "windows.h"
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

    //while 1 = return value of pvp_mode() and pvbot_mode()
    //1 = restart, 0 = quit
    while(1){
        int game_mode = greeter(game_window, window_height, window_width, window_startx, window_starty);
        int play = 0;
        if (game_mode == 0)
            play = pvp_mode(game_window, window_height, window_width, window_startx, window_starty);
        else if (game_mode == 1)
            play = pvbot_mode(game_window, window_height, window_width, window_startx, window_starty);
        else {
            endwin();
            exit(EXIT_FAILURE);
        }
        if (play == 1) {
            endwin();
            exit(EXIT_SUCCESS);
        }
    }
    del_panel(game_panel);
    destroy_win(game_window);
    endwin();
    exit(EXIT_SUCCESS);
}
