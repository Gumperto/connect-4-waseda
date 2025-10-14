#include "greeter.h"
#include "gamemodes.h"
#include "windows.h"
#include <stdbool.h>
#include <ncurses.h>

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

    refresh();

    WINDOW* game_window = create_newwin(window_height, window_width, window_starty, window_startx);
    keypad(game_window, TRUE);

    //while 1 = return value of pvp_mode() and pvbot_mode()
    //1 = restart, 0 = quit
    while(1){
        char game_mode = greeter(game_window, window_height, window_width, window_startx, window_starty);
        int play = 0;
        if (game_mode == 'p' || game_mode == 'P')
            play = pvp_mode();
        else if (game_mode == 'b' || game_mode == 'B')
            play = pvbot_mode();
        else
            return 1;
        if (play == 0) break;
    }
    endwin();
    return 0;
}
