#include <string.h>
#include <math.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"

#include <math.h>
static int timer_countdown;
Player player; // Player
Player cocudos; // Cocudos
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet 
// Weapon
Weapon weapon; 
// initialize coin obtained
coins_obtained = 0;


static void init(void){
    timer_countdown = 60;
    initEnemy();

    
    map = create_map("Assets/map0.txt", 0);

    player = create_player("Assets/panda2.png", map.SpawnP.x, map.SpawnP.y, 1);
    cocudos = create_player("Assets/player.png", map.SpawnJ.x, map.SpawnJ.y, 0);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    // cooldown , speed, damage
    
    if (guns_eq) {
        if (yellow_eq) {
            weapon = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
        }
        else if (orange_eq) {
            weapon = create_weapon("Assets/guns.png", "Assets/orange_bullet.png", 16, 8, 15);
        }
        else if (fireball_eq) {
            weapon = create_weapon("Assets/guns.png", "Assets/fireball.png", 16, 8, 25);
        }
    }
    else if (sniper_eq) {
        if (yellow_eq) {
            weapon = create_weapon("Assets/sniper.png", "Assets/yellow_bullet.png", 50, 20, 50);
        }
        else if (orange_eq) {
            weapon = create_weapon("Assets/sniper.png", "Assets/orange_bullet.png", 50, 20, 75);
        }
        else if (fireball_eq) {
            weapon = create_weapon("Assets/sniper.png", "Assets/fireball.png", 50, 20, 100);
        }
    }
    else if (machine_gun_eq) {
        if (yellow_eq) {
            weapon = create_weapon("Assets/machine_gun.png", "Assets/yellow_bullet.png", 5, 15, 20);
        }
        else if (orange_eq) {
            weapon = create_weapon("Assets/machine_gun.png", "Assets/orange_bullet.png", 5, 15, 25);
        }
        else if (fireball_eq) {
            weapon = create_weapon("Assets/machine_gun.png", "Assets/fireball.png", 5, 15, 34);
        }
    }

    //weapon = create_weapon("Assets/guns.png", "Assets/orange_bullet.png", 16, 8, 10);
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    change_bgm("Assets/audio/wii_music.mp3");
}

static void update(void){
    /*
        [TODO Homework]
        
        Change the scene if winning/losing to win/lose scene
    */

    // Start HW 
    
    int distance = sqrt((player.coord.x - cocudos.coord.x) * (player.coord.x - cocudos.coord.x) +
        (player.coord.y - cocudos.coord.y) * (player.coord.y - cocudos.coord.y));

    if (distance >= 400) {
        map.far = true;
    }
    else {
        map.far = false;
    }

    if (distance >= 500) {
        map.toofar = true;
    }
    else {
        map.toofar = false;
    }


    if (map.win) {
        timer_countdown--;
        if (timer_countdown == 0) {
            change_scene(create_win_scene());
            return;
        }
    }
    
    if(player.health <= 0 || cocudos.health <= 0 || map.toofar) {
        timer_countdown--;
        if (timer_countdown == 0) {
            coins_obtained = 0;
            change_scene(create_lose_scene());
            return;
        }
    }

    if (add_health) {
        add_health = false;
        player.health += 10;
        cocudos.health += 10;
    }

    

    // End HW

    update_player(&player, &map);
    update_player2(&cocudos, &map);

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

    
    
    updateEnemyList(enemyList, &map, &player, &cocudos);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
 
    update_map(&map, player.coord, cocudos.coord, &coins_obtained);
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
    draw_player2(&cocudos, Camera);
    

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */

    // Start HW

    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);

    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_weapon(&weapon, player.coord, Camera);

    al_draw_scaled_bitmap(map.coin_assets, 0, 0, 16, 16, 0, 0, 100, 100, 0);

    al_draw_text(P3_FONT, al_map_rgb(255, 255, 255), 52, 75, ALLEGRO_ALIGN_CENTER, coin_text);

    al_draw_scaled_bitmap(player.image, 0, 0, 32, 32, 1590, 20, 50, 50, 0);
    al_draw_scaled_bitmap(cocudos.image, 0, 0, 48, 48, 1590, 75, 50, 50, 1);

    if (map.far) {
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(146, 161, 185), // Hover changes color
            SCREEN_W / 2,
            50,
            ALLEGRO_ALIGN_CENTER,
            "PRINCESS IS TOO FAR"
        );
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(255, 69, 69), // Hover changes color
            SCREEN_W / 2,
            53,
            ALLEGRO_ALIGN_CENTER,
            "PRINCESS IS TOO FAR"
        );
    }

    for (int i = 0; i < player.health / 10; i++) {
        al_draw_scaled_bitmap(al_load_bitmap("assets/heart.png"), 0, 0, 32, 32, (i * 65) + (1240 + ((50 - player.health) * 6)), 0, 100, 100, 0);
    }

    for (int i = 0; i < cocudos.health / 10; i++) {
        al_draw_scaled_bitmap(al_load_bitmap("assets/heart.png"), 0, 0, 32, 32, (i * 65) + (1240 + ((50 - cocudos.health) * 6)), 60, 100, 100, 0);
    }


    // End HW
}

