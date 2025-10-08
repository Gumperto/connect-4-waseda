#include "greeter.h"
#include "gamemodes.h"

int main() {
    char game_mode = greeter();
    if (game_mode == 'p' || game_mode == 'P')
        pvp_mode();
    else if (game_mode == 'b' || game_mode == 'B')
        pvbot_mode();
    else
        return 1;
}
