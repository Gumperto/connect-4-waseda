#ifndef GREETER
#define GREETER
#include <ncurses.h>

extern char greeter(WINDOW* window, int window_height, int window_width, 
                    int window_startx, int window_starty);
extern void fetch_names(char* player1, char* player2);

#endif
