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
static ALLEGRO_BITMAP* loading_bitmap;


// Add a pointer array to hold the background bitmaps
static ALLEGRO_BITMAP* backgrounds[10];

static void init(void) {
    timer_countdown = 220;
    scene = 0;

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    // Load background and player bitmaps
    player_bitmap = al_load_bitmap("Assets/panda2.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    loading_bitmap = al_load_bitmap("Assets/loading_bg.jpg");
    if (!loading_bitmap) {
        game_abort("Failed to load loading bitmap");
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
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        scene++;
        click = true;
    }

    // If ENTER is pressed, set scene to 10 and start timer
    if ((scene == 10 || al_key_down(&key_state, ALLEGRO_KEY_SPACE)) && !starttimer) {
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
        al_draw_scaled_bitmap(loading_bitmap,
            0, 0, 1640, 1000,
            0, 0, 1640, 1000, 0);

        al_draw_text(
            TITLE_FONT,
            al_map_rgb(0, 0, 0),
            SCREEN_W / 7,
            SCREEN_H / 2 - 50,
            ALLEGRO_ALIGN_CENTER,
            "LOADING"
        );
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(199, 207, 221),
            SCREEN_W / 7 + 2,
            SCREEN_H / 2 - 50 + 3,
            ALLEGRO_ALIGN_CENTER,
            "LOADING"
        );

        al_draw_text(
            TITLE_FONT,
            al_map_rgb(0, 0, 0),
            6*SCREEN_W / 7,
            SCREEN_H / 2 - 50,
            ALLEGRO_ALIGN_CENTER,
            "LEVEL 1"
        );
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(199, 207, 221),
            6 * SCREEN_W / 7 + 2,
            SCREEN_H / 2 - 50 + 3,
            ALLEGRO_ALIGN_CENTER,
            "LEVEL 1"
        );

        al_draw_text(
            TITLE_FONT,
            al_map_rgb(0, 0, 0),
            6 * SCREEN_W / 7,
            SCREEN_H / 2 - 50 + 65,
            ALLEGRO_ALIGN_CENTER,
            "\"C\""
        );
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(199, 207, 221),
            6 * SCREEN_W / 7 + 2,
            SCREEN_H / 2 - 50 + 65 + 3,
            ALLEGRO_ALIGN_CENTER,
            "\"C\""
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

static Button shopButton;
static Button settingButton;
static Button exitButton;
static ALLEGRO_BITMAP* transition_bitmap;


static void init_transition(void) {

    shopButton = button_create(SCREEN_W / 2 - 200, 550, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    settingButton = button_create(SCREEN_W / 2 - 200, 680, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    exitButton = button_create(SCREEN_W / 2 - 200, 800, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    transition_bitmap = al_load_bitmap("Assets/transition_bg.jpg");

    if (!transition_bitmap) {
        game_abort("Failed to load transition bitmap");
    }

    change_bgm("Assets/audio/rickroll.mp3");

}



static void update_transition(void) {

    al_draw_scaled_bitmap(transition_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);


    update_button(&shopButton);
    update_button(&settingButton);
    update_button(&exitButton);

    if (keyState[ALLEGRO_KEY_ENTER]) {
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_loading_scene());
    }

    if (shopButton.hovered && (mouseState.buttons & 1)) { // Shop button pressed
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_shop_scene());
    }

    if (settingButton.hovered && (mouseState.buttons & 1)) { // Setting button pressed
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_setting_scene());
    }

    if (exitButton.hovered && (mouseState.buttons & 1)) { // Exit button pressed
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        exit(0);
    }
}

static void draw_transition(void) {
    al_draw_text(
        P1_FONT,
        al_map_rgb(0, 0, 0),
        SCREEN_W / 2,
        100,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO CONTINUE NEXT MAP"
    );

    al_draw_text(
        P1_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        98,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO CONTINUE NEXT MAP"
    );

    // Shop button
    draw_button(shopButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        550 + 28 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        550 + 31 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );

    // Setting button
    draw_button(settingButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        680 + 28 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        680 + 31 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );

    // Exit button
    draw_button(exitButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        800 + 28 + exitButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "EXIT"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        800 + 31 + exitButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "EXIT"
    );
}


static void destroy_transition(void) {
    destroy_button(&shopButton);
    destroy_button(&settingButton);
    destroy_button(&exitButton);
    al_destroy_bitmap(transition_bitmap);
}

Scene create_transition_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "transition";
    scene.init = &init_transition;
    scene.draw = &draw_transition;
    scene.update = &update_transition;
    scene.destroy = &destroy_transition;

    return scene;
}

