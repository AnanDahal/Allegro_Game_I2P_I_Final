#ifndef player_h
#define player_h

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "map.h"
#include "utility.h"


typedef enum {
    PLAYER_IDLE, PLAYER_WALKING, PLAYER_DYING
} PLAYER_STATUS;

typedef struct _Player{
    Point coord; // coordinate of the player
    int speed; 
    int direction;
    int health;

    


    ALLEGRO_SAMPLE* hurt_audio;

    ALLEGRO_BITMAP* image;
    uint8_t animation_tick; // For animation

    float knockback_angle;
    uint8_t knockback_CD;

    PLAYER_STATUS status;

} Player;


int player_up;
int player_down;
int player_left;
int player_right;

int cocudos_up;
int cocudos_down;
int cocudos_left;
int cocudos_right;

Player create_player(char * path,int row,int col, int sound);
void update_player(Player * player, Map * map);
void draw_player(Player * player, Point cam);
void delete_player(Player * player);
void hitPlayer(Player * player, Point enemy_coord, int damage);

void update_player2(Player* player, Map* map);
void draw_player2(Player* player, Point cam);

#endif /* player_h */
