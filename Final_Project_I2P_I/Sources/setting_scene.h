#ifndef setting_scene_h
#define setting_scene_h

#include "utility.h"


extern bool yellow_eq;
extern bool guns_eq;

extern bool orange_eq;
extern bool sniper_eq;
extern bool orange_bought;
extern bool sniper_bought;

extern bool fireball_eq;
extern bool machine_gun_eq;
extern bool fireball_bought;
extern bool machine_gun_bought;

ALLEGRO_SAMPLE* buy_sfx;

Scene create_setting_scene(void);
Scene create_shop_scene(void);

#endif /* setting_scene_h */