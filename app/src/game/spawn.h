

#ifndef SPAWN_H
#define SPAWN_H

#include "defines.h"

bool spawn_system_initialize();
u16 spawn_character(Character2D _character);

spawn_system_state* get_spawn_system();
void clean_up_spawn_system();

u16 damage_spawn(u16 _id, u16 damage);

bool update_spawns(Vector2 player_position);
bool render_spawns();


#endif
