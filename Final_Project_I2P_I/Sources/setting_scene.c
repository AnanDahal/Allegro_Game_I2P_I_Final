#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "game_scene.h"
#include "map.h"

Map map;
static Button backButton;
static Button shopButton;
static ALLEGRO_BITMAP* coin_bitmap;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    shopButton = button_create(SCREEN_W / 2 - 200, 500, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
}

static void update(void) {

    update_button(&backButton);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }

    update_button(&shopButton);

    if (mouseState.buttons && shopButton.hovered == true) {
        change_scene(create_shop_scene());
    }

}

static void draw(void) {
    // button
    draw_button(backButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    // button
    draw_button(shopButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        500 + 28 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        500 + 31 + shopButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SHOP"
    );


}

static void destroy(void) {
    destroy_button(&backButton);
    destroy_button(&shopButton);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}




/*
update shop
    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);
    al_draw_scaled_bitmap(coin_bitmap, 0, 0, 16, 16, 0, 0, 100, 100, 0);
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), 52, 80, ALLEGRO_ALIGN_CENTER, coin_text);

*/

static Button buy1Button;
static Button buy2Button;
static ALLEGRO_BITMAP* sniper_bitmap;
static ALLEGRO_BITMAP* cocudos_bitmap;
const int sniper_price = 3;
const int character_price = 10;



static void init_shop(void) {
    coin_bitmap = al_load_bitmap("Assets/coin_icon.png");

    if (!coin_bitmap) {
        game_abort("Failed to load coin bitmap");
    }

    cocudos_bitmap = al_load_bitmap("Assets/character.png");

    if (!cocudos_bitmap) {
        game_abort("Failed to load cocudos bitmap");
    }

    sniper_bitmap = al_load_bitmap("Assets/sniper.png");

    if (!sniper_bitmap) {
        game_abort("Failed to load sniper bitmap");
    }

    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy1Button = button_create(80, 200, 140, 50, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy2Button = button_create(380, 200, 140, 50, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
}
bool test = false;
static void update_shop(void) {

    update_button(&backButton);
    update_button(&buy1Button);
    update_button(&buy2Button);

    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    al_clear_to_color(al_map_rgb(128, 128, 128)); // Victory green
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);
    al_draw_scaled_bitmap(coin_bitmap, 0, 0, 16, 16, 0, 0, 100, 100, 0);
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), 52, 80, ALLEGRO_ALIGN_CENTER, coin_text);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
    

    if (!mouseState.buttons) {
        test = false;
    }

    if (mouseState.buttons && buy1Button.hovered == true && !test) {
        coins_obtained -= sniper_price;
        // make case where you buy change it to bought (already hovered)
        test = true;
    }

    if (mouseState.buttons && buy2Button.hovered == true && !test) {
        coins_obtained -= character_price;
        // make case where you buy change it to bought (already hovered)
        test = true;
    }
    

}

static void draw_shop(void) {
    al_draw_scaled_bitmap(sniper_bitmap,
        0, 0, 32, 32,
        100, 100, 100, 100, 0);

    draw_button(buy1Button);

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        150,
        177 + 28 + buy1Button.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        150,
        177 + 31 + buy1Button.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    
    al_draw_scaled_bitmap(cocudos_bitmap,
        0, 0, 48, 48,
        400, 100, 100, 100, 0);

    draw_button(buy2Button);

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        450,
        177 + 28 + buy2Button.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        450,
        177 + 31 + buy2Button.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BUY"
    );

    draw_button(backButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
}


static void destroy_shop(void) {
    destroy_button(&backButton);
    destroy_button(&buy1Button);
    al_destroy_bitmap(coin_bitmap);
    al_destroy_bitmap(sniper_bitmap);
    al_destroy_bitmap(cocudos_bitmap);
}

Scene create_shop_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "shop";
    scene.init = &init_shop;
    scene.draw = &draw_shop;
    scene.update = &update_shop;
    scene.destroy = &destroy_shop;

    return scene;
}
