#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button shopButton;
static Button settingButton;
static Button exitButton;
static ALLEGRO_BITMAP* nthu_bitmap;
static ALLEGRO_BITMAP* iskcon_bitmap;
static ALLEGRO_BITMAP* menu_bitmap;

static void init(void) {
    shopButton = button_create(SCREEN_W / 2 - 200, 550, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    settingButton = button_create(SCREEN_W / 2 - 200, 680, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    exitButton = button_create(SCREEN_W / 2 - 200, 800, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    nthu_bitmap = al_load_bitmap("Assets/nthu.png");

    if (!nthu_bitmap) {
        game_abort("Failed to load nthu bitmap");
    }

    iskcon_bitmap = al_load_bitmap("Assets/iskcon.png");

    if (!iskcon_bitmap) {
        game_abort("Failed to load iskcon bitmap");
    }

    menu_bitmap = al_load_bitmap("Assets/menu_bg.png");

    if (!menu_bitmap) {
        game_abort("Failed to load menu bitmap");
    }

     

    change_bgm("Assets/audio/rickroll.mp3");
}

static void update(void) {
    update_button(&shopButton);
    update_button(&settingButton);
    update_button(&exitButton);

    if (keyState[ALLEGRO_KEY_ENTER]) {
        al_play_sample(button_sfx, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_loading_scene());
    }

    if (shopButton.hovered && (mouseState.buttons & 1)) { // Shop button pressed
        al_play_sample(button_sfx, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_shop_scene());
    }

    if (settingButton.hovered && (mouseState.buttons & 1)) { // Setting button pressed
        al_play_sample(button_sfx, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_setting_scene());
    }

    if (exitButton.hovered && (mouseState.buttons & 1)) { // Exit button pressed
        al_play_sample(button_sfx, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        exit(0);
    }
}

static void draw(void) {
    // Title Text
    al_draw_scaled_bitmap(menu_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);
    al_draw_scaled_bitmap(nthu_bitmap,
        0, 0, 32, 32,
        SCREEN_W / 4, SCREEN_H / 2, 64, 64, 0);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        225,
        ALLEGRO_ALIGN_CENTER,
        "NIO's HELL"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        220,
        ALLEGRO_ALIGN_CENTER,
        "NIO's HELL"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        500,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(199, 207, 221),
        SCREEN_W / 2,
        498,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
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

    al_draw_scaled_bitmap(nthu_bitmap,
        0, 0, 317, 318,
        100, 100, 300, 300, 0);

    al_draw_scaled_bitmap(iskcon_bitmap,
        0, 0, 520, 480,
        1300, 100, 300, 300, 0);
}

static void destroy(void) {
    destroy_button(&shopButton);
    destroy_button(&settingButton);
    destroy_button(&exitButton);
    al_destroy_bitmap(nthu_bitmap);
    al_destroy_bitmap(iskcon_bitmap);
    al_destroy_bitmap(menu_bitmap);
}

Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
