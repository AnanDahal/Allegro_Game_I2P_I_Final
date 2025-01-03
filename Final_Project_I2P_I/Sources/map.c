#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "game_scene.h"
#include "bullet.h"
#include "player.h"




static void get_map_offset(Map* map);
static Point get_wall_offset_assets(Map* map, int i, int j);
static Point get_floor_offset_assets(Map* map, int i, int j);
static Point get_hole_offset_assets(Map* map, int i, int j);
static const int offset = 16;


static int coin_animation = 0;
static int trophy_animation = 0;
static int button_animation = 0;
static int door_animation = 0;

Bullet bullet;
BulletNode* bulletList;
int timer = 80;
int timer_player = 60;
int timer_cocudos = 60;

static bool tile_collision(Point player, Point tile_coord);

Map create_map(char* path, uint8_t type) {
    Map map;

    button_animation = 0;
    door_animation = 0;

    memset(&map, 0, sizeof(Map));

    FILE* f = fopen(path, "r");
    if (!f) {
        game_abort("Can't load map file : %s", path);
    }

    game_log("Using %s to load the map", path);

    fscanf(f, "%d %d", &map.row, &map.col);
    char ch;

    // Map Array
    map.map = (uint8_t**)malloc(map.row * sizeof(uint8_t*));
    for (int i = 0; i < map.row; i++) {
        map.map[i] = (uint8_t*)malloc(map.col * sizeof(uint8_t));
    }

    // Map Offset Resource Pack Array
    map.offset_assets = (Point**)malloc(map.row * sizeof(Point*));
    for (int i = 0; i < map.row; i++) {
        map.offset_assets[i] = (Point*)malloc(map.col * sizeof(Point));
    }

    coin_counter = 0;
    map_coin = 0;

    // Scan the map to the array
    int door_counter = 0;
    for (int i = 0; i < map.row; i++) {
        for (int j = 0; j < map.col; j++) {
            fscanf(f, " %c", &ch);

            switch (ch)
            {
            case '#': // Case Wall
                map.map[i][j] = WALL;
                break;

            case 'W': // Shooting Wall
                map.map[i][j] = SHOOTING_WALL;
                shootingwall_coord[i][j] = (PointFloat){ i , j };
                break;

            case '.': // Case Floor
                map.map[i][j] = FLOOR;
                break;

            case 'P': // Spawn Point
                map.map[i][j] = FLOOR;
                map.SpawnP = (Point){ i, j };
                break;

            case 'J': // Cocudos Spawn
                map.map[i][j] = FLOOR;
                map.SpawnJ = (Point){ i, j };
                break;

            case 'S': // Slime Enemy
                map.map[i][j] = FLOOR;
                map.EnemyCode[map.EnemySpawnSize] = ch;
                map.EnemySpawn[map.EnemySpawnSize++] = (Point){ i, j };
                break;

            case 'M': // Magma Enemy
                map.map[i][j] = FLOOR;
                map.EnemyCode[map.EnemySpawnSize] = ch;
                map.EnemySpawn[map.EnemySpawnSize++] = (Point){ i, j };
                break;

            case 'L': // Boss Skeleton Enemy
                map.map[i][j] = FLOOR;
                map.EnemyCode[map.EnemySpawnSize] = ch;
                map.EnemySpawn[map.EnemySpawnSize++] = (Point){ i, j };
                break;

            case 'C': // Coins
                map.map[i][j] = COIN;
                map.coin_status[i][j] = APPEAR;
                coin_counter++;
                break;
            case 'K': // Spikes
                map.map[i][j] = SPIKE;
                break;

            case 'H': // Health
                map.map[i][j] = HEALTH;
                map.health_status[i][j] = H_APPEAR;
                break;
            case 'T': // Trophy
                map.map[i][j] = TROPHY;
                if(map_level != 3) map.trophy_status[i][j] = T_DISAPPEAR;
                else map.trophy_status[i][j] = T_APPEAR;
                break;
            case 'D': // Door
                map.map[i][j] = DOOR_CLOSE;
                map.door_status[i][j] = CLOSED;
                door_up = false;
                break;
            case '2': // Door
                map.map[i][j] = D2C;
                d2_status[i][j] = CLOSED;
                door_up2 = false;
                break;
            case '3': // Door
                map.map[i][j] = D3C;
                d3_status[i][j] = CLOSED;
                door_up3 = false;
                break;
            case '4': // Door
                map.map[i][j] = D4C;
                d4_status[i][j] = CLOSED;
                door_up4 = false;
                break;
            case '5': // Door
                map.map[i][j] = D5C;
                d5_status[i][j] = CLOSED;
                door_up5 = false;
                break;
            case '6': // Door
                map.map[i][j] = D6C;
                d6_status[i][j] = CLOSED;
                door_up6 = false;
                break;

            case 'B': // Button
                map.map[i][j] = BUTTON;
                map.button_status[i][j] = POP_UP;
                break;
            case 'Q': // Button
                map.map[i][j] = B2;
                b2_status[i][j] = POP_UP;
                break;
            case 'E': // Button
                map.map[i][j] = B3;
                b3_status[i][j] = POP_UP;
                break;
            case 'R': // Button
                map.map[i][j] = B4;
                b4_status[i][j] = POP_UP;
                break;
            case 'Y': // Button
                map.map[i][j] = B5;
                b5_status[i][j] = POP_UP;
                break;
            case 'U': // Button
                map.map[i][j] = B6;
                b6_status[i][j] = POP_UP;
                break;

            case '_': // Nothing
                map.map[i][j] = HOLE;
                break;

            default: // If not determined, set it as black
                map.map[i][j] = NOTHING;
                break;
            }
        }
    }

    if (map_level == 3) {
        map.assets = al_load_bitmap("Assets/map_packets_3.png");
        if (!map.assets) {
            game_abort("Can't load map assets");
        }
    }
    else {
        map.assets = al_load_bitmap("Assets/map_packets.png");
        if (!map.assets) {
            game_abort("Can't load map assets");
        }
    }


    map.coin_assets = al_load_bitmap("Assets/coins.png");
    if (!map.coin_assets) {
        game_abort("Can't load coin assets");
    }




    // load the offset for each tiles type
    get_map_offset(&map);

    map.coin_audio = al_load_sample("Assets/audio/coins.mp3");
    if (!map.coin_audio) {
        game_abort("Can't load coin audio");
    }

    map.button_audio = al_load_sample("Assets/audio/button.mp3");
    if (!map.button_audio) {
        game_abort("Can't load button audio");
    }

    map.door_assets = al_load_bitmap("Assets/Door.png");
    if (!map.door_assets) {
        game_abort("Can't load door assets");
    }


    map.button_assets = al_load_bitmap("Assets/Button.png");
    if (!map.button_assets) {
        game_abort("Can't load button assets");
    }

    map.spikes_assets = al_load_bitmap("Assets/spikes.png");
    if (!map.spikes_assets) {
        game_abort("Can't load spikes assets");
    }


    map.trophy_assets = al_load_bitmap("Assets/trophy.png");
    if (!map.trophy_assets) {
        game_abort("Can't load trophy assets");
    }

    map.health_assets = al_load_bitmap("Assets/health_box.png");
    if (!map.health_assets) {
        game_abort("Can't load health assets");
    }

    map.trophy_audio = al_load_sample("Assets/audio/win.mp3");
    if (!map.trophy_audio) {
        game_abort("Can't load trophy audio");
    }

    map.health_audio = al_load_sample("Assets/audio/health.mp3");
    if (!map.health_audio) {
        game_abort("Can't load health audio");
    }

    map.win = false;
    map.far = false;
    map.toofar = false;

    fclose(f);

    return map;
}

