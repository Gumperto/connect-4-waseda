#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "leaderboard.h"
#include "macros.h"

void update_leaderboard(char *winner) {

    int player_count = 0, isSameName = 0;
    char name_string[MAX_NAME_SIZE];
    
    typedef struct leaderboardTag {
        char name[MAX_NAME_SIZE];
        int wins;
    } lbTag;

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
            sscanf(line_store, " %127[^,], %d", lb[i].name, &lb[i].wins);      // make sure to have names only until max characters, and no comma used
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

        // sorting and printing leaderboard

        printf("\n===   TOP 5 LEADERBOARD   ===\n");
        int limit = (player_count < 5) ? player_count : 5;
        for (int i = 0; i < limit; i++) {
            printf("%d. %-20s %d wins\n", i + 1, lb[i].name, lb[i].wins);
        }
        printf("================================\n");

        int winner_rank = -1;
        for (int i = 0; i < player_count; i++) {
            if (strcmp(lb[i].name, winner) == 0) {
                winner_rank = i + 1;
                break;
            }
        }

        if (winner_rank > 5) {
            printf("\n%s is currently ranked #%d with %d wins!\n", 
                winner, winner_rank, lb[winner_rank - 1].wins);
        } else if (winner_rank > 0) {
            printf("\n%s is in the TOP 5! Currently ranked #%d\n", 
                winner, winner_rank);
        } else {
            printf("\n(Winner not found in leaderboard — unexpected!)\n");
        }
    }

    fclose(leaderboard_file);
    fclose(player_list);
    
}
