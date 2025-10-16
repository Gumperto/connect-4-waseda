#ifndef gamemodes
#define gamemodes
#include <ncurses.h>

extern int pvp_mode(WINDOW* window, int window_height, int window_width, 
                    int window_startx, int window_starty);
extern int pvbot_mode(WINDOW* window, int window_height, int window_width, 
                    int window_startx, int window_starty);
extern int pvboss_mode(WINDOW* window, int window_height, int window_width, 
                    int window_startx, int window_starty);

#endif
