#include "game.h"
#include <allegro5/allegro.h>
#include "player.h"
int main(int argc, char **argv)
{
    player_up = ALLEGRO_KEY_W;
    player_down = ALLEGRO_KEY_S;
    player_left = ALLEGRO_KEY_A;
    player_right = ALLEGRO_KEY_D;

    cocudos_up = ALLEGRO_KEY_UP;
    cocudos_down = ALLEGRO_KEY_DOWN;
    cocudos_left = ALLEGRO_KEY_LEFT;
    cocudos_right = ALLEGRO_KEY_RIGHT;
    startGame();
   
    return 0;
    
}
