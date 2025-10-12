#include "greeter.h"
#include "macros.h"
#include <stdio.h>
#include <string.h>

#define MAX_NAME_SIZE 128

char greeter() {
    char game_mode;
    printf("===========================\n");
    printf("===Welcome to Connect 4!===\n");
    printf("===========================\n");
    
    printf("\n");
    printf("===SELECT GAME MODE===\n");
    printf("1. Play against another player\n");
    printf("2. Play against the CPU\n");
    printf("\n");

    printf("Enter game mode: [p]layer / [b]ot >>> ");
    scanf(" %c", &game_mode);
    getchar();

    while ((game_mode != 'p' && game_mode != 'P') && 
           (game_mode != 'b' && game_mode != 'B')) {
        printf("Game mode %c not found! Re-enter: [p]layer / [b]ot >>> ", game_mode);
        scanf(" %c", &game_mode);
        getchar();
    }
    printf("\n");

    printf("===GAME START===\n");
    printf("\n");
    return game_mode;
}

void fetch_names(char *player1, char *player2) {
    printf("===NAME SELECTION===\n");
    printf("\n");

    printf("What's Player 1's name? >>> ");
    fgets(player1, MAX_NAME_SIZE, stdin);
    player1[strcspn(player1, "\n")] = 0;
    printf("Hello, %s! Your tile is: %c\n", player1, PLAYER_1_SYMBOL);

    printf("\n");

    printf("What's Player 2's name? >>> ");
    fgets(player2, MAX_NAME_SIZE, stdin);
    player2[strcspn(player2, "\n")] = 0;
    printf("Hello, %s! Your tile is: %c\n", player2, OPPONENT_SYMBOL);    
}