static void destroy(void){
    delete_player(&player);
    delete_player(&cocudos);
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

static Button restartButton;
static Button quitButton;
static ALLEGRO_BITMAP* lose_bitmap;



static void init_lose(void) {
    int button_width = 300;
    int button_height = 80;
    int button_spacing = 30;

    lose_bitmap = al_load_bitmap("Assets/lose_scene.png");

    if (!lose_bitmap) {
        game_abort("Failed to load lose bitmap");
    }

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    restartButton = button_create(
        (SCREEN_W - button_width) / 2, // Centered horizontally
        SCREEN_H / 2 - (button_height + button_spacing), // Positioned above center
        button_width,
        button_height,
        "Assets/UI_Button.png",
        "Assets/UI_Button_hovered.png"
    );

    quitButton = button_create(
        (SCREEN_W - button_width) / 2, // Centered horizontally
        SCREEN_H / 2 + button_spacing, // Positioned below center
        button_width,
        button_height,
        "Assets/UI_Button.png",
        "Assets/UI_Button_hovered.png"
    );

    // Change background music to a dramatic or melancholic track
    change_bgm("Assets/audio/sleepless_deathbed.mp3");
}


static void update_lose(void) {
    update_button(&quitButton);
    update_button(&restartButton);

    if (restartButton.hovered && (mouseState.buttons & 1)) { // Check if hovered and left mouse button is pressed.
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_loading_scene());
    }

    if (quitButton.hovered && (mouseState.buttons & 1)) { // Check if hovered and left mouse button is pressed.
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        exit(0);
    }
}

static void draw_lose(void) {
    // Draw a background color or image
    al_draw_scaled_bitmap(lose_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);

    // Draw the "YOU LOSE" text with a glow effect
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        150,
        ALLEGRO_ALIGN_CENTER,
        "YOU LOSE"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(255, 69, 69),
        SCREEN_W / 2,
        145,
        ALLEGRO_ALIGN_CENTER,
        "YOU LOSE"
    );

    // Draw Restart button
    draw_button(restartButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110), // Hover changes color
        SCREEN_W / 2,
        restartButton.y + (restartButton.h / 2) - (al_get_font_line_height(P2_FONT) / 2) + (restartButton.hovered ? 7 : 0), // Shift downward slightly if hovered
        ALLEGRO_ALIGN_CENTER,
        "Restart"
    );

    // Draw Quit button
    draw_button(quitButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110), // Hover changes color
        SCREEN_W / 2,
        quitButton.y + (quitButton.h / 2) - (al_get_font_line_height(P2_FONT) / 2) + (quitButton.hovered ? 7 : 0), // Shift downward slightly if hovered
        ALLEGRO_ALIGN_CENTER,
        "Quit"
    );

    // Optional: Add additional message
    al_draw_text(
        P3_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        100,
        ALLEGRO_ALIGN_CENTER,
        "Better luck next time!"
    );
}


static void destroy_lose(void) {
    destroy_button(&restartButton);
    destroy_button(&quitButton);
    al_destroy_bitmap(lose_bitmap);
}

Scene create_lose_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "lose";
    scene.init = &init_lose;
    scene.draw = &draw_lose;
    scene.update = &update_lose;
    scene.destroy = &destroy_lose;

    return scene;
}

// Button declarations
static Button mainMenuButton;
static Button quitButton;
static ALLEGRO_BITMAP* win_bitmap;


// Initialization function for the win scene
static void init_win(void) {
    // Create buttons
    mainMenuButton = button_create(SCREEN_W / 2 - 200, 500, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    quitButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    win_bitmap = al_load_bitmap("Assets/win_scene.png");

    if (!win_bitmap) {
        game_abort("Failed to load win bitmap");
    }

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    // Change background music
    change_bgm("Assets/audio/victory_theme.mp3");
}

// Update function for the win scene
static void update_win(void) {
    // Update button states
    update_button(&mainMenuButton);
    update_button(&quitButton);

    // Handle button actions
    if (mainMenuButton.hovered && (mouseState.buttons & 1)) { // Main Menu button pressed
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_menu_scene());
    }
    if (quitButton.hovered && (mouseState.buttons & 1)) { // Quit button pressed
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        coins_obtained = 0;
        al_rest(0.2); // Add a small delay to avoid abrupt exit
        exit(0);
    }
}

// Draw function for the win scene
static void draw_win(void) {
    // Draw background color
    al_draw_scaled_bitmap(win_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);

    // Draw the "YOU WIN!" title with a glow effect

    
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(200, 230, 255),
        SCREEN_W / 2,
        200,
        ALLEGRO_ALIGN_CENTER,
        "YOU WIN!"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(255, 255, 100),
        SCREEN_W / 2,
        195,
        ALLEGRO_ALIGN_CENTER,
        "YOU WIN!"
    );

    // Draw the Main Menu button
    draw_button(mainMenuButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        mainMenuButton.y + (mainMenuButton.h / 2) - (al_get_font_line_height(P2_FONT) / 2) + (mainMenuButton.hovered ? 7 : 0),
        ALLEGRO_ALIGN_CENTER,
        "Main Menu"
    );

    // Draw the Quit button
    draw_button(quitButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        quitButton.y + (quitButton.h / 2) - (al_get_font_line_height(P2_FONT) / 2) + (quitButton.hovered ? 7 : 0),
        ALLEGRO_ALIGN_CENTER,
        "Quit"
    );
}

// Destroy function for the win scene
static void destroy_win(void) {
    destroy_button(&mainMenuButton);
    destroy_button(&quitButton);
    al_destroy_bitmap(win_bitmap);
}

// Function to create the win scene
Scene create_win_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "win";
    scene.init = &init_win;
    scene.update = &update_win;
    scene.draw = &draw_win;
    scene.destroy = &destroy_win;

    return scene;
}

