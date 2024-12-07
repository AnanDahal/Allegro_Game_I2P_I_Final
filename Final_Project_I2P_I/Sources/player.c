#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"

#include <math.h>

static bool isCollision(Player* player, Map* map);

Player create_player(char* path, int row, int col) {
    Player player;
    memset(&player, 0, sizeof(player));

    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };

    player.speed = 4;
    player.health = 50;

    player.image = al_load_bitmap(path);
    if (!player.image) {
        game_abort("Error Load Bitmap with path : %s", path);
    }

    player.hurt_audio = al_load_sample("Assets/audio/hurt.mp3");
    if (!player.hurt_audio) {
        game_abort("Can't load hurt audio");
    }

    return player;
}

void update_player(Player* player, Map* map) {

    Point original = player->coord;

    // Knockback effect
    if (player->knockback_CD > 0) {

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){ next_x, player->coord.y };

        if (!isCollision(player, map)) {
            player->coord = next;
        }

        next = (Point){ player->coord.x, next_y };
        if (!isCollision(player, map)) {
            player->coord = next;
        }
    }

    /*
        [TODO HACKATHON 1-1]

        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */


    // Start 1 - 1

    if ((keyState[ALLEGRO_KEY_W] || keyState[ALLEGRO_KEY_S] || keyState[ALLEGRO_KEY_A] || keyState[ALLEGRO_KEY_D]) && player->health > 0) {
        player->status = PLAYER_WALKING;
    }
    else if (player->health > 0) {
        player->status = PLAYER_IDLE;
    }
    else {
        player->status = PLAYER_DYING;
    }
    if (player->status != PLAYER_DYING) {
        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y -= player->speed;
            player->direction = UP;
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y += player->speed;
            player->direction = DOWN;
        }
    }

    // End 1 - 1


    // if Collide, snap to the grid to make it pixel perfect
    if (isCollision(player, map)) {
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }

    /*
        [TODO HACKATHON 1-2]

        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */

    // Start 1 - 2
    if (player->status != PLAYER_DYING) {

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.x -= player->speed;
            player->direction = LEFT;
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.x += player->speed;
            player->direction = RIGHT;
        }
        // End 1 - 2
    }




    if (isCollision(player, map)) {
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }

    /*
        [TODO Homework]

        Calculate the animation tick to draw animation later
    */
    // Start HW 

    player->animation_tick = (player->animation_tick + 1) % 64;

    // End HW
}
int flag = 0; // Change the flag to flip character
void draw_player(Player* player, Point cam) {
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis

    

    /*
        [TODO Homework]

        Draw Animation of Dying, Walking, and Idle
    */

    // Start HW 

    int scene_x = 0;
    int scene_y = 0;
    int red_tint = player->knockback_CD > 0 ? 0 : 255;

    switch (player->status) {
    case (PLAYER_IDLE):
        if (keyState[ALLEGRO_KEY_D]) flag = 1;
        if (keyState[ALLEGRO_KEY_A]) flag = 0;
        scene_x = (int)(player->animation_tick / 32);
        break;
    case (PLAYER_WALKING):
        scene_y = 32;
        if (keyState[ALLEGRO_KEY_D]) flag = 1;
        if (keyState[ALLEGRO_KEY_A]) flag = 0;
        scene_x = (int)(player->animation_tick / 16);
        break;
    case (PLAYER_DYING):
        scene_y = 64;
        flag = 0;
        scene_x = (int)(player->animation_tick / 16);
        red_tint = 255;
        break;
    }

    int src_w = 32;
    int src_h = 32;
    int srcx = scene_x * 32;
    //int srcy = scene_y * 32;



    al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, red_tint, red_tint),
        srcx, scene_y, src_w, src_h, // source image x, y, width, height
        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        flag // Flip or not
    );

    // End HW


#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif

}

void delete_player(Player* player) {
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map) {

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    /*

        [TODO HACKATHON 2-1]

        Check every corner of enemy if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;

    */

    // Start 2 - 1

    if (!isWalkable(map->map[player->coord.y / TILE_SIZE][player->coord.x / TILE_SIZE])) return true;
    if (!isWalkable(map->map[(player->coord.y + TILE_SIZE - 1) / TILE_SIZE][player->coord.x / TILE_SIZE])) return true;
    if (!isWalkable(map->map[player->coord.y / TILE_SIZE][(player->coord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;
    if (!isWalkable(map->map[(player->coord.y + TILE_SIZE - 1) / TILE_SIZE][(player->coord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;

    // End 2 - 1

    return false;
}

void hitPlayer(Player* player, Point enemy_coord, int damage) {
    if (player->knockback_CD == 0) {
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

        /*
            [TODO Homework]

            Game Logic when the player get hit or die

            player->knockback_angle = angle;
            player->knockback_CD = 32;

            player->health = ...
            if(player->health <= 0){

                player->health = 0;
                player->status = ...

                ...
            }
        */

        // Start HW
        player->knockback_angle = angle;
        player->knockback_CD = 32;

        player->health -= damage;

        al_play_sample(player->hurt_audio, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);


        if (player->health <= 0) {
            player->health = 0;
            player->status = PLAYER_DYING;
            player->animation_tick = 0;
            // Trigger game-over logic if needed
        }

        // End HW

    }
}