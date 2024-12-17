#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <string.h>
#include "menu_scene.h"
#include "loading_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static int timer_countdown;
static int scene;
bool starttimer = false;

static ALLEGRO_BITMAP* player_bitmap;

static ALLEGRO_BITMAP* player_bitmap1;
static ALLEGRO_BITMAP* player_bitmap2;
static ALLEGRO_BITMAP* player_bitmap3;
static ALLEGRO_BITMAP* player_bitmap4;
static ALLEGRO_BITMAP* player_bitmap5;
static ALLEGRO_BITMAP* cocudos_bitmap1;
static ALLEGRO_BITMAP* cocudos_bitmap2;
static ALLEGRO_BITMAP* cocudos_bitmap3;
static ALLEGRO_BITMAP* cocudos_bitmap4;
static ALLEGRO_BITMAP* cocudos_bitmap5;

// Add a pointer array to hold the background bitmaps
static ALLEGRO_BITMAP* backgrounds[10];

static void init(void) {
    timer_countdown = 60;
    scene = 0;


    // Load background and player bitmaps
    player_bitmap = al_load_bitmap("Assets/panda2.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    backgrounds[0] = al_load_bitmap("Assets/loading_c1.png");
    backgrounds[1] = al_load_bitmap("Assets/loading_p1.png");
    backgrounds[2] = al_load_bitmap("Assets/loading_c2.png");
    backgrounds[3] = al_load_bitmap("Assets/loading_p2.png");
    backgrounds[4] = al_load_bitmap("Assets/loading_c3.png");
    backgrounds[5] = al_load_bitmap("Assets/loading_p3.png");
    backgrounds[6] = al_load_bitmap("Assets/loading_c4.png");
    backgrounds[7] = al_load_bitmap("Assets/loading_p4.png");
    backgrounds[8] = al_load_bitmap("Assets/loading_c5.png");
    backgrounds[9] = al_load_bitmap("Assets/loading_p5.png");

    // Check if all backgrounds loaded
    for (int i = 0; i < 10; i++) {
        if (!backgrounds[i]) {
            game_abort("Failed to load background bitmap %d", i);
        }
    }

    change_bgm("Assets/audio/harry_potter.mp3");
}
bool click = false;
static void update(void) {
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state);

    if (!al_key_down(&key_state, ALLEGRO_KEY_SPACE) && !mouseState.buttons) {
        click = false;
    }

    // If SPACEBAR is pressed, increment scene
    if (mouseState.buttons && scene < 10 && !starttimer && !click) {
        scene++;
        click = true;
    }

    // If ENTER is pressed, set scene to 10 and start timer
    if ((scene == 10 || al_key_down(&key_state, ALLEGRO_KEY_SPACE)) && !starttimer) {
        scene = 10;
        starttimer = true;
        click = true;
    }

    if (starttimer) {
        timer_countdown--;
    }

    if (timer_countdown == 0) {
        change_scene(create_game_scene());
    }
}

static void draw(void) {
    if (scene < 10 && !starttimer) {
        // Draw the background scaled to the screen size (1640x1000)
        al_draw_scaled_bitmap(backgrounds[scene],
            0, 0,                               // Source X, Y (top-left corner of the bitmap)
            al_get_bitmap_width(backgrounds[scene]),  // Source width
            al_get_bitmap_height(backgrounds[scene]), // Source height
            0, 0,                               // Destination X, Y (top-left corner on screen)
            SCREEN_W, SCREEN_H,                 // Destination width and height
            0                                   // Flags (0 = no flags)
        );
        
        al_draw_text(
            P1_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            900 + 28,
            ALLEGRO_ALIGN_CENTER,
            "Click to Continue     [SPACE] to skip"
        );
        al_draw_text(
            P1_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            900 + 31,
            ALLEGRO_ALIGN_CENTER,
            "Click to Continue     [SPACE] to skip"
        );
    }
    else {
        // Final scene: draw player and text
        al_clear_to_color(al_map_rgb(0, 0, 0)); // Clear screen to black
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(146, 161, 185),
            SCREEN_W / 2,
            SCREEN_H / 2 - 50,
            ALLEGRO_ALIGN_CENTER,
            "LEVEL 1"
        );
        al_draw_scaled_bitmap(player_bitmap,
            0, 0, 32, 32,                      // Source bitmap dimensions
            SCREEN_W / 2 - 32, SCREEN_H / 2,   // Position on screen
            64, 64,                            // Scaled dimensions
            0                                  // Flags
        );
    }
}


static void destroy(void) {
    al_destroy_bitmap(player_bitmap);
    for (int i = 0; i < 10; i++) {
        al_destroy_bitmap(backgrounds[i]);
    }
}

Scene create_loading_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "loading";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
