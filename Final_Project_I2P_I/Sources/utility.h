#ifndef utility_h
#define utility_h
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>


#define LOG_ENABLE

//#define DRAW_HITBOX


extern const int FPS; // Frames per second
extern const int SCREEN_W; // Screen Width
extern const int SCREEN_H; // Screen Height
extern const int RESERVE_SAMPLES; // Maximum Audio played at a time
extern const char* GAME_TITLE; // Window title name
extern const int TILE_SIZE; // TILE SIZE Displayed in the game

extern float SFX_VOLUME; // Sound Effects Volume
extern float BGM_VOLUME; // Background Music Volume

ALLEGRO_SAMPLE* button_sfx;

extern ALLEGRO_FONT* TITLE_FONT; // 64px
extern ALLEGRO_FONT* P1_FONT; // 48px
extern ALLEGRO_FONT* P2_FONT; // 36px
extern ALLEGRO_FONT* P3_FONT; // 24px

extern bool add_health;
extern bool reduce_health_player;
extern bool reduce_health_cocudos;
extern bool reduce_health_cocudos_stepped_off;
extern bool reduce_health_player_stepped_off;




typedef void(*function_void)(void);
typedef void(*function_key)(int keycode);
typedef void(*function_mouse)(int btn, int x, int y, int dz);



// Direction enumeration
typedef enum _DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT
} DIRECTION;

// Integer Point (x, y)
typedef struct _Point{
     int x;
     int y;
 } Point;

// Float Point (x, y)
typedef struct _PointFloat{
    float x;
    float y;
} PointFloat;

// Rectangle (x, y, w=width, h=height)
typedef struct _RecArea {
    float x, y, w, h;
} RecArea;

// log message type
typedef enum logtype_{
    Log,
    Error,
    Warning
} logtype;

typedef struct Scene_{
    char* name;
    function_void init; // initialize
    function_void update; // update scene
    function_void draw; // draw scene
    function_void destroy; // destroy scene
} Scene;

// Change Background Music
void change_bgm(char* audio_path);

// Utility Initialization
void init_Util(void);

// Destroy Utility
void destroy_init(void);

// Game Log Message: To print something
void game_log(const char * msg, ...);

// Game Error Message: To Print something and then abort the game
void game_abort(const char * msg, ...);

#endif
