#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button settingButton;
static ALLEGRO_BITMAP* nthu_bitmap;
static ALLEGRO_BITMAP* iskcon_bitmap;
static ALLEGRO_BITMAP* menu_bitmap;

static void init(void) {
    settingButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

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
    update_button(&settingButton);
    if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_loading_scene());
    }

    /*
        [TODO HACKATHON 4-3] 
        
        Change scene to setting scene when the button is pressed
    */

    // Start 4-3  CHECKAGAIN

    if (settingButton.hovered && (mouseState.buttons & 1)) { // Check if hovered and left mouse button is pressed.
        change_scene(create_setting_scene());
    }

    // End 4 - 3
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

    // button
    draw_button(settingButton);
    // button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        600 + 28 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        600 + 31 + settingButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SETTING"
    );
    al_draw_scaled_bitmap(nthu_bitmap,
        0, 0, 317, 318,
        100, 100, 300, 300, 0);

    al_draw_scaled_bitmap(iskcon_bitmap,
        0, 0, 520, 480,
        1300, 100, 300, 300, 0);
}

static void destroy(void) {
    destroy_button(&settingButton);
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
