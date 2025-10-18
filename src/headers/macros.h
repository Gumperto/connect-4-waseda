#ifndef MACRO
#define MACRO

#define MAX_ROWS 6
#define MAX_COLS 7
#define PLAYER_1 1
#define PLAYER_2 2
#define BOT -1
#define BOSS -2
#define MAX_NAME_SIZE 128
#define PLAYER_1_SYMBOL 'X'
#define OPPONENT_SYMBOL 'O'
#define EMPTY '-'
#define PADDING_SIZE 3

enum {
    MAIN_MENU,
    PVP,
    PVBOT,
    PVBOSS,
    LEADERBOARD,
    QUIT,
    RESTART,
    ERROR = -1,
};

#endif
