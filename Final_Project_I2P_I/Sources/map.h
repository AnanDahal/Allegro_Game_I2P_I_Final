#ifndef map_h
#define map_h

#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"

// IF YOU WANT TO INCREASE YOUR MAP SIZE OR ENEMY SIZE MAKE SURE TO INCREASE DEFINE VARIABLE BELOW IF IT EXCEED THE MAXIMUM
#define MAX_MAP_ROW 100
#define MAX_MAP_COL 100
#define MAX_ENEMY_SPAWN 100
#define MAX_DOORS 3

/*
    Map Tiles type
    Hint : Some type might need a background, to configure it try to modify "get_map_offset(Map * map)"
*/

int coin_counter;

typedef enum _BLOCK_TYPE {
    FLOOR,
    WALL,
    DOOR_CLOSE,
    HOLE,
    COIN,
    NOTHING,
    TROPHY,
    BUTTON,
    DOOR_OPEN,
    HEALTH,
    S_COIN,
    SHOOTING_WALL
} BLOCK_TYPE;

typedef enum _COIN_STATUS {
    APPEAR,
    DISAPPEARING,
    DISAPPEAR
} COIN_STATUS;

typedef enum _TROPHY_STATUS {
    T_APPEAR,
    T_DISAPPEARING,
    T_DISAPPEAR
} TROPHY_STATUS;

typedef enum _HEALTH_STATUS {
    H_APPEAR,
    H_DISAPPEARING,
    H_DISAPPEAR
} HEALTH_STATUS;


typedef enum _DOOR_STATUS {
    CLOSED,
    CLOSING,
    OPENING,
    OPEN,
} DOOR_STATUS;

typedef enum _BUTTON_STATUS {
    POP_UP,
    POPPING,
    POP_DOWN,
    POPPING_UP
} BUTTON_STATUS;


// Map Struct
typedef struct Map_ {
    uint8_t** map; // Map array
    Point** offset_assets; // Assets

    int row, col;

    // Map assets
    ALLEGRO_BITMAP* assets;
    ALLEGRO_BITMAP* coin_assets;
    ALLEGRO_BITMAP* health_assets;
    ALLEGRO_BITMAP* trophy_assets;
    ALLEGRO_BITMAP* door_assets;
    ALLEGRO_BITMAP* button_assets;
    // Coin Properties
    ALLEGRO_SAMPLE* coin_audio;


    uint8_t animation;

    COIN_STATUS coin_status[MAX_MAP_ROW][MAX_MAP_COL];
    int coin_disappear_animation[MAX_MAP_ROW][MAX_MAP_COL];

    ALLEGRO_SAMPLE* trophy_audio;
    TROPHY_STATUS trophy_status[MAX_MAP_ROW][MAX_MAP_COL];

    ALLEGRO_SAMPLE* health_audio;
    HEALTH_STATUS health_status[MAX_MAP_ROW][MAX_MAP_COL];

    // Door Properties

    DOOR_STATUS door_status[MAX_MAP_ROW][MAX_MAP_COL];
    BUTTON_STATUS button_status[MAX_MAP_ROW][MAX_MAP_COL];


    // Spawn Coordinate
    Point SpawnP;
    Point SpawnJ;
    Point EnemySpawn[MAX_ENEMY_SPAWN];
    char EnemyCode[MAX_ENEMY_SPAWN];
    uint8_t EnemySpawnSize;

    bool win;
    bool far;
    bool toofar;

} Map;

bool door_up;
bool player_on_button;
bool cocudos_on_button;
PointFloat shootingwall_coord[MAX_MAP_ROW][MAX_MAP_COL];


/*
    MAP FUNCTION
    Feel free to add more if you have some idea or interaction with the map
 */
Map create_map(char* path, uint8_t type); // Create a map based on given file path
void draw_map(Map* map, Point cam); // Draw the map
void update_map(Map* map, Point player, Point cocudos, int* total_coins, int* map_coin); // Update map : you might want add some parameter here
void destroy_map(Map* map); // Destroy map


bool isWalkable(BLOCK_TYPE block);

#endif /* map_h */