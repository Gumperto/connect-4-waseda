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

    wrefresh(local_win);

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
   refresh();
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
