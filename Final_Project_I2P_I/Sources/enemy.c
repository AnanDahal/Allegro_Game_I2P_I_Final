#define _USE_MATH_DEFINES
#include "enemy.h"
#include "utility.h"
#include "setting_scene.h"
#include <math.h>
#include <stdlib.h>

#define QUEUE_SIZE 2000



ALLEGRO_BITMAP* slimeBitmap;
ALLEGRO_BITMAP* skeletonBitmap;

// To check if p0 sprite and p1 sprite can go directly
static bool validLine(Map* map, Point p0, Point p1);
// To check if we can have a movement cutting directly to it
static bool bresenhamLine(Map* map, Point p0, Point p1);
// Find shortest path using BFS, could be better if you imeplement your own heap
static Point shortestPath(Map* map, Point src, Point dst);
// Calulate the movement speed to update and scaled it
static Point findScaledDistance(Point p1, Point p2);
// Return true if enemy have collision with unwalkable tiles in map
static bool isCollision(Point enemyCoord, Map* map);
// Return true if player collide with enemy
static bool playerCollision(Point enemyCoord, Point playerCoord);
static bool cocudosCollision(Point enemyCoord, Point cocudosCoord);


void initEnemy(void) {
    // Slime
    char* slimePath = "Assets/Slime.png";
    slimeBitmap = al_load_bitmap(slimePath);
    if (!slimeBitmap) {
        game_abort("Error Load Bitmap with path : %s", slimePath);
    }

    char* skeletonPath = "Assets/skeleton.png";
    skeletonBitmap = al_load_bitmap(skeletonPath);
    if (!skeletonBitmap) {
        game_abort("Error Load Bitmap with path : %s", skeletonPath);
    }
}

Enemy createEnemy(int row, int col, char type) {
    game_log("Creating Enemy at ( %d, %d )", row, col);

    Enemy enemy;
    memset(&enemy, 0, sizeof(enemy));

    enemy.animation_tick = 0;
    enemy.death_animation_tick = 0;
    enemy.status = ALIVE;

    enemy.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };

    switch (type) {
    case 'S':
        enemy.health = 100; // 100
        enemy.type = slime;
        enemy.speed = 3;
        enemy.image = slimeBitmap;
        break;
        
    case 'M':
        enemy.health = 200;
        enemy.type = magma;
        enemy.speed = 2;
        enemy.image = slimeBitmap;
        break;
    case 'L':
        enemy.health = 2000;
        enemy.type = skeleton;
        enemy.speed = 4;
        enemy.image = skeletonBitmap;
        break;
    default:
        enemy.health = 100;
        enemy.type = slime;
        enemy.speed = 3;
        enemy.image = slimeBitmap;
        break;
    }

    return enemy;
}

// Function to check if a tile is available for placing a coin
bool isTileAvailable(Map* map, int x, int y) {
    // Check bounds
    if (x < 0 || x >= map->row || y < 0 || y >= map->col) {
        return false; // Out of bounds
    }

    // Check if the tile is not a coin, wall, or hole
    return map->map[y][x] != COIN &&
        map->map[y][x] != S_COIN &&
        map->map[y][x] != WALL &&
        map->map[y][x] != HEALTH &&
        map->map[y][x] != HOLE &&
        map->map[y][x] != DOOR_CLOSE &&
        map->map[y][x] != BUTTON &&
        map->map[y][x] != B2 &&
        map->map[y][x] != B3 &&
        map->map[y][x] != B4 &&
        map->map[y][x] != B5 &&
        map->map[y][x] != B6 &&
        map->map[y][x] != D2C &&
        map->map[y][x] != D3C &&
        map->map[y][x] != D4C &&
        map->map[y][x] != D5C &&
        map->map[y][x] != D6C;
}

// Helper function to find the nearest available tile
void findAvailableTile(Map* map, int* tileX, int* tileY) {
    // Directions to check: right, left, down, up
    int directions[4][2] = {
        {1, 0},  // right
        {-1, 0}, // left
        {0, 1},  // down
        {0, -1}  // up
    };

    for (int i = 0; i < 4; i++) {
        int newX = *tileX + directions[i][0];
        int newY = *tileY + directions[i][1];
        if (isTileAvailable(map, newX, newY)) {
            *tileX = newX;
            *tileY = newY;
            return;
        }
    }
}

