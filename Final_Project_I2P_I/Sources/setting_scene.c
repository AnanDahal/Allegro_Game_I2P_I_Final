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

static Button buy_yellow_bullet_Button;
static Button buy_orange_bullet_Button;
static Button buy_fireball_bullet_Button;

static Button buy_guns_Button;
static Button buy_sniper_Button;
static Button buy_machine_gun_Button;

static ALLEGRO_BITMAP* yellow_bullet_bitmap;
static ALLEGRO_BITMAP* guns_bitmap;

static ALLEGRO_BITMAP* orange_bullet_bitmap;
static ALLEGRO_BITMAP* sniper_bitmap;

static ALLEGRO_BITMAP* fireball_bullet_bitmap;
static ALLEGRO_BITMAP* machine_gun_bitmap;

bool yellow_eq = true;
bool guns_eq = true;

bool orange_eq = false;
bool sniper_eq = false;
bool orange_bought = false;
bool sniper_bought = false;

bool fireball_eq = false;
bool machine_gun_eq = false;
bool fireball_bought = false;
bool machine_gun_bought = false;

const int orange_bullet_price = 5;
const int sniper_price = 5;

const int fireball_bullet_price = 10;
const int machine_gun_price = 10;


static void init_shop(void) {
    coin_bitmap = al_load_bitmap("Assets/coin_icon.png");

    if (!coin_bitmap) {
        game_abort("Failed to load coin bitmap");
    }

    // shop 

    // 1
    yellow_bullet_bitmap = al_load_bitmap("Assets/yellow_bullet.png");

    if (!yellow_bullet_bitmap) {
        game_abort("Failed to load yellow bullet bitmap");
    }

    guns_bitmap = al_load_bitmap("Assets/guns.png");

    if (!guns_bitmap) {
        game_abort("Failed to load guns bitmap");
    }

    // 2 
    orange_bullet_bitmap = al_load_bitmap("Assets/orange_bullet.png");

    if (!orange_bullet_bitmap) {
        game_abort("Failed to load orange bullet bitmap");
    }

    sniper_bitmap = al_load_bitmap("Assets/sniper.png");

    if (!sniper_bitmap) {
        game_abort("Failed to load sniper bitmap");
    }

    // 3

    fireball_bullet_bitmap = al_load_bitmap("Assets/fireball.png");

    if (!fireball_bullet_bitmap) {
        game_abort("Failed to load fireball bullet bitmap");
    }

    machine_gun_bitmap = al_load_bitmap("Assets/machine_gun.png");

    if (!machine_gun_bitmap) {
        game_abort("Failed to load machine gun bitmap");
    }
    

    // end pic shop

    backButton = button_create(SCREEN_W / 2 - 200, 800, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");


    // Top row: Guns, Sniper, Machine Gun
    buy_guns_Button = button_create(250, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_sniper_Button = button_create(SCREEN_W / 2 - 138, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_machine_gun_Button = button_create(1100, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    // Bottom row: Yellow, Orange, Fireball
    buy_yellow_bullet_Button = button_create(250, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_orange_bullet_Button = button_create(SCREEN_W / 2 - 138, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_fireball_bullet_Button = button_create(1100, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

}
bool test = false;
static void update_shop(void) {

    update_button(&backButton);
    update_button(&buy_guns_Button);
    update_button(&buy_sniper_Button);
    update_button(&buy_machine_gun_Button);
    update_button(&buy_yellow_bullet_Button);
    update_button(&buy_orange_bullet_Button);
    update_button(&buy_fireball_bullet_Button);

    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    al_clear_to_color(al_map_rgb(128, 128, 128)); 
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);
    al_draw_scaled_bitmap(coin_bitmap, 0, 0, 16, 16, 0, 0, 100, 100, 0);
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), 52, 80, ALLEGRO_ALIGN_CENTER, coin_text);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }
    

    if (!mouseState.buttons) {
        test = false;
    }

    // guns 
    if (mouseState.buttons && buy_guns_Button.hovered == true && !test) {
        if (!guns_eq) {
            guns_eq = true;
            sniper_eq = false;
            machine_gun_eq = false;
        }
    }

    // yellow bullet
    if (mouseState.buttons && buy_yellow_bullet_Button.hovered == true && !test) {
        if (!yellow_eq) {
            yellow_eq = true;
            orange_eq = false;
            fireball_eq = false;
        }
    }

    // sniper
    if (mouseState.buttons && buy_sniper_Button.hovered == true && !test) {
        if (!sniper_bought && coins_obtained >= sniper_price) {
            coins_obtained -= sniper_price;
            test = true;
            sniper_bought = true;
            sniper_eq = true;
            guns_eq = false;
            machine_gun_eq = false;
        }
        else if (!sniper_eq && sniper_bought) {
            sniper_eq = true;
            guns_eq = false;
            machine_gun_eq = false;
        }
    }

    // orange bullet
    if (mouseState.buttons && buy_orange_bullet_Button.hovered == true && !test) {
        if (!orange_bought && coins_obtained >= orange_bullet_price) {
            coins_obtained -= orange_bullet_price;
            test = true;
            orange_bought = true;
            orange_eq = true;
            yellow_eq = false;
            fireball_eq = false;
        }
        else if (!orange_eq && orange_bought) {
            orange_eq = true;
            yellow_eq = false;
            fireball_eq = false;
        }
    }
    

    // machine gun
    if (mouseState.buttons && buy_machine_gun_Button.hovered == true && !test) {
        if (!machine_gun_bought && coins_obtained >= machine_gun_price) {
            coins_obtained -= machine_gun_price;
            test = true;
            machine_gun_bought = true;
            machine_gun_eq = true;
            guns_eq = false;
            sniper_eq = false;
        }
        else if (!machine_gun_eq && machine_gun_bought) {
            machine_gun_eq = true;
            guns_eq = false;
            sniper_eq = false;
        }
    }

    // fireball bullet
    if (mouseState.buttons && buy_fireball_bullet_Button.hovered == true && !test) {
        if (!fireball_bought && coins_obtained >= fireball_bullet_price) {
            coins_obtained -= fireball_bullet_price;
            test = true;
            fireball_bought = true;
            fireball_eq = true;
            yellow_eq = false;
            orange_eq = false;
        }
        else if (!fireball_eq && fireball_bought) {
            fireball_eq = true;
            yellow_eq = false;
            orange_eq = false;
        }
    }
}

static void draw_shop(void) {
    // Constants for layout
    int screen_width = 1640;
    int button_width = 275;
    int button_height = 100;
    int button_spacing = 375; // Horizontal spacing
    int row1_y = 300;         // Y-position for row 1
    int row2_y = 600;         // Y-position for row 2

    // Button X-coordinates
    int button_x1 = 250 + (screen_width - (3 * button_width) - (2 * button_spacing)) / 2; // Left button
    int button_x2 = button_x1 + button_width + button_spacing - 250;                      // Middle button
    int button_x3 = button_x2 + button_width + button_spacing - 250;                      // Right button

    // Draw Guns Section
    al_draw_scaled_bitmap(guns_bitmap, 0, 0, 32, 32, button_x1, 75, 200, 200, 0);
    draw_button(buy_guns_Button);
    if (guns_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x1 + button_width / 2 - 25, row1_y + button_height - 70 + buy_guns_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x1 + button_width / 2 - 25, row1_y + button_height - 67 + buy_guns_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x1 + button_width / 2 - 25, row1_y + button_height - 70 + buy_guns_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x1 + button_width / 2 - 25, row1_y + button_height - 67 + buy_guns_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }

    // Draw Sniper Section
    al_draw_scaled_bitmap(sniper_bitmap, 0, 0, 32, 32, button_x2 + 37, 75, 200, 200, 0);
    draw_button(buy_sniper_Button);
    if (sniper_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 70 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 67 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if (!sniper_eq && sniper_bought) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 70 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 67 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 70 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 67 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
    }

    // Draw Machine Gun Section
    al_draw_scaled_bitmap(machine_gun_bitmap, 0, 0, 32, 32, button_x3 + 37, 75, 200, 200, 0);
    draw_button(buy_machine_gun_Button);
    if (machine_gun_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if(!machine_gun_eq && machine_gun_bought){
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");

    }

    // Draw Bullet Section
    int bullet_y = row2_y; // Y-coordinate for bullets
    int bullet_x1 = button_x1; // Left button position for bullets
    int bullet_x2 = button_x2; // Middle button position for bullets
    int bullet_x3 = button_x3; // Right button position for bullets

    // Yellow Bullet
    al_draw_scaled_bitmap(yellow_bullet_bitmap, 0, 0, 32, 32, bullet_x1 + 60, bullet_y - 175, 100, 100, 0);
    draw_button(buy_yellow_bullet_Button);
    if (yellow_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 70 + buy_yellow_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 67 + buy_yellow_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 70 + buy_yellow_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 67 + buy_yellow_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }

    // Orange Bullet
    al_draw_scaled_bitmap(orange_bullet_bitmap, 0, 0, 32, 32, bullet_x2 + 87, bullet_y - 175, 100, 100, 0);
    draw_button(buy_orange_bullet_Button);
    if (orange_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 70 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 67 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if (!orange_eq && orange_bought) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 70 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 67 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 70 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 67 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
    }

    // Fireball Bullet
    al_draw_scaled_bitmap(fireball_bullet_bitmap, 0, 0, 16, 16, bullet_x3 + 100, bullet_y - 175, 100, 100, 0);
    draw_button(buy_fireball_bullet_Button);
    if (fireball_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if (!fireball_eq && fireball_bought) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "BUY");
    }


    // Draw Back Button
    draw_button(backButton);
    al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), SCREEN_W / 2, 800 + 28 + backButton.hovered * 11, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), SCREEN_W / 2, 800 + 31 + backButton.hovered * 11, ALLEGRO_ALIGN_CENTER, "BACK");
}



static void destroy_shop(void) {
    destroy_button(&backButton);

    destroy_button(&buy_guns_Button);
    destroy_button(&buy_sniper_Button);
    destroy_button(&buy_machine_gun_Button);
    destroy_button(&buy_yellow_bullet_Button);
    destroy_button(&buy_orange_bullet_Button);
    destroy_button(&buy_fireball_bullet_Button);

    al_destroy_bitmap(guns_bitmap);
    al_destroy_bitmap(sniper_bitmap);
    al_destroy_bitmap(machine_gun_bitmap);
    al_destroy_bitmap(yellow_bullet_bitmap);
    al_destroy_bitmap(orange_bullet_bitmap);
    al_destroy_bitmap(fireball_bullet_bitmap);
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
