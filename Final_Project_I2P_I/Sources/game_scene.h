#ifndef game_scene_h
#define game_scene_h

#include "utility.h"

int coins_obtained;
int map_coin;
int map_level;

Scene create_lose_scene(void);
Scene create_game_scene(void);
Scene create_win_scene(void);


#endif /* game_scene_h */