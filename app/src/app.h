
#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

bool app_initialize();

bool window_should_close();

bool app_update();
bool app_render();

#endif
