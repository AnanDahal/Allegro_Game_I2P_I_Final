#include "bullet.h"
#include <math.h>

Bullet create_bullet(char* bullet_path, PointFloat coord, float angle, float speed, float damage, bool playershoot, bool shootingwallshoot) {
    Bullet bullet;
    bullet.coord = coord;
    bullet.angle = angle;
    bullet.speed = speed;
    bullet.damage = damage;
    bullet.image = al_load_bitmap(bullet_path);
    bullet.playershoot = playershoot;
    bullet.shootingwallshoot = shootingwallshoot;

    // For better repositioning
    bullet.coord.x += (float)(TILE_SIZE / 2) * cos(bullet.angle);
    bullet.coord.y += (float)(TILE_SIZE / 2) * sin(bullet.angle);

    return bullet;
}

Point bulcoord;


// Return true if the bullet collides, so it will be deleted from the list
bool update_bullet(Bullet* bullet, enemyNode* enemyList, Map* map, Player* player, Player* cocudos) {
    
    bullet->coord.x += bullet->speed * cos(bullet->angle);
    bullet->coord.y += bullet->speed * sin(bullet->angle);
    

    int tile_y = (int)(bullet->coord.x / TILE_SIZE);
    int tile_x = (int)(bullet->coord.y / TILE_SIZE);
    if (tile_x < 0 || tile_x >= map->row || tile_y < 0 || tile_y >= map->col 
        || map->map[tile_x][tile_y] == WALL || map->map[tile_x][tile_y] == SHOOTING_WALL
        || map->map[tile_x][tile_y] == DOOR_CLOSE) {
        return true;
    }
    
    enemyNode* cur = enemyList->next;
    if (!bullet->shootingwallshoot) {
        while (cur != NULL) {
            Point enemyCoord = cur->enemy.coord;

            if (tile_y >= enemyCoord.x / TILE_SIZE && tile_x >= enemyCoord.y / TILE_SIZE && tile_x <= (enemyCoord.y + TILE_SIZE - 1) / TILE_SIZE && tile_y <= (enemyCoord.x + TILE_SIZE - 1) / TILE_SIZE) {
                hitEnemy(&cur->enemy, bullet->damage, bullet->angle);
                return true;
            }
            //game_log("BUL COORD X == %d Y == %d PLAYER COORD X == %d Y == %d ENEMYCOORD X == %d Y == %d", bulcoord.x, bulcoord.y, player->coord.x, player->coord.y, cur->enemy.coord.x, cur->enemy.coord.y);

            cur = cur->next;
        }
    }
    

    bulcoord.x = (int)bullet->coord.x;
    bulcoord.y = (int)bullet->coord.y;


    if (!bullet->playershoot) {
        if (tile_y >= player->coord.x / TILE_SIZE && tile_x >= player->coord.y / TILE_SIZE &&
            tile_x <= (player->coord.y + TILE_SIZE - 1) / TILE_SIZE && tile_y <= (player->coord.x + TILE_SIZE - 1) / TILE_SIZE) {
            //game_log("PLAYERS HIT");
            hitPlayer(player, bulcoord, bullet->damage);
            return true;
        }
    }
    

    if (tile_y >= cocudos->coord.x / TILE_SIZE && tile_x >= cocudos->coord.y / TILE_SIZE &&
        tile_x <= (cocudos->coord.y + TILE_SIZE - 1) / TILE_SIZE && tile_y <= (cocudos->coord.x + TILE_SIZE - 1) / TILE_SIZE) {
        //game_log("COCUDOS HIT");
        hitPlayer(cocudos, bulcoord, bullet->damage);
        return true;
    }


    return false;
}

void draw_bullet(Bullet* bullet, Point camera) {
    float scale = TILE_SIZE / 16;
    al_draw_bitmap(bullet->image, bullet->coord.x - camera.x - 16, bullet->coord.y - camera.y - 16, 0);
}

void destroy_bullet(Bullet* bullet) {
    // In case you are using immage asset for the bullet, free it here
    if (bullet && bullet->image) {
        al_destroy_bitmap(bullet->image);
    }
}

/*
    LINKED LIST IMPLEMENTATION FOR BULLET
    THERE ARE :
        (1) CREATE      : INITIALIZING DUMMY HEAD
        (2) INSERT      : INSERT NEW NODE OF BULLET
        (3) UPDATE      : ITERATE EVERYTHING AND UPDATE THE BULLET
        (4) DRAW        : ITERATE EVERYTHING AND DRAW THE BULLET
        (5) DESTROY     : DESTROY THE LINKED LIST
 */

BulletNode* createBulletList(void) {
    BulletNode* dummyhead = (BulletNode*)malloc(sizeof(BulletNode));
    dummyhead->next = NULL;
    return dummyhead;
}

void insertBulletList(BulletNode* dummyhead, Bullet bullet) {
    BulletNode* tmp = (BulletNode*)malloc(sizeof(BulletNode));
    tmp->bullet = bullet;
    tmp->next = dummyhead->next;
    dummyhead->next = tmp;
}

void updateBulletList(BulletNode* dummyhead, enemyNode* enemyList, Map* map, Player* player, Player* cocudos) {
    BulletNode* cur = dummyhead->next;
    BulletNode* prev = dummyhead;

    while (cur != NULL) {
        bool shouldDelete = update_bullet(&cur->bullet, enemyList, map, player, cocudos);
        if (shouldDelete) {
            prev->next = cur->next;
            destroy_bullet(&cur->bullet);
            free(cur);
            cur = prev;
        }
        prev = cur;
        cur = cur->next;
    }
}

void drawBulletList(BulletNode* dummyhead, Point camera) {
    BulletNode* cur = dummyhead->next;

    while (cur != NULL) {
        draw_bullet(&cur->bullet, camera);
        cur = cur->next;
    }
}

void destroyBulletList(BulletNode* dummyhead) {
    BulletNode* del = dummyhead;
    dummyhead = dummyhead->next;
    free(del); // No Images
    while (dummyhead != NULL) {
        BulletNode* del = dummyhead;
        dummyhead = dummyhead->next;
        destroy_bullet(&del->bullet);
        free(del);
    }
}