#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"

#include <math.h>

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

// Weapon
Weapon weapon; 
int coins_obtained;

static void init(void){
    
    initEnemy();
    
    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    weapon = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/wii_music.mp3");
}

static void update(void){
    /*
        [TODO Homework]
        
        Change the scene if winning/losing to win/lose scene
    */

    // Start HW 
    
 /*   if (player.health == 0) {
        change_scene(create_lose_scene());
    }
    else if (coins_obtained == 3) {
        change_scene(create_win_scene());
    }
    */


    // End HW

    update_player(&player, &map);

    Point Camera;
    /*
        [TODO HACKATHON 1-3]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    // Start 1 - 3
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);
    // End 1 - 3

    if (player.health != 0) {
        updateEnemyList(enemyList, &map, &player);
        update_weapon(&weapon, bulletList, player.coord, Camera);
        updateBulletList(bulletList, enemyList, &map);
        update_map(&map, player.coord, &coins_obtained);
    }
    
    
}

static void draw(void){
    Point Camera;
    /*
        [TODO HACKATHON 1-4]
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Just copy from the [TODO HACKATHON 1-3]
    */

    // Start 1 - 4
    Camera.x = player.coord.x - (SCREEN_W / 2);
    Camera.y = player.coord.y - (SCREEN_H / 2);
    // End 1 - 4

    // Draw
    
    draw_map(&map, Camera);
    draw_player(&player, Camera);
    

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */

    // Start HW

    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);

    if (player.health != 0) {
        drawEnemyList(enemyList, Camera);
        drawBulletList(bulletList, Camera);
        draw_weapon(&weapon, player.coord, Camera);

        al_draw_scaled_bitmap(map.coin_assets, 0, 0, 16, 16, 0, 0, 100, 100, 0);

        al_draw_text(P3_FONT, al_map_rgb(255, 255, 255), 52, 75, ALLEGRO_ALIGN_CENTER, coin_text);

        for (int i = 0; i < player.health / 10; i++) {
            al_draw_scaled_bitmap(al_load_bitmap("assets/heart.png"), 0, 0, 32, 32, (i * 65) + (1300 + ((50 - player.health) * 6)), 0, 100, 100, 0);
        }
    }


    // End HW
}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}
