#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <unistd.h>
#include "struct_headers.h"
#include "macros.h"
#include "windows.h"

coordinate *placeTiles(boardObject *game_board, int columnN, playerData* player) {
    coordinate *p_coordinate = malloc(sizeof(coordinate));
    p_coordinate->x = columnN;
    if(player->player_id == PLAYER_1) {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == EMPTY) {
                game_board->board[i][columnN] = PLAYER_1_SYMBOL;
                p_coordinate->y = i;
                break;
            }
        }
    } else {
        p_coordinate->y = -1;
        for(int i = game_board->rows - 1; i >= 0; i--) {
            if(game_board->board[i][columnN] == EMPTY) {
                game_board->board[i][columnN] = OPPONENT_SYMBOL;
                p_coordinate->y = i;
                break;
            }
        }
    }
    return p_coordinate;
}

coordinate* scanTiles(boardObject *game_board, playerData* player, 
                      WINDOW* window, int begin_y, int begin_x) {
    int switch_char;
    int player_index;
    coordinate* recent_coords = NULL;
    keypad(window, TRUE);

    if (player->player_id == PLAYER_1 || player->player_id == PLAYER_2) {
        const char* menu_text[MAX_COLS + 1][2] = {
            "1  ","",
            "2  ","",
            "3  ","",
            "4  ","",
            "5  ","",
            "6  ","",
            "7  ","",
            (char*) NULL,   
        };

        ITEM **menu_items = (ITEM **)calloc(MAX_COLS + 1, sizeof(ITEM *));
        for (int i = 0; i < MAX_COLS; i++) 
            menu_items[i] = new_item(menu_text[i][0], menu_text[i][1]);

        MENU *menu = new_menu(menu_items);

        int display_height = 1;
        int display_width = game_board->cols * (PADDING_SIZE + 1) - PADDING_SIZE;

        WINDOW* menu_window = derwin(window, display_height, display_width, 
                                     begin_y, begin_x);

        set_menu_win(menu, window);
        set_menu_sub(menu, menu_window);
        set_menu_format(menu, 1, game_board->cols);

        set_menu_mark(menu, "");
        set_menu_pad(menu, 3);

        post_menu(menu);
        wrefresh(window);

        while(1)
        {   
            switch_char = wgetch(window);
            switch(switch_char)
            {	case KEY_RIGHT:
                    menu_driver(menu, REQ_RIGHT_ITEM);
                    break;
                case KEY_LEFT:
                    menu_driver(menu, REQ_LEFT_ITEM);
                    break;
                case 10: // Which is <ENTER>
                {
                    ITEM* current;

                    current = current_item(menu);
                    player_index = item_index(current);
                    recent_coords = placeTiles(game_board, player_index, player);
                    if (recent_coords->y == -1) {
                        error_window("Invalid column! That's already filled!");
                        continue;
                    }
                    goto cleanup;
                }
                break;
            }
            wrefresh(menu_window);
        }

        cleanup:
            unpost_menu(menu);
            for(int i = 0; i < MAX_COLS; ++i)
                free_item(menu_items[i]);
            free_menu(menu);
            destroy_win(menu_window);
            wclear(window);
            wrefresh(window);
            keypad(window, FALSE);
            return recent_coords;
    }
    else if (player->player_id == BOT){
        player_index = simpleBot();
        recent_coords = placeTiles(game_board, player_index, player);
        while (recent_coords->y >= game_board->rows || recent_coords->y < 0) {
            player_index = simpleBot();
            recent_coords = placeTiles(game_board, player_index, player);
            sleep(1);
        }
        return recent_coords;
    }
    else if (player->player_id == BOSS) {
        player_index = finalBoss(game_board);
        recent_coords = placeTiles(game_board, player_index, player);
        while (recent_coords->y >= game_board->rows || recent_coords->y < 0 || recent_coords->x >= game_board->cols || recent_coords->x < 0) {
            player_index = finalBoss(game_board);
            recent_coords = placeTiles(game_board, player_index, player);
            sleep(1);
        }
        return recent_coords;
    }
}
