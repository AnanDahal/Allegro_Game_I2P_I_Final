#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "game_scene.h"
#include "map.h"
#include "player.h"
#include "loading_scene.h"


Player player; // Player
Player cocudos; // Cocudos



ALLEGRO_KEYBOARD_STATE keyboardState;

Map map;
static Button backButton;
static ALLEGRO_BITMAP* coin_bitmap;

static ALLEGRO_BITMAP* setting_bitmap;


static Button player_up_button;
static Button player_down_button;
static Button player_left_button;
static Button player_right_button;

static Button cocudos_up_button;
static Button cocudos_down_button;
static Button cocudos_left_button;
static Button cocudos_right_button;

int button_width = 400;
int button_height = 100;

static void init(void) {

    setting_bitmap = al_load_bitmap("Assets/setting_bg.jpg");

    if (!setting_bitmap) {
        game_abort("Failed to load setting bitmap");
    }

    button_sfx = al_load_sample("Assets/audio/button.mp3");

    // Initialize back button
    backButton = button_create(SCREEN_W / 2 - 200, 800, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    // Initialize keybinding buttons (set to the same size as backButton)
    

    // Player Controls buttons
    int player_y_offset = 150;
    player_up_button = button_create(SCREEN_W / 4 - button_width / 2, player_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    player_y_offset += 120;  // Added more space to separate the buttons

    player_down_button = button_create(SCREEN_W / 4 - button_width / 2, player_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    player_y_offset += 120;

    player_left_button = button_create(SCREEN_W / 4 - button_width / 2, player_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    player_y_offset += 120;

    player_right_button = button_create(SCREEN_W / 4 - button_width / 2, player_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    // Cocudos Controls buttons
    int cocudos_y_offset = 150;
    cocudos_up_button = button_create(SCREEN_W * 3 / 4 - button_width / 2, cocudos_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    cocudos_y_offset += 120;

    cocudos_down_button = button_create(SCREEN_W * 3 / 4 - button_width / 2, cocudos_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    cocudos_y_offset += 120;

    cocudos_left_button = button_create(SCREEN_W * 3 / 4 - button_width / 2, cocudos_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    cocudos_y_offset += 120;

    cocudos_right_button = button_create(SCREEN_W * 3 / 4 - button_width / 2, cocudos_y_offset, button_width, button_height, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    
    change_bgm("Assets/audio/setting_bgm.mp3");

}





typedef enum {
    NONE,
    PLAYER_UP,
    PLAYER_DOWN,
    PLAYER_LEFT,
    PLAYER_RIGHT,
    COCUDOS_UP,
    COCUDOS_DOWN,
    COCUDOS_LEFT,
    COCUDOS_RIGHT
} KeyBinding;

static KeyBinding waiting_for_key = NONE;

static void update(void) {
    // Update all buttons
    update_button(&backButton);
    update_button(&player_up_button);
    update_button(&player_down_button);
    update_button(&player_left_button);
    update_button(&player_right_button);
    update_button(&cocudos_up_button);
    update_button(&cocudos_down_button);
    update_button(&cocudos_left_button);
    update_button(&cocudos_right_button);

    // Handle back button
    if (mouseState.buttons && backButton.hovered == true) {
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (map_level == 2 || map_level == 3) {
            change_scene(create_transition_scene());
        }
        else {
            change_scene(create_menu_scene());
        }
    }

    // Handle key rebinding
    if (waiting_for_key != NONE) {
        al_get_keyboard_state(&keyboardState);
        for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
            if (al_key_down(&keyboardState, i)) {
                // Check if the new key is already assigned to another action and swap
                if (i == player_up) {
                    if (waiting_for_key != PLAYER_UP) player_up = i;
                }
                else if (i == player_down) {
                    if (waiting_for_key != PLAYER_DOWN) player_down = i;
                }
                else if (i == player_left) {
                    if (waiting_for_key != PLAYER_LEFT) player_left = i;
                }
                else if (i == player_right) {
                    if (waiting_for_key != PLAYER_RIGHT) player_right = i;
                }
                else if (i == cocudos_up) {
                    if (waiting_for_key != COCUDOS_UP) cocudos_up = i;
                }
                else if (i == cocudos_down) {
                    if (waiting_for_key != COCUDOS_DOWN) cocudos_down = i;
                }
                else if (i == cocudos_left) {
                    if (waiting_for_key != COCUDOS_LEFT) cocudos_left = i;
                }
                else if (i == cocudos_right) {
                    if (waiting_for_key != COCUDOS_RIGHT) cocudos_right = i;
                }

                // Swap the current key with the one being reassigned
                switch (waiting_for_key) {
                case PLAYER_UP:
                    if (i == player_down) player_down = player_up;
                    if (i == player_left) player_left = player_up;
                    if (i == player_right) player_right = player_up;
                    if (i == cocudos_up) cocudos_up = player_up;
                    if (i == cocudos_down) cocudos_down = player_up;
                    if (i == cocudos_left) cocudos_left = player_up;
                    if (i == cocudos_right) cocudos_right = player_up;
                    player_up = i;
                    break;
                case PLAYER_DOWN:
                    if (i == player_up) player_up = player_down;
                    if (i == player_left) player_left = player_down;
                    if (i == player_right) player_right = player_down;
                    if (i == cocudos_up) cocudos_up = player_down;
                    if (i == cocudos_down) cocudos_down = player_down;
                    if (i == cocudos_left) cocudos_left = player_down;
                    if (i == cocudos_right) cocudos_right = player_down;
                    player_down = i;
                    break;
                case PLAYER_LEFT:
                    if (i == player_up) player_up = player_left;
                    if (i == player_down) player_down = player_left;
                    if (i == player_right) player_right = player_left;
                    if (i == cocudos_up) cocudos_up = player_left;
                    if (i == cocudos_down) cocudos_down = player_left;
                    if (i == cocudos_left) cocudos_left = player_left;
                    if (i == cocudos_right) cocudos_right = player_left;
                    player_left = i;
                    break;
                case PLAYER_RIGHT:
                    if (i == player_up) player_up = player_right;
                    if (i == player_down) player_down = player_right;
                    if (i == player_left) player_left = player_right;
                    if (i == cocudos_up) cocudos_up = player_right;
                    if (i == cocudos_down) cocudos_down = player_right;
                    if (i == cocudos_left) cocudos_left = player_right;
                    if (i == cocudos_right) cocudos_right = player_right;
                    player_right = i;
                    break;
                case COCUDOS_UP:
                    if (i == player_up) player_up = cocudos_up;
                    if (i == player_down) player_down = cocudos_up;
                    if (i == player_left) player_left = cocudos_up;
                    if (i == player_right) player_right = cocudos_up;
                    if (i == cocudos_down) cocudos_down = cocudos_up;
                    if (i == cocudos_left) cocudos_left = cocudos_up;
                    if (i == cocudos_right) cocudos_right = cocudos_up;
                    cocudos_up = i;
                    break;
                case COCUDOS_DOWN:
                    if (i == player_up) player_up = cocudos_down;
                    if (i == player_down) player_down = cocudos_down;
                    if (i == player_left) player_left = cocudos_down;
                    if (i == player_right) player_right = cocudos_down;
                    if (i == cocudos_up) cocudos_up = cocudos_down;
                    if (i == cocudos_left) cocudos_left = cocudos_down;
                    if (i == cocudos_right) cocudos_right = cocudos_down;
                    cocudos_down = i;
                    break;
                case COCUDOS_LEFT:
                    if (i == player_up) player_up = cocudos_left;
                    if (i == player_down) player_down = cocudos_left;
                    if (i == player_left) player_left = cocudos_left;
                    if (i == player_right) player_right = cocudos_left;
                    if (i == cocudos_up) cocudos_up = cocudos_left;
                    if (i == cocudos_down) cocudos_down = cocudos_left;
                    if (i == cocudos_right) cocudos_right = cocudos_left;
                    cocudos_left = i;
                    break;
                case COCUDOS_RIGHT:
                    if (i == player_up) player_up = cocudos_right;
                    if (i == player_down) player_down = cocudos_right;
                    if (i == player_left) player_left = cocudos_right;
                    if (i == player_right) player_right = cocudos_right;
                    if (i == cocudos_up) cocudos_up = cocudos_right;
                    if (i == cocudos_down) cocudos_down = cocudos_right;
                    if (i == cocudos_left) cocudos_left = cocudos_right;
                    cocudos_right = i;
                    break;
                }

                waiting_for_key = NONE; // Reset state
                return;
            }
        }
    }

    // Handle button clicks for key rebinding
    if (mouseState.buttons) {
        if (player_up_button.hovered == true) waiting_for_key = PLAYER_UP;
        else if (player_down_button.hovered == true) waiting_for_key = PLAYER_DOWN;
        else if (player_left_button.hovered == true) waiting_for_key = PLAYER_LEFT;
        else if (player_right_button.hovered == true) waiting_for_key = PLAYER_RIGHT;
        else if (cocudos_up_button.hovered == true) waiting_for_key = COCUDOS_UP;
        else if (cocudos_down_button.hovered == true) waiting_for_key = COCUDOS_DOWN;
        else if (cocudos_left_button.hovered == true) waiting_for_key = COCUDOS_LEFT;
        else if (cocudos_right_button.hovered == true) waiting_for_key = COCUDOS_RIGHT;
    }
}




static void draw(void) {

    al_draw_scaled_bitmap(setting_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);


    // Draw back button
    draw_button(backButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(50, 50, 50), 
        SCREEN_W / 2,
        800 + 30 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        800 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    // Indicate waiting for key input
    if (waiting_for_key != NONE) {
        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 0, 0),
            SCREEN_W / 2,
            100,
            ALLEGRO_ALIGN_CENTER,
            "Press any key to rebind!"
        );
    }

    // Draw Player Controls section
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 4, 50,
        ALLEGRO_ALIGN_CENTER,
        "Player Controls"
    );

    // Draw player key buttons with shadow effect
    draw_button(player_up_button);
    int offsety = 130;
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        player_up_button.x + button_width / 2, offsety + button_height / 2 + player_up_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Up: %s", al_keycode_to_name(player_up)
    );
    al_draw_textf(
        P2_FONT,
        player_up_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        player_up_button.x + button_width / 2, offsety + button_height / 2 + player_up_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Up: %s", al_keycode_to_name(player_up)
    );

    offsety += 120;

    draw_button(player_down_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        player_down_button.x + button_width / 2, offsety + button_height / 2 + player_down_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Down: %s", al_keycode_to_name(player_down)
    );
    al_draw_textf(
        P2_FONT,
        player_down_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        player_down_button.x + button_width / 2, offsety + button_height / 2 + player_down_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Down: %s", al_keycode_to_name(player_down)
    );

    offsety += 120;

    draw_button(player_left_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        player_left_button.x + button_width / 2, offsety + button_height / 2 + player_left_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Left: %s", al_keycode_to_name(player_left)
    );
    al_draw_textf(
        P2_FONT,
        player_left_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        player_left_button.x + button_width / 2, offsety + button_height / 2 + player_left_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Left: %s", al_keycode_to_name(player_left)
    );

    offsety += 120;

    draw_button(player_right_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        player_right_button.x + button_width / 2, offsety + button_height / 2 + player_right_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Right: %s", al_keycode_to_name(player_right)
    );
    al_draw_textf(
        P2_FONT,
        player_right_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        player_right_button.x + button_width / 2, offsety + button_height / 2 + player_right_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Right: %s", al_keycode_to_name(player_right)
    );


    // Draw Cocudos Controls section
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        SCREEN_W * 3 / 4, 50 + 2,
        ALLEGRO_ALIGN_CENTER,
        "Cocudos Controls"
    );
    al_draw_textf(
        P2_FONT,
        al_map_rgb(255, 255, 255), // Main text color
        SCREEN_W * 3 / 4, 50,
        ALLEGRO_ALIGN_CENTER,
        "Cocudos Controls"
    );

    // Cocudos Controls - Adjust layout to match Player Controls
    int cocudos_offsety = 130;

    draw_button(cocudos_up_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        cocudos_up_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_up_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Up: %s", al_keycode_to_name(cocudos_up)
    );
    al_draw_textf(
        P2_FONT,
        cocudos_up_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        cocudos_up_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_up_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Up: %s", al_keycode_to_name(cocudos_up)
    );

    cocudos_offsety += 120;

    draw_button(cocudos_down_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        cocudos_down_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_down_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Down: %s", al_keycode_to_name(cocudos_down)
    );
    al_draw_textf(
        P2_FONT,
        cocudos_down_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        cocudos_down_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_down_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Down: %s", al_keycode_to_name(cocudos_down)
    );

    cocudos_offsety += 120;

    draw_button(cocudos_left_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        cocudos_left_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_left_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Left: %s", al_keycode_to_name(cocudos_left)
    );
    al_draw_textf(
        P2_FONT,
        cocudos_left_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        cocudos_left_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_left_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Left: %s", al_keycode_to_name(cocudos_left)
    );

    cocudos_offsety += 120;

    draw_button(cocudos_right_button);
    al_draw_textf(
        P2_FONT,
        al_map_rgb(50, 50, 50), // Shadow (dark gray)
        cocudos_right_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_right_button.hovered * 11 + 2,
        ALLEGRO_ALIGN_CENTER, "Right: %s", al_keycode_to_name(cocudos_right)
    );
    al_draw_textf(
        P2_FONT,
        cocudos_right_button.hovered ? al_map_rgb(0, 255, 0) : al_map_rgb(255, 255, 255), // Main text color
        cocudos_right_button.x + button_width / 2, cocudos_offsety + button_height / 2 + cocudos_right_button.hovered * 11,
        ALLEGRO_ALIGN_CENTER, "Right: %s", al_keycode_to_name(cocudos_right)
    );

}





static void destroy(void) {
    destroy_button(&backButton);

    destroy_button(&player_up_button);
    destroy_button(&player_down_button);
    destroy_button(&player_left_button);
    destroy_button(&player_right_button);

    destroy_button(&cocudos_up_button);
    destroy_button(&cocudos_down_button);
    destroy_button(&cocudos_left_button);
    destroy_button(&cocudos_right_button);
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

static ALLEGRO_BITMAP* shop_bitmap;

static void init_shop(void) {
    coin_bitmap = al_load_bitmap("Assets/coin_icon.png");

    buy_sfx = al_load_sample("Assets/audio/buying.mp3");
    button_sfx = al_load_sample("Assets/audio/button.mp3");

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

    shop_bitmap = al_load_bitmap("Assets/shop_bg.png");

    if (!shop_bitmap) {
        game_abort("Failed to load shop bitmap");
    }

    backButton = button_create(SCREEN_W / 2 - 200, 800, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");


    // Top row: Guns, Sniper, Machine Gun
    buy_guns_Button = button_create(250, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_sniper_Button = button_create(SCREEN_W / 2 - 138, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_machine_gun_Button = button_create(1100, 300, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    // Bottom row: Yellow, Orange, Fireball
    buy_yellow_bullet_Button = button_create(250, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_orange_bullet_Button = button_create(SCREEN_W / 2 - 138, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buy_fireball_bullet_Button = button_create(1100, 600, 275, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    change_bgm("Assets/audio/shopping_bgm.mp3");
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

    

    if (mouseState.buttons && backButton.hovered == true) {
        al_play_sample(button_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        if (map_level == 2 || map_level == 3) {
            change_scene(create_transition_scene());
        }
        else {
            change_scene(create_menu_scene());
        }
        
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
            al_play_sample(buy_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
            al_play_sample(buy_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
            al_play_sample(buy_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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
            al_play_sample(buy_sfx, SFX_VOLUME + 3, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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

    al_draw_scaled_bitmap(shop_bitmap,
        0, 0, 1640, 1000,
        0, 0, 1640, 1000, 0);


    // Define a buffer to hold the formatted string
    char coin_text[32]; // Ensure the buffer is large enough to hold the text

    // Format the integer into the buffer as a string
    snprintf(coin_text, sizeof(coin_text), "%d", coins_obtained);
    al_draw_scaled_bitmap(coin_bitmap, 0, 0, 16, 16, 0, 0, 100, 100, 0);
    al_draw_text(P2_FONT, al_map_rgb(255, 255, 255), 52, 80, ALLEGRO_ALIGN_CENTER, coin_text);

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
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 70 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "5 COINS");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 67 + buy_sniper_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "5 COINS");
    }

    // Draw Machine Gun Section
    al_draw_scaled_bitmap(machine_gun_bitmap, 0, 0, 32, 32, button_x3 + 37, 75, 200, 200, 0);
    draw_button(buy_machine_gun_Button);
    if (machine_gun_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if (!machine_gun_eq && machine_gun_bought) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 70 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "10 COINS");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 67 + buy_machine_gun_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "10 COINS");

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
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 70 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "5 COINS");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 67 + buy_orange_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "5 COINS");
    }

    // Fireball Bullet
    al_draw_scaled_bitmap(fireball_bullet_bitmap, 0, 0, 16, 16, bullet_x3 + 100, bullet_y - 175, 100, 100, 0);
    draw_button(buy_fireball_bullet_Button);
    if (fireball_eq) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIPPED");
    }
    else if (!fireball_eq && fireball_bought) {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "EQUIP");
    }
    else {
        al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 70 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "10 COINS");
        al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 67 + buy_fireball_bullet_Button.hovered * 11, ALLEGRO_ALIGN_CENTER, "10 COINS");
    }

    // Descriptions

    // guns
    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x1 + button_width / 2 - 25, row1_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 3");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x1 + button_width / 2 - 25, row1_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 3");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x1 + button_width / 2 - 25, row1_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 1");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x1 + button_width / 2 - 25, row1_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 1");

    // sniper
    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 1");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 1");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x2 + button_width / 2, row1_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x2 + button_width / 2, row1_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");

    // machine gun
    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 5");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "FIRING RATE: 5");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), button_x3 + button_width / 2 + 20, row1_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), button_x3 + button_width / 2 + 20, row1_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");

    // yellow bullet
    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 1");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 1");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 1");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x1 + button_width / 2 - 25, bullet_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 1");

    // orange bullet

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 3");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 3");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x2 + button_width / 2, bullet_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 3");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x2 + button_width / 2, bullet_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 3");

    // fireball bullet

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 125, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 5");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 122, ALLEGRO_ALIGN_CENTER, "KNOCKBACK: 5");

    al_draw_text(P3_FONT, al_map_rgb(66, 76, 110), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 165, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");
    al_draw_text(P3_FONT, al_map_rgb(225, 225, 225), bullet_x3 + button_width / 2 + 20, bullet_y + button_height - 162, ALLEGRO_ALIGN_CENTER, "DAMAGE: 5");

    // Draw Back Button
    draw_button(backButton);
    al_draw_text(P2_FONT, al_map_rgb(66, 76, 110), SCREEN_W / 2, 800 + 28 + backButton.hovered * 11, ALLEGRO_ALIGN_CENTER, "BACK");
    al_draw_text(P2_FONT, al_map_rgb(225, 225, 225), SCREEN_W / 2, 800 + 31 + backButton.hovered * 11, ALLEGRO_ALIGN_CENTER, "BACK");
}



static void destroy_shop(void) {
    destroy_button(&backButton);

    al_destroy_bitmap(shop_bitmap);

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