#ifndef WINDOWS
#define WINDOWS
#include <ncurses.h>
#include <panel.h>
#include <menu.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void print_in_middle(WINDOW* window, int starty, int startx, int box_width, char *string, chtype color);
void print_ascii_art(WINDOW* window, int starty, int startx, int box_width, char *file_name, chtype color);

#endif
