#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <string.h>
#include <stdlib.h>
#include "windows.h"
#include "macros.h"

#define WIDTH 80

WINDOW *create_newwin(int height, int width, int starty, int startx){
    WINDOW* local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    return local_win;
}

void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wclear(local_win);
    wrefresh(local_win);
    delwin(local_win);
}

void print_in_middle(WINDOW* window, int starty, int startx, int box_width, char *string, chtype color) {
   int str_length, x, y;
   float temp;

   if (window == NULL)
       window = stdscr;

   getyx(window, y, x);

   if (startx != 0)
       x = startx;
   if (starty != 0)
       y = starty;
   if (box_width == 0)
       box_width = WIDTH;

   str_length = strlen(string);
   temp = (box_width - str_length) / 2;
   x = startx + (int)temp;

   wattron(window, color);
   mvwprintw(window, y, x, "%s", string);
   wattroff(window, color);
   wrefresh(window);
}


void win_show(WINDOW *win, char *label, int label_color) {	
    int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE); // First line
	mvwhline(win, 2, 1, ACS_HLINE, width - 2); // Second line
	mvwaddch(win, 2, width - 1, ACS_RTEE); // Last line
	
	print_in_middle(win, 1, 0, width, label, label_color);
}

void print_ascii_art(WINDOW* window, int starty, int startx, int box_width, char *file_name, chtype color) {
    int index = starty;
    FILE* file_ptr = fopen(file_name, "r");
    char* line = malloc(sizeof(char) * MAX_NAME_SIZE);

    if (file_ptr != NULL) {
        while(fgets(line, MAX_NAME_SIZE, file_ptr)) {
            line[strcspn(line, "\n")] = 0;
            print_in_middle(window, index, startx, box_width, line, color);
            index++;
        }
        fclose(file_ptr);
    }
    else {
        free(line);
        return;
    }
    free(line);
}

void error_window(char *err_message) {
    int ch;
    init_pair(4, COLOR_RED, COLOR_BLACK);
    WINDOW* err_window = newwin(5, COLS / 2, (LINES - 5) / 2, COLS / 4);
    keypad(err_window, TRUE);

    PANEL* err_panel = new_panel(err_window);
    win_show(err_window, "Press F1 to exit", COLOR_PAIR(4));
    print_in_middle(err_window, 3, 0, COLS / 2, err_message, COLOR_PAIR(4));
    wrefresh(err_window);
    update_panels();
    doupdate();

    while((ch = wgetch(err_window)) != KEY_F(1)) {
        continue;
    }

    del_panel(err_panel);
    destroy_win(err_window);
}
