#include "greeter.h"
#include "gamemodes.h"
#include <stdbool.h>

int main() {
    //while 1 = return value of pvp_mode() and pvbot_mode()
    //1 = restart, 0 = quit
    while(1){
        char game_mode = greeter();
        int play = 0;
        if (game_mode == 'p' || game_mode == 'P')
            play = pvp_mode();
        else if (game_mode == 'b' || game_mode == 'B')
            play = pvbot_mode();
        else
            return 1;
        if (play == 0) break;

    }
    return 0;

}