// Return True if the enemy is dead
bool updateEnemy(Enemy* enemy, Map* map, Player* player, Player* cocudos) {

    if (enemy->status == DYING && enemy->type != skeleton) {
        enemy->death_animation_tick++;
        
        if (enemy->death_animation_tick > 64) { 

            int tileX = enemy->coord.x / TILE_SIZE;
            int tileY = enemy->coord.y / TILE_SIZE;

            if (!isTileAvailable(map, tileX, tileY)) {
                findAvailableTile(map, &tileX, &tileY);
            }

            // Set the tile to a coin
            if (enemy->type == slime) {
                map->map[tileY][tileX] = COIN;
                map->coin_status[tileY][tileX] = APPEAR;
                coin_counter++;
            }
            else {
                map->map[tileY][tileX] = S_COIN;
                map->coin_status[tileY][tileX] = APPEAR;
                coin_counter += 3;
            }

            return true; 
        }
  
    }
    
    if (enemy->status == DYING && enemy->type == skeleton) {
        enemy->death_animation_tick += 2;  
        if (enemy->death_animation_tick >= 254) { 
            return true; 
        }
    }
    
    if (enemy->status != ALIVE) return false;

    enemy->animation_tick = (enemy->animation_tick + 1) % 64;

    if (enemy->animation_hit_tick > 0) {
        enemy->animation_tick = (enemy->animation_tick + 1) % 64;
        enemy->animation_hit_tick--;
    }

    if (enemy->knockback_CD > 0) {
        enemy->knockback_CD--;
        int next_x = enemy->coord.x + 4 * cos(enemy->knockback_angle);
        int next_y = enemy->coord.y + 4 * sin(enemy->knockback_angle);
        Point next;
        next = (Point){ next_x, enemy->coord.y };

        if (!isCollision(next, map)) {
            enemy->coord = next;
        }

        next = (Point){ enemy->coord.x, next_y };
        if (!isCollision(next, map)) {
            enemy->coord = next;
        }
        
    }
    else {

        Point delta;
        Point delta1 = shortestPath(map, enemy->coord, player->coord);
        Point delta2 = shortestPath(map, enemy->coord, cocudos->coord);

        // Check if directions to both targets are the same
        if (delta1.x == delta2.x && delta1.y == delta2.y) {
            delta = delta1;
        }
        else {
            // Calculate squared distances for comparison
            int dist1 = (player->coord.x - enemy->coord.x) * (player->coord.x - enemy->coord.x) +
                (player->coord.y - enemy->coord.y) * (player->coord.y - enemy->coord.y);
            int dist2 = (cocudos->coord.x - enemy->coord.x) * (cocudos->coord.x - enemy->coord.x) +
                (cocudos->coord.y - enemy->coord.y) * (cocudos->coord.y - enemy->coord.y);

            // Determine closer target
            if (dist1 < dist2) {
                // Move towards player
                if (abs(player->coord.x - enemy->coord.x) > abs(player->coord.y - enemy->coord.y)) {
                    // Horizontal movement
                    delta = (Point){ player->coord.x > enemy->coord.x ? 1 : -1, 0 };
                }
                else {
                    // Vertical movement
                    delta = (Point){ 0, player->coord.y > enemy->coord.y ? 1 : -1 };
                }
            }
            else {
                // Move towards cocudos
                if (abs(cocudos->coord.x - enemy->coord.x) > abs(cocudos->coord.y - enemy->coord.y)) {
                    // Horizontal movement
                    delta = (Point){ cocudos->coord.x > enemy->coord.x ? 1 : -1, 0 };
                }
                else {
                    // Vertical movement
                    delta = (Point){ 0, cocudos->coord.y > enemy->coord.y ? 1 : -1 };
                }
            }
        }

        Point next, prev = enemy->coord;

        if (delta.x > 0) enemy->dir = RIGHT;
        if (delta.x < 0) enemy->dir = LEFT;

        next = (Point){ enemy->coord.x + delta.x * enemy->speed, enemy->coord.y };
        if (!isCollision(next, map))
            enemy->coord = next;

        next = (Point){ enemy->coord.x, enemy->coord.y + delta.y * enemy->speed };
        if (!isCollision(next, map))
            enemy->coord = next;

        // To fix bug if the enemy need to move a little bit, the speed will not be use
        if (enemy->coord.x == prev.x && enemy->coord.y == prev.y) {
            next = (Point){ enemy->coord.x + delta.x, enemy->coord.y };
            if (!isCollision(next, map))
                enemy->coord = next;

            next = (Point){ enemy->coord.x, enemy->coord.y + delta.y };
            if (!isCollision(next, map))
                enemy->coord = next;
        }

        if (enemy->type == slime) {
            if (playerCollision(enemy->coord, player->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 32;
                hitPlayer(player, enemy->coord, 10);
            }
            else if (cocudosCollision(enemy->coord, cocudos->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 32;
                hitPlayer(cocudos, enemy->coord, 10);
            }
        }
        if (enemy->type == magma) {
            if (playerCollision(enemy->coord, player->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 32;
                hitPlayer(player, enemy->coord, 20);
            }
            else if (cocudosCollision(enemy->coord, cocudos->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 32;
                hitPlayer(cocudos, enemy->coord, 20);
            }
        }
        if (enemy->type == skeleton) {
            if (playerCollision(enemy->coord, player->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 32;
                hitPlayer(player, enemy->coord, 30);
            }
            else if (cocudosCollision(enemy->coord, cocudos->coord) && enemy->animation_hit_tick == 0) {
                enemy->animation_tick = 0;
                enemy->animation_hit_tick = 832;
                hitPlayer(cocudos, enemy->coord, 30);
            }
        }
    }

    return false;
}

void drawEnemy(Enemy* enemy, Point cam) {

    int dy = enemy->coord.y - cam.y; // destiny y axis
    int dx = enemy->coord.x - cam.x; // destiny x axis
    int offset;

    if (enemy->status == ALIVE) {
        if (enemy->type != skeleton) {
            offset = 16 * (int)(enemy->animation_tick / 8);
            if (enemy->animation_hit_tick > 0) {
                offset += 16 * 8;
            }
        }
        else {
            if(!enemy->knockback_CD) offset = 64 * (int)(enemy->animation_tick / 2);
            else offset = 64 * (int)(enemy->animation_tick / 16);

            if (offset >= 768 && !enemy->knockback_CD) {
                offset = offset % 768;
            }
            else if(offset >= 192 && enemy->knockback_CD){
                offset = offset % 192;
            }
        }
        

        int flag = enemy->dir == RIGHT ? 1 : 0;
        int flag2 = enemy->dir == RIGHT ? 0 : 1;
        int tint_red = enemy->knockback_CD > 0 ? 255 : 0;
        int tint_red2 = enemy->knockback_CD > 0 ? 255 : 160;

        int temp = 2;

        if (enemy->knockback_CD > 0 && enemy->type == skeleton) {
            temp = 4;
        }
        else if (enemy->animation_hit_tick) {
            temp = 0;
        }

        if (enemy->type == slime) {
            al_draw_tinted_scaled_rotated_bitmap_region(enemy->image, offset, 0, 16, 16, al_map_rgb(tint_red, 255, 255),
                0, 0, dx, dy, TILE_SIZE / 16, TILE_SIZE / 16,
                0, flag);
        }

        if (enemy->type == magma) {
            al_draw_tinted_scaled_rotated_bitmap_region(enemy->image, offset, 0, 16, 16, al_map_rgb(tint_red2, 32, 240),
                0, 0, dx, dy, TILE_SIZE / 12, TILE_SIZE / 12,
                0, flag);
        }

        if (enemy->type == skeleton) {
            al_draw_tinted_scaled_rotated_bitmap_region(enemy->image, offset, temp * 64, 64, 64, al_map_rgb(255, 255, 255),
                0, 0, dx - 25, dy - 25, TILE_SIZE / 25, TILE_SIZE / 25,
                0, flag2);
        }

        // Health bar dimensions and colors
        int bar_width = TILE_SIZE;   
        int bar_height = 5;         
        int bar_x = dx;             
        int bar_y = dy - bar_height - 2; 
        int max_health;
        if (enemy->type == magma) {
            max_health = 200; 
        }
        else if (enemy->type == slime) {
            max_health = 100; 
        }
        else if (enemy->type == skeleton) {
            max_health = 2000;
        }

        int health_width = (enemy->health * bar_width) / max_health; 

        if (health_width > bar_width) health_width = bar_width;
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(100, 100, 100)); // Gray background
        al_draw_filled_rectangle(bar_x, bar_y, bar_x + health_width, bar_y + bar_height, al_map_rgb(255, 0, 0)); // Red for current health
        al_draw_rectangle(bar_x, bar_y, bar_x + bar_width, bar_y + bar_height, al_map_rgb(0, 0, 0), 2); // Black border

    }

    else if (enemy->status == DYING) {

        
        if (enemy->type != skeleton) {
            int flag = enemy->dir == RIGHT ? 0 : 1;
            int frame = enemy->death_animation_tick / 8; 
            
            al_draw_tinted_scaled_rotated_bitmap_region(enemy->image, frame * 16, 16, 16, 16, al_map_rgb(255, 255, 255), 0, 0, dx, dy, TILE_SIZE / 16, TILE_SIZE / 16, 0, flag);
        }
        else {
            int flag = enemy->dir == RIGHT ? 1 : 0;
            int frame = enemy->death_animation_tick / 16; // Frame derived from tick
            al_draw_tinted_scaled_rotated_bitmap_region(enemy->image,
                frame * 64, 64, 64, 64,
                al_map_rgb(255, 255, 255), 0, 0,
                dx - 25, dy - 25,
                TILE_SIZE / 25, TILE_SIZE / 25, 0, flag);
            
        }

    }

#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
}


void destroyEnemy(Enemy* enemy) {

}

void terminateEnemy(void) {
    al_destroy_bitmap(slimeBitmap);
    al_destroy_bitmap(skeletonBitmap);
}

void hitEnemy(Enemy* enemy, int damage, float angle) {

    enemy->health -= damage;
    if (enemy->health <= 0) {
        enemy->status = DYING;
    }

    enemy->knockback_angle = angle;
    if (yellow_eq) {
        enemy->knockback_CD = 10;
    }
    else if (orange_eq) {
        enemy->knockback_CD = 15;
    }
    else if (fireball_eq) {
        enemy->knockback_CD = 25;
    }
}



enemyNode* createEnemyList(void) {
    enemyNode* dummyhead = (enemyNode*)malloc(sizeof(enemyNode));
    dummyhead->next = NULL;
    return dummyhead;
}

void insertEnemyList(enemyNode* dummyhead, Enemy _enemy) {
    enemyNode* tmp = (enemyNode*)malloc(sizeof(enemyNode));
    tmp->enemy = _enemy;
    tmp->next = dummyhead->next;
    dummyhead->next = tmp;
}

void updateEnemyList(enemyNode* dummyhead, Map* map, Player* player, Player* cocudos) {
    enemyNode* cur = dummyhead->next;
    enemyNode* prev = dummyhead;

    while (cur != NULL) {
        bool shouldDelete = updateEnemy(&cur->enemy, map, player, cocudos);
        if (shouldDelete) {
            prev->next = cur->next;
            destroyEnemy(&cur->enemy);
            free(cur);
            cur = prev->next;
        }
        else {
            prev = cur;
            cur = cur->next;
        }
    }
}

void drawEnemyList(enemyNode* dummyhead, Point cam) {
    enemyNode* cur = dummyhead->next;

    while (cur != NULL) {
        drawEnemy(&cur->enemy, cam);
        cur = cur->next;
    }
}

void destroyEnemyList(enemyNode* dummyhead) {
    while (dummyhead != NULL) {
        enemyNode* del = dummyhead;
        dummyhead = dummyhead->next;
        destroyEnemy(&del->enemy);
        free(del);
    }
}

static bool validLine(Map* map, Point p0, Point p1) {
    int offsetX[4] = { 0, 0, TILE_SIZE - 1, TILE_SIZE - 1 };
    int offsetY[4] = { 0, TILE_SIZE - 1, 0, TILE_SIZE - 1 };

    for (int i = 0; i < 4; i++) {
        Point n1 = (Point){
            p0.x + offsetX[i],
            p0.y + offsetY[i]
        };

        Point n2 = (Point){
            p1.x + offsetX[i],
            p1.y + offsetY[i]
        };
        if (bresenhamLine(map, n1, n2)) return false;
    }

    return true;
}

static Point shortestPath(Map* map, Point enemy, Point player) {
    // Point enemy & player is pixel coordinate
    static DIRECTION dir[MAX_MAP_ROW][MAX_MAP_COL]; // to backtrack from dst to src
    static bool visit[MAX_MAP_ROW][MAX_MAP_COL];
    memset(visit, 0, sizeof(visit));

    // Point declared below is not coordinate of pixel, but COORDINATE OF ARRAY!
    Point src = (Point){
        enemy.y / TILE_SIZE,
        enemy.x / TILE_SIZE
    };
    Point dst = (Point){
        player.y / TILE_SIZE,
        player.x / TILE_SIZE
    };

    static Point Queue[QUEUE_SIZE];
    int front = 0, rear = 0;

    Queue[rear++] = src;
    bool found = false;

    // Movement set
    static int dx[4] = { 1, -1, 0, 0 };
    static int dy[4] = { 0, 0, 1, -1 };
    static DIRECTION move[4] = { UP, DOWN, LEFT, RIGHT }; // To backtrack

    while (front != rear) {
        Point cur = Queue[front++];

        // Found the destiny
        if (cur.x == dst.x && cur.y == dst.y) {
            found = true;
            break;
        };

        for (int i = 0; i < 4; i++) {
            Point next = (Point){
                cur.x + dx[i],
                cur.y + dy[i]
            };

            if (next.x < 0 || next.y < 0) continue;
            if (next.x >= map->row || next.y >= map->col) continue;

            if (isWalkable(map->map[next.x][next.y]) && !visit[next.x][next.y]) {
                dir[next.x][next.y] = move[i];
                visit[next.x][next.y] = true;
                Queue[rear++] = next;
            }
        }
    }

    // Toward a String-Pulling Approach to Path Smoothing on Grid Graphs
    // http://idm-lab.org/bib/abstracts/papers/socs20c.pdf
    if (found) {
        if (validLine(map, enemy, player))
            return findScaledDistance(enemy, player);

        int max_iteration = 1000;
        Point it = dst;
        while (max_iteration--) {
            Point translate_it = (Point){
                it.y * TILE_SIZE,
                it.x * TILE_SIZE
            };

            if (validLine(map, enemy, translate_it))
                return findScaledDistance(enemy, translate_it);

            switch (dir[it.x][it.y]) {
            case UP:
                it.x--;
                break;
            case DOWN:
                it.x++;
                break;
            case LEFT:
                it.y--;
                break;
            case RIGHT:
                it.y++;
                break;
            default:
                goto END;
            }

        }
    END:
        game_log("FAILED TO ITERATE");
    }
    return (Point) { 0, 0 };
}

static bool bresenhamLine(Map* map, Point p0, Point p1) {
    int dx = abs(p1.x - p0.x), sx = p0.x < p1.x ? 1 : -1;
    int dy = -abs(p1.y - p0.y), sy = p0.y < p1.y ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        int tile_x = p0.y / TILE_SIZE;
        int tile_y = p0.x / TILE_SIZE;
        if (!isWalkable(map->map[tile_x][tile_y])) return true;

        if (p0.x == p1.x && p0.y == p1.y) break;
        e2 = 2 * err;

        if (e2 > dy) {
            err += dy;
            p0.x += sx;
        }
        else if (e2 < dx) {
            err += dx;
            p0.y += sy;
        }
    }

    return false;
}

static Point findScaledDistance(Point p1, Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;

    double d = sqrt(dx * dx + dy * dy);

    // Floating error fix, when smaller than delta it will be immediately 0
    if (d < 0.001) {
        return (Point) { 0, 0 };
    }

    double dxUnit = dx / d;
    double dyUnit = dy / d;

    return (Point) { round(dxUnit), round(dyUnit) };
}


static bool playerCollision(Point enemyCoord, Point playerCoord) {
    // Rectangle & Rectanlge Collision
    if (enemyCoord.x < playerCoord.x + TILE_SIZE &&
        enemyCoord.x + TILE_SIZE > playerCoord.x &&
        enemyCoord.y < playerCoord.y + TILE_SIZE &&
        enemyCoord.y + TILE_SIZE > playerCoord.y) {
        return true;
    }
    else {
        return false;
    }
}

static bool cocudosCollision(Point enemyCoord, Point cocudosCoord) {
    // Rectangle & Rectanlge Collision
    if (enemyCoord.x < cocudosCoord.x + TILE_SIZE &&
            enemyCoord.x + TILE_SIZE > cocudosCoord.x &&
            enemyCoord.y < cocudosCoord.y + TILE_SIZE &&
            enemyCoord.y + TILE_SIZE > cocudosCoord.y) {
        return true;
    }
    else {
        return false;
    }
}

static bool isCollision(Point enemyCoord, Map* map) {
    if (enemyCoord.x < 0 ||
        enemyCoord.y < 0 ||
        (enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    if (!isWalkable(map->map[enemyCoord.y / TILE_SIZE][enemyCoord.x / TILE_SIZE])) return true;
    if (!isWalkable(map->map[(enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE][enemyCoord.x / TILE_SIZE])) return true;
    if (!isWalkable(map->map[enemyCoord.y / TILE_SIZE][(enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;
    if (!isWalkable(map->map[(enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE][(enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE])) return true;


    return false;
}