void draw_map(Map* map, Point cam) {
    // Draw map based on the camera point coordinate

    if (map_level == 3) {
        al_clear_to_color(al_map_rgb(75, 5, 0));
    }
    else {
        al_clear_to_color(al_map_rgb(24, 20, 37));
    }

    for (int i = 0; i < map->row; i++) {
        for (int j = 0; j < map->col; j++) {

            int dy = i * TILE_SIZE - cam.y; // destiny y axis
            int dx = j * TILE_SIZE - cam.x; // destiny x axis

            Point offset_asset = map->offset_assets[i][j];


            al_draw_scaled_bitmap(map->assets, // image
                offset_asset.x, offset_asset.y, 16, 16, // source x, source y, width, height
                dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, destiny y, destiny width, destiny height
                0); // flag : set 0


            int src_coin_width = 16;
            int src_coin_height = 16;

            int src_door_height = 16;
            int src_door_width = 32;

            switch (map->map[i][j]) {
            case SHOOTING_WALL: {
                
                al_draw_scaled_bitmap(map->assets,
                    32, 48, 16, 16,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);

                if (timer == 0) {

                    PointFloat center = (PointFloat){
                        (shootingwall_coord[i][j].y * TILE_SIZE) + TILE_SIZE / 2,
                        (shootingwall_coord[i][j].x * TILE_SIZE) + TILE_SIZE / 2
                    };

                    Bullet bullet = create_bullet("Assets/orange_bullet.png", center, 1.571, 7, 10, 0, 1); // 1.571 radians = ~90� (downwards)
                    insertBulletList(bulletList, bullet);
                    timer = 60;
                }
                else {
                    timer--;
                }


                break;
            }
            case SPIKE: {

                al_draw_scaled_bitmap(map->spikes_assets,
                    0, 0, 16, 16,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);

                break;
            }
            case COIN: {
                int offsetx = 0;
                int offsety = 0;
                if (map->coin_status[i][j] == APPEAR) {
                    offsetx = src_coin_width * (int)(coin_animation / 8);
                }
                else if (map->coin_status[i][j] == DISAPPEARING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 8);
                    offsety = 1 * src_coin_height;
                    map->coin_disappear_animation[i][j] += 1;
                    if (map->coin_disappear_animation[i][j] == 64) {
                        map->coin_status[i][j] = DISAPPEAR;
                    }
                }
                else if (map->coin_status[i][j] == DISAPPEAR) {
                    map->map[i][j] = FLOOR;
                    break;
                }
                al_draw_scaled_bitmap(map->coin_assets,
                    offsetx, offsety, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case S_COIN: {
                int offsetx = 0;
                int offsety = 0;
                if (map->coin_status[i][j] == APPEAR) {
                    offsetx = src_coin_width * (int)(coin_animation / 8);
                }
                else if (map->coin_status[i][j] == DISAPPEARING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 8);
                    offsety = 1 * src_coin_height;
                    map->coin_disappear_animation[i][j] += 1;
                    if (map->coin_disappear_animation[i][j] == 64) {
                        map->coin_status[i][j] = DISAPPEAR;
                    }
                }
                else if (map->coin_status[i][j] == DISAPPEAR) {
                    map->map[i][j] = FLOOR;
                    break;
                }
                al_draw_tinted_scaled_bitmap(map->coin_assets, al_map_rgb(160, 32, 240),
                    offsetx, offsety, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE + 5, TILE_SIZE + 5,
                    0);
                break;
            }
            case TROPHY: {
                if ((map_coin == coin_counter && map->trophy_status[i][j] != T_DISAPPEARING) || map_level == 3 && (map->trophy_status[i][j] != T_DISAPPEARING)) {
                    int offsetx = 32 * (int)(trophy_animation / (64 / 9));
                    if (offsetx > 32 * 9) {
                        offsetx = 0;
                    }
                    int offsety = 0;

                    al_draw_scaled_bitmap(map->trophy_assets,
                        offsetx, offsety, 32, 32,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                    break;
                }

                if (map->trophy_status[i][j] == T_DISAPPEARING) {
                    int offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 8);
                    int offsety = 1 * src_coin_height;
                    map->coin_disappear_animation[i][j] += 1;
                    if (map->coin_disappear_animation[i][j] == 64) {
                        map->win = true;
                    }
                    al_draw_scaled_bitmap(map->coin_assets,
                        offsetx, offsety, 16, 16,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                    break;
                }

                break;
            }
            case HEALTH: {
                if (map->health_status[i][j] == H_APPEAR) {

                    al_draw_scaled_bitmap(map->health_assets,
                        0, 0, 32, 32,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                    break;
                }

                if (map->health_status[i][j] == H_DISAPPEARING) {
                    int offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 8);
                    int offsety = 1 * src_coin_height;
                    map->coin_disappear_animation[i][j] += 1;
                    if (map->coin_disappear_animation[i][j] == 64) {
                        map->trophy_status[i][j] = H_DISAPPEAR;
                    }
                    al_draw_scaled_bitmap(map->coin_assets,
                        offsetx, offsety, 16, 16,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                    break;
                }

                break;
            }
            case DOOR_CLOSE: {
                int offsetx = 0;
                int offsety = 0;

                if (map->door_status[i][j] == CLOSED) {
                    if (door_up) { // Button is pressed
                        map->door_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (map->door_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        map->door_status[i][j] = OPEN;
                        map->map[i][j] = DOOR_OPEN;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case DOOR_OPEN: {
                int offsetx = 0;
                int offsety = 16;

                if (map->door_status[i][j] == OPEN) {
                    if (!door_up) { // Button released
                        map->door_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (map->door_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        map->door_status[i][j] = CLOSED;
                        map->map[i][j] = DOOR_CLOSE;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }

            case D2C: {
                int offsetx = 0;
                int offsety = 0;

                if (d2_status[i][j] == CLOSED) {
                    if (door_up2) { // Button is pressed
                        d2_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (d2_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d2_status[i][j] = OPEN;
                        map->map[i][j] = D2O;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D2O: {
                int offsetx = 0;
                int offsety = 16;

                if (d2_status[i][j] == OPEN) {
                    if (!door_up2) { // Button released
                        d2_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (d2_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d2_status[i][j] = CLOSED;
                        map->map[i][j] = D2C;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D3C: {
                int offsetx = 0;
                int offsety = 0;

                if (d3_status[i][j] == CLOSED) {
                    if (door_up3) { // Button is pressed
                        d3_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (d3_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d3_status[i][j] = OPEN;
                        map->map[i][j] = D3O;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D3O: {
                int offsetx = 0;
                int offsety = 16;

                if (d3_status[i][j] == OPEN) {
                    if (!door_up3) { // Button released
                        d3_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (d3_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d3_status[i][j] = CLOSED;
                        map->map[i][j] = D3C;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D4C: {
                int offsetx = 0;
                int offsety = 0;

                if (d4_status[i][j] == CLOSED) {
                    if (door_up4) { // Button is pressed
                        d4_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (d4_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d4_status[i][j] = OPEN;
                        map->map[i][j] = D4O;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D4O: {
                int offsetx = 0;
                int offsety = 16;

                if (d4_status[i][j] == OPEN) {
                    if (!door_up4) { // Button released
                        d4_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (d4_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d4_status[i][j] = CLOSED;
                        map->map[i][j] = D4C;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D5C: {
                int offsetx = 0;
                int offsety = 0;

                if (d5_status[i][j] == CLOSED) {
                    if (door_up5) { // Button is pressed
                        d5_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (d5_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d5_status[i][j] = OPEN;
                        map->map[i][j] = D5O;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D5O: {
                int offsetx = 0;
                int offsety = 16;

                if (d5_status[i][j] == OPEN) {
                    if (!door_up5) { // Button released
                        d5_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (d5_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d5_status[i][j] = CLOSED;
                        map->map[i][j] = D5C;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D6C: {
                int offsetx = 0;
                int offsety = 0;

                if (d6_status[i][j] == CLOSED) {
                    if (door_up6) { // Button is pressed
                        d6_status[i][j] = OPENING;
                        map->coin_disappear_animation[i][j] = 0; // Reset animation
                    }
                    offsetx = 0;
                }
                else if (d6_status[i][j] == OPENING) {
                    offsety = 0;
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d6_status[i][j] = OPEN;
                        map->map[i][j] = D6O;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case D6O: {
                int offsetx = 0;
                int offsety = 16;

                if (d6_status[i][j] == OPEN) {
                    if (!door_up6) { // Button released
                        d6_status[i][j] = CLOSING;
                        map->coin_disappear_animation[i][j] = 0; // Start closing 
                    }
                }
                else if (d6_status[i][j] == CLOSING) {
                    offsetx = src_door_width * (int)(map->coin_disappear_animation[i][j] / 32);
                    map->coin_disappear_animation[i][j] += 5;

                    if (map->coin_disappear_animation[i][j] >= 200) { // Animation completed
                        d6_status[i][j] = CLOSED;
                        map->map[i][j] = D6C;
                    }
                }

                al_draw_scaled_bitmap(map->door_assets,
                    offsetx, offsety, src_door_width, src_door_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }


            case BUTTON: {
                int offsetx = 0;

                if (map->button_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (map->button_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        map->button_status[i][j] = POP_DOWN;
                        door_up = true; // Button pressed
                    }
                }
                else if (map->button_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player_on_button && !cocudos_on_button) { // No player or cocudos on the button
                        map->button_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up = false; // Button released
                    }
                }
                else if (map->button_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        map->button_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case B2: {
                int offsetx = 0;

                if (b2_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (b2_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b2_status[i][j] = POP_DOWN;
                        door_up2 = true; // Button pressed
                    }
                }
                else if (b2_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player2_on_button && !cocudos2_on_button) { // No player or cocudos on the button
                        b2_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up2 = false; // Button released
                    }
                }
                else if (b2_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b2_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case B3: {
                int offsetx = 0;

                if (b3_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (b3_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b3_status[i][j] = POP_DOWN;
                        door_up3 = true; // Button pressed
                    }
                }
                else if (b3_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player3_on_button && !cocudos3_on_button) { // No player or cocudos on the button
                        b3_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up3 = false; // Button released
                    }
                }
                else if (b3_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b3_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case B4: {
                int offsetx = 0;

                if (b4_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (b4_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b4_status[i][j] = POP_DOWN;
                        door_up4 = true; // Button pressed
                    }
                }
                else if (b4_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player4_on_button && !cocudos4_on_button) { // No player or cocudos on the button
                        b4_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up4 = false; // Button released
                    }
                }
                else if (b4_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b4_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case B5: {
                int offsetx = 0;

                if (b5_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (b5_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b5_status[i][j] = POP_DOWN;
                        door_up5 = true; // Button pressed
                    }
                }
                else if (b5_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player5_on_button && !cocudos5_on_button) { // No player or cocudos on the button
                        b5_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up5 = false; // Button released
                    }
                }
                else if (b5_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b5_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            case B6: {
                int offsetx = 0;

                if (b6_status[i][j] == POP_UP) {
                    offsetx = 0;
                }
                else if (b6_status[i][j] == POPPING) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b6_status[i][j] = POP_DOWN;
                        door_up6 = true; // Button pressed
                    }
                }
                else if (b6_status[i][j] == POP_DOWN) {
                    offsetx = 32;

                    if (!player6_on_button && !cocudos6_on_button) { // No player or cocudos on the button
                        b6_status[i][j] = POPPING_UP;
                        map->coin_disappear_animation[i][j] = 16;
                        door_up6 = false; // Button released
                    }
                }
                else if (b6_status[i][j] == POPPING_UP) {
                    offsetx = src_coin_width * (int)(map->coin_disappear_animation[i][j] / 16);
                    map->coin_disappear_animation[i][j] += 1;

                    if (map->coin_disappear_animation[i][j] >= 32) { // Animation completed
                        b6_status[i][j] = POP_UP;
                    }
                }

                al_draw_scaled_bitmap(map->button_assets,
                    offsetx, 0, src_coin_width, src_coin_height,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }


            default:
                break;
            }
#ifdef DRAW_HITBOX
            al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(0, 255, 0), 1
            );
#endif
        }
    }
}



void update_map(Map* map, Point player, Point cocudos, int* total_coins, int* map_coins) {
    

    coin_animation = (coin_animation + 1) % 64;
    trophy_animation = (trophy_animation + 1) % (64 * 2);

    int center_x = (int)((player.x + (int)(TILE_SIZE / 2)) / TILE_SIZE);
    int center_y = (int)((player.y + (int)(TILE_SIZE / 2)) / TILE_SIZE);

    int center_x_cocudos = (int)((cocudos.x + (int)(TILE_SIZE / 2)) / TILE_SIZE);
    int center_y_cocudos = (int)((cocudos.y + (int)(TILE_SIZE / 2)) / TILE_SIZE);

    if (map->map[center_y_cocudos][center_x_cocudos] == COIN &&
        map->coin_status[center_y_cocudos][center_x_cocudos] == APPEAR) {
        *total_coins += 1;
        *map_coins += 1;
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 0;
        map->coin_status[center_y_cocudos][center_x_cocudos] = DISAPPEARING;
        al_play_sample(map->coin_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    if (map->map[center_y_cocudos][center_x_cocudos] == S_COIN &&
        map->coin_status[center_y_cocudos][center_x_cocudos] == APPEAR) {
        *total_coins += 3;
        *map_coins += 3;
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 0;
        map->coin_status[center_y_cocudos][center_x_cocudos] = DISAPPEARING;
        al_play_sample(map->coin_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    
    if (map->map[center_y_cocudos][center_x_cocudos] == TROPHY &&
        ((map_coin == coin_counter) || (map_level == 3))) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        map->trophy_status[center_y_cocudos][center_x_cocudos] = T_DISAPPEARING;
        al_play_sample(map->trophy_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    if (map->map[center_y_cocudos][center_x_cocudos] == HEALTH &&
        map->health_status[center_y_cocudos][center_x_cocudos] == APPEAR) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        map->health_status[center_y_cocudos][center_x_cocudos] = DISAPPEARING;
        add_health = true;
        al_play_sample(map->health_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    

    if (map->map[center_y_cocudos][center_x_cocudos] == SPIKE) {
        reduce_health_cocudos = true;
    }
    else if(map->map[center_y_cocudos][center_x_cocudos] != SPIKE && reduce_health_cocudos){
        reduce_health_cocudos = false;
        reduce_health_cocudos_stepped_off = true;
    }

    if (map->map[center_y][center_x] == SPIKE) {
        reduce_health_player = true;
    }
    else if (map->map[center_y][center_x] != SPIKE && reduce_health_player){
        reduce_health_player = false;
        reduce_health_player_stepped_off = true;
    }


    // Check if player is on the button
    if (map->map[center_y][center_x] == BUTTON &&
        map->button_status[center_y][center_x] == POP_UP && !player_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        map->button_status[center_y][center_x] = POPPING;
        player_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == BUTTON) {
        player_on_button = true;
    }
    else {
        player_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == BUTTON &&
        map->button_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        map->button_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == BUTTON) {
        cocudos_on_button = true;
    }
    else {
        cocudos_on_button = false;
    }

    if (map->map[center_y][center_x] == B2 &&
        b2_status[center_y][center_x] == POP_UP && !player2_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        b2_status[center_y][center_x] = POPPING;
        player2_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == B2) {
        player2_on_button = true;
    }
    else {
        player2_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == B2 &&
        b2_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos2_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        b2_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos2_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == B2) {
        cocudos2_on_button = true;
    }
    else {
        cocudos2_on_button = false;
    }

    if (map->map[center_y][center_x] == B3 &&
        b3_status[center_y][center_x] == POP_UP && !player3_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        b3_status[center_y][center_x] = POPPING;
        player3_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == B3) {
        player3_on_button = true;
    }
    else {
        player3_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == B3 &&
        b3_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos3_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        b3_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos3_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == B3) {
        cocudos3_on_button = true;
    }
    else {
        cocudos3_on_button = false;
    }

    if (map->map[center_y][center_x] == B4 &&
        b4_status[center_y][center_x] == POP_UP && !player4_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        b4_status[center_y][center_x] = POPPING;
        player4_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == B4) {
        player4_on_button = true;
    }
    else {
        player4_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == B4 &&
        b4_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos4_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        b4_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos4_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == B4) {
        cocudos4_on_button = true;
    }
    else {
        cocudos4_on_button = false;
    }

    if (map->map[center_y][center_x] == B5 &&
        b5_status[center_y][center_x] == POP_UP && !player5_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        b5_status[center_y][center_x] = POPPING;
        player5_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == B5) {
        player5_on_button = true;
    }
    else {
        player5_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == B5 &&
        b5_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos5_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        b5_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos5_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == B5) {
        cocudos5_on_button = true;
    }
    else {
        cocudos5_on_button = false;
    }

    if (map->map[center_y][center_x] == B6 &&
        b6_status[center_y][center_x] == POP_UP && !player6_on_button) {
        map->coin_disappear_animation[center_y][center_x] = 16;
        b6_status[center_y][center_x] = POPPING;
        player6_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y][center_x] == B6) {
        player6_on_button = true;
    }
    else {
        player6_on_button = false;
    }


    // Check if cocudos is on the button
    if (map->map[center_y_cocudos][center_x_cocudos] == B6 &&
        b6_status[center_y_cocudos][center_x_cocudos] == POP_UP && !cocudos6_on_button) {
        map->coin_disappear_animation[center_y_cocudos][center_x_cocudos] = 16;
        b6_status[center_y_cocudos][center_x_cocudos] = POPPING;
        cocudos6_on_button = true;
        al_play_sample(map->button_audio, SFX_VOLUME, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
    else if (map->map[center_y_cocudos][center_x_cocudos] == B6) {
        cocudos6_on_button = true;
    }
    else {
        cocudos6_on_button = false;
    }
}

void destroy_map(Map* map) {
    for (int i = 0; i < map->row; i++) {
        free(map->map[i]);
        free(map->offset_assets[i]);
    }
    free(map->map);
    free(map->offset_assets);


    al_destroy_bitmap(map->assets);
    al_destroy_bitmap(map->spikes_assets);
    al_destroy_bitmap(map->coin_assets);
    al_destroy_sample(map->coin_audio);
    al_destroy_bitmap(map->trophy_assets);
    al_destroy_sample(map->trophy_audio);
    al_destroy_bitmap(map->health_assets);
    al_destroy_sample(map->health_audio);
    al_destroy_bitmap(map->door_assets);
    al_destroy_bitmap(map->button_assets);
    al_destroy_sample(map->button_audio);
}

bool isWalkable(BLOCK_TYPE block) {
    if (block == FLOOR || block == COIN || block == TROPHY || block == HEALTH || block == S_COIN
        || block == BUTTON || block == DOOR_OPEN || block == B2 || block == B3 || block == B4 || block == B5
        || block == B6 || block == D2O || block == D3O || block == D4O || block == D5O || block == D6O || block == SPIKE) return true;
    return false;
}


static bool isWall(Map* map, int i, int j);
//static bool isFloorHole(Map * map, int i, int j);
static bool isFloor(Map* map, int i, int j);
static bool isNothing(Map* map, int i, int j);

static Point get_floor_offset_assets(Map* map, int i, int j) {
    bool up = isWall(map, i - 1, j);
    bool left = isWall(map, i, j - 1);
    bool right = isWall(map, i, j + 1);
    bool top_left = isWall(map, i - 1, j - 1);
    bool top_right = isWall(map, i - 1, j + 1);

    if (up && left && right) {
        return (Point) { offset * 12, offset * 3 };
    }
    if (up && left) {
        return (Point) { offset * 11, 0 };
    }
    if (up && right) {
        return (Point) { offset * 13, 0 };
    }
    if (left) {
        if (top_left)
            return (Point) { offset * 11, offset * 1 };
        else
            return (Point) { offset * 15, offset * 1 };

    }
    if (right) {
        if (top_right)
            return (Point) { offset * 13, offset * 1 };
        else
            return (Point) { offset * 14, offset * 1 };
    }
    if (up) {
        return (Point) { offset * 12, 0 };
    }
    if (top_left) {
        return (Point) { offset * 11, offset * 3 };
    }
    if (top_right) {
        return (Point) { offset * 13, offset * 3 };
    }

    return (Point) { 12 * offset, 4 * offset };
}

// Calculate the offset from the source assets
static Point get_wall_offset_assets(Map* map, int i, int j) {
    bool up = isWall(map, i - 1, j);
    bool down = isWall(map, i + 1, j);
    bool left = isWall(map, i, j - 1);
    bool right = isWall(map, i, j + 1);

    if (up && down && left && right) {
        return (Point) { 3 * offset, 5 * offset };
    }
    //if (up && down && !left && !right && isFloor(map, i, j - 1) && isFloor(map, i, j - 2)) {
    //    return (Point) { 6 * offset, 5 * offset }; // For horizontal span
    //}
    //if (!up && !down && left && right && isFloor(map, i - 1, j) && isFloor(map, i - 2, j)) {
    //    return (Point) { 3 * offset, 6 * offset }; // For vertical span
    //}

    if (up && down && right) {
        bool left_floor = isFloor(map, i, j - 1);
        bool right_down = isWall(map, i + 1, j + 1);
        bool right_up = isWall(map, i - 1, j + 1);
        if (right_down && right_up)
            return (Point) { 2 * offset, 5 * offset };
        if (left_floor) {
            return (Point) { 1 * offset, 3 * offset };
        }
        else {
            return (Point) { 4 * offset, 5 * offset };
        }
    }
    if (up && down && left) {
        bool right_floor = isFloor(map, i, j + 1);
        bool left_down = isWall(map, i + 1, j - 1);
        bool left_up = isWall(map, i - 1, j - 1);
        if (left_down && left_up)
            return (Point) { 4 * offset, 5 * offset };
        if (right_floor) {
            return (Point) { 1 * offset, 3 * offset };
        }
        else {
            return (Point) { 2 * offset, 5 * offset };
        }
    }
    if (down && right && left) {
        bool down_right_wall = isWall(map, i + 1, j + 1);
        bool down_left_wall = isWall(map, i + 1, j - 1);
        bool down_right = isFloor(map, i + 1, j + 1);
        bool down_left = isFloor(map, i + 1, j - 1);
        if (down_right_wall && down_left_wall)
            return (Point) { 3 * offset, 4 * offset };
        if ((down_right ^ down_left) == 0)
            return (Point) { 8 * offset, 5 * offset };
        if (down_right)
            return (Point) { 4 * offset, 4 * offset };
        if (down_left)
            return (Point) { 2 * offset, 4 * offset };
    }
    if (left && right) {
        if (isFloor(map, i + 1, j))
            return (Point) { 7 * offset, 5 * offset };
        else
            return (Point) { 7 * offset, 7 * offset };
    }
    if (down && up) {
        bool left_ = isFloor(map, i, j - 1);
        bool right_ = isFloor(map, i, j + 1);
        if ((left_ ^ right_) == 0)
            return (Point) { 1 * offset, 3 * offset };
        if (left_)
            return (Point) { 2 * offset, 5 * offset };
        return (Point) { 4 * offset, 5 * offset };
    }
    if (down && right) {
        if (isFloor(map, i + 1, j + 1))
            return (Point) { 4 * offset, 5 * offset };
        else
            return (Point) { 2 * offset, 4 * offset };
    }
    if (down && left) {
        if (isFloor(map, i, j + 1) && isFloor(map, i + 1, j - 1))
            return (Point) { 1 * offset, 2 * offset };
        else if (isFloor(map, i + 1, j - 1))
            return (Point) { 2 * offset, 5 * offset };
        else
            return (Point) { 4 * offset, 4 * offset };
    }
    if (up && right) {
        if (isFloor(map, i + 1, j))
            return (Point) { 2 * offset, 6 * offset };
        else
            return (Point) { 5 * offset, 6 * offset };
    }
    if (up && left) {
        if (isFloor(map, i + 1, j))
            return (Point) { 4 * offset, 6 * offset };
        else
            return (Point) { 6 * offset, 6 * offset };
    }
    if (left || right) {
        if (isFloor(map, i + 1, j))
            return (Point) { 7 * offset, 5 * offset };
        else
            return (Point) { 7 * offset, 7 * offset };
    }
    if (down || up) {
        bool left_ = isFloor(map, i, j - 1);
        bool right_ = isFloor(map, i, j + 1);
        if ((left_ ^ right_) == 0)
            return (Point) { 1 * offset, 3 * offset };
        if (left_)
            return (Point) { 2 * offset, 5 * offset };
        return (Point) { 4 * offset, 5 * offset };
    }
    return (Point) { 0, 0 };
}

static Point get_hole_offset_assets(Map* map, int i, int j) {
    bool up = isNothing(map, i - 1, j);
    bool U_Floor = isFloor(map, i - 1, j);

    if (up) {
        return (Point) { 0, 0 };
    }
    if (U_Floor) {
        bool UL = isFloor(map, i - 1, j - 1);
        bool UR = isFloor(map, i - 1, j + 1);
        if (UL && UR) return (Point) { offset * 13, offset * 11 };
        if (UL) return (Point) { offset * 14, offset * 11 };
        if (UR) return (Point) { offset * 12, offset * 11 };
        return (Point) { offset * 10, offset * 12 };
    }
    return (Point) { 0, 0 };
}

static Point get_nothing_offset_assets(Map* map, int i, int j) {
    bool U_Floor = isFloor(map, i - 1, j);
    if (U_Floor) {
        bool UL = isFloor(map, i - 1, j - 1);
        bool UR = isFloor(map, i - 1, j + 1);
        if (UL && UR) return (Point) { offset * 13, offset * 11 };
        if (UL) return (Point) { offset * 14, offset * 11 };
        if (UR) return (Point) { offset * 12, offset * 11 };
        return (Point) { offset * 10, offset * 12 };
    }
    return (Point) { 0, 0 };
}

static void get_map_offset(Map* map) {
    // Calculate once to use it later on when draw() function called
    for (int i = 0; i < map->row; i++) {
        for (int j = 0; j < map->col; j++) {
            switch (map->map[i][j]) {
            case WALL:
                map->offset_assets[i][j] = get_wall_offset_assets(map, i, j);
                break;
            case SHOOTING_WALL:
                map->offset_assets[i][j] = get_wall_offset_assets(map, i, j);
                break;
            case FLOOR:
            case COIN:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;

            case TROPHY:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case HEALTH:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case HOLE:
                map->offset_assets[i][j] = get_hole_offset_assets(map, i, j);
                break;
            case DOOR_OPEN:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case DOOR_CLOSE:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case BUTTON:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;

            case D2O:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D2C:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case B2:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D3O:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D3C:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case B3:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D4O:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D4C:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case B4:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D5O:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D5C:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case B5:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D6O:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case D6C:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case B6:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case SPIKE:
                map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                break;
            case NOTHING:
            default:
                map->offset_assets[i][j] = (Point){ 0, 0 };
                break;
            }
        }
    }
}

static bool isWall(Map* map, int i, int j) {
    if (i < 0 || j < 0 || i >= map->row || j >= map->col) return false;
    if (map->map[i][j] == WALL || map->map[i][j] == SHOOTING_WALL) return true;
    return false;
}

static bool isFloor(Map* map, int i, int j) {
    if (i < 0 || j < 0 || i >= map->row || j >= map->col) return false;
    if (map->map[i][j] == FLOOR) return true;
    return false;
}

static bool isNothing(Map* map, int i, int j) {
    if (i < 0 || j < 0 || i >= map->row || j >= map->col) return true;
    if (map->map[i][j] == NOTHING || map->map[i][j] == HOLE) return true;
    return false;
}

/*
    Collision
 */
static bool tile_collision(Point player, Point tile_coord) {

    if (player.x < tile_coord.x + TILE_SIZE &&
        player.x + TILE_SIZE > tile_coord.x &&
        player.y < tile_coord.y + TILE_SIZE &&
        player.y + TILE_SIZE > tile_coord.y) {
        return true; // Rectangles collide
    }
    else {
        return false; // Rectangles do not collide
    }
}