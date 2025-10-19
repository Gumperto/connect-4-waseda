#include <ncurses.h>
#include <panel.h>
#include <menu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "leaderboard.h"
#include "windows.h"
#include "macros.h"

typedef struct leaderboardTag {
    char name[MAX_NAME_SIZE];
    int wins;
} lbTag;

void update_leaderboard(char *winner) {

    int player_count = 0, isSameName = 0;
    char name_string[MAX_NAME_SIZE];

    FILE *leaderboard_file = fopen("./build/leaderboard.txt", "r");
    if (!leaderboard_file) {
        printf("Error: Could not open leaderboard.txt\n");
        return;
    }
    FILE *player_list = fopen("./build/player_list.txt", "r+");
    if (!player_list) {
        printf("Error: Could not open player_list.txt\n");
        fclose(leaderboard_file);
        return;
    }

    while (fgets(name_string, sizeof(name_string), player_list) != NULL) {
        name_string[strcspn(name_string, "\n")] = '\0';

        if (strcmp(winner, name_string) == 0) {
            isSameName = 1;
        }
        player_count++;
    }

    if (player_count >= 50 && isSameName == 0) {
        printf("Maximum leaderboard player count reached. Can not add new player names.\n");
    } else {
        
        lbTag *lb = malloc(sizeof(lbTag) * (player_count + 1));

        char line_store[MAX_NAME_SIZE + 5];
        for (int i = 0; i < player_count; i++) {
            fgets(line_store, sizeof(line_store), leaderboard_file);
            sscanf(line_store, " %127[^,], %d", lb[i].name, &lb[i].wins);
        }

        if (isSameName == 0) {
            fprintf(player_list, "%s\n", winner);
            player_count++;
            strcpy(lb[player_count - 1].name, winner);
            lb[player_count - 1].wins = 1;
        } else {
            for (int i = 0; i < player_count; i++) {
                if (strcmp(lb[i].name, winner) == 0) {
                    lb[i].wins ++;
                    break;
                }
            }
        }
        
        freopen("./build/leaderboard.txt", "w", leaderboard_file);
        if (!leaderboard_file) {
            printf("Error: Could not reopen leaderboard.txt for writing\n");
            free(lb);
            fclose(player_list);
            return;
        }
        for (int i = 0; i < player_count; i++) {
            fprintf(leaderboard_file, "%s, %d\n", lb[i].name, lb[i].wins);
        }

        free(lb);
    }
    fclose(leaderboard_file);
    fclose(player_list);
}

void print_leaderboard(WINDOW* window, char *winner, int winnerCheck) {
    int ch;
    box(window, 0, 0);
    wrefresh(window);
    int window_height, window_width;
    getmaxyx(window, window_height, window_width);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    print_ascii_art(window, 2, 0, window_width, "./assets/leaderboard.txt", COLOR_PAIR(3));

    int display_height = MAX_ROWS + 10;
    int display_width = (MAX_COLS * (PADDING_SIZE + 1)) - PADDING_SIZE + 20;
    WINDOW* leaderboard_win = derwin(window, display_height, display_width,
                              (window_height - display_height) / 2, 
                              (window_width - display_width) / 2);

    PANEL* leaderboard_panel = new_panel(leaderboard_win);

    win_show(leaderboard_win, "Leaderboard: Press q to exit", COLOR_PAIR(3));

    int player_count = 0;
    char name_string[MAX_NAME_SIZE];

    FILE *leaderboard_file = fopen("./build/leaderboard.txt", "r");
        if (!leaderboard_file) { 
            error_window("Error: Could not open leaderboard.txt"); 
            goto cleanup;
        }
    FILE *player_list = fopen("./build/player_list.txt", "r");
        if (!player_list) { 
            error_window("Error: Could not open player_list.txt"); 
            fclose(leaderboard_file); 
            goto cleanup;
        }

    while (fgets(name_string, sizeof(name_string), player_list)) 
        player_count++;

    if (player_count == 0) { 
        error_window("No players yet. Play a game or two!"); 
        fclose(leaderboard_file); 
        fclose(player_list); 
        goto cleanup;
    }

    lbTag *lb = malloc(sizeof(lbTag) * player_count);

    char line_store[MAX_NAME_SIZE+10];
    for (int i = 0 ; i < player_count; i++) {
        fgets(line_store,sizeof(line_store),leaderboard_file);
        sscanf(line_store," %127[^,], %d", lb[i].name, &lb[i].wins);
    }

    for (int i = 0; i < player_count - 1; i++) {
        for (int j = i + 1; j < player_count; j++) {
            if (lb[j].wins > lb[i].wins) { 
                lbTag temp=lb[i]; 
                lb[i]=lb[j]; 
                lb[j]=temp; 
            }
        }
    }

    char buffer[MAX_NAME_SIZE + 10];
    int begin_y = 4;
    print_in_middle(leaderboard_win, begin_y, 0, display_width,
                    "TOP 5 LEADERBOARD", COLOR_PAIR(3));
    int limit = (player_count < 5) ? player_count : 5;
    for (int i = 0; i < limit; i++) {
        begin_y++;
        snprintf(buffer, MAX_NAME_SIZE + 10, "%d. %-20s %d wins", i+1, lb[i].name, lb[i].wins);
        print_in_middle(leaderboard_win, begin_y, 0, display_width,
                        buffer, COLOR_PAIR(3));
    }
    begin_y++;
    print_in_middle(leaderboard_win, begin_y, 0, display_width,
                    "================================", COLOR_PAIR(3));

    if (winnerCheck == 1) {
        int winner_rank = -1;
        for (int i = 0; i < player_count; i++) {
            if (strcmp (lb[i].name, winner) == 0) { 
                winner_rank = i + 1; 
                break; 
            }
        }

        if(winner_rank > 5) {
            begin_y += 2;
            snprintf(buffer, MAX_NAME_SIZE + 10, 
                     "%s is currently ranked #%d with %d wins!", 
                     winner, winner_rank, lb[winner_rank-1].wins);
            print_in_middle(leaderboard_win, begin_y, 0, display_width,
                            buffer, COLOR_PAIR(3));
            begin_y += 2;
        } else if(winner_rank>0) {
            begin_y += 2;
            snprintf(buffer, MAX_NAME_SIZE + 10, 
                     "%s is in the TOP 5! Currently ranked #%d", 
                     winner, winner_rank, winner_rank);
            print_in_middle(leaderboard_win, begin_y, 0, display_width,
                            buffer, COLOR_PAIR(3));

            begin_y += 2;
        } else {
            begin_y += 2;
            print_in_middle(leaderboard_win, begin_y, 0, display_width,
                            "(Winner not found in leaderboard)", COLOR_PAIR(3));
            begin_y += 2;
        }
    }

    wrefresh(leaderboard_win);
    while((ch = wgetch(leaderboard_win)) != 'q') continue;
    free(lb);
    fclose(leaderboard_file);
    fclose(player_list);
    goto cleanup;

    cleanup:
        del_panel(leaderboard_panel);
        destroy_win(leaderboard_win);

        wclear(window);
        wrefresh(window);
}
