#include "game.h"

#include "core/window.h"
#include "core/camera.h"
#include "core/fmath.h"
#include "game/player.h"
#include "game/spawn.h"
#include "game/resource.h"
#include "game/collision.h"

const int gridsize = 33;
const int map_size = gridsize * 100;

u16 radius = 0x250;  // 592

bool game_initialize() {
    create_window("title");

    player_system_initialize();
    spawn_system_initialize();
    resource_system_initialize();
    collision_system_initialize();

    set_player_position(get_screen_size().x / 2, get_screen_size().y / 2);

    player_state _player = *get_player_state();

    add_collision((Rectangle){
                      .x = _player.position.x - _player.player_texture.width / 2,
                      .y = _player.position.y - _player.player_texture.height / 2,
                      .width = _player.player_texture.width,
                      .height = _player.player_texture.height,
                  },
                  PLAYER);

    unsigned int texId = load_texture("D:\\Workspace\\CLang\\Resources\\fudesumi.png", true, (Vector2){32, 32});
    Texture2D tex = get_texture_by_id(texId);

    for (size_t i = 0; i < 360; i += 10) {
        Vector2 position = get_a_point_of_a_circle(get_active_camera().offset, 500, i);

        spawn_character((Character2D){
            .texId = texId,
            .initialized = true,
            .position = position
        },
        ENEMY);
    }

    return true;
}

bool game_update() {
    // Update
    //----------------------------------------------------------------------------------
    update_camera(get_player_state()->position);

    update_player();
    update_spawns();
    update_collision();

    // radius -= 1;

    return true;
}

bool game_render() {
    begin_draw();

    // Background
    for (i16 i = -map_size + 13; i < map_size; i += gridsize) {  // X Axis
        DrawLine(i, -map_size, i, i + (map_size), (Color){21, 17, 71, 255});
    }
    for (i16 i = -map_size - 3; i < map_size; i += gridsize) {  // Y Axis
        DrawLine(-map_size, i, i + (map_size), i, (Color){21, 17, 71, 255});
    }

    render_player();
    render_spawns();
    render_collision();

    end_draw();
    return true;
}

void game_on_resize() {
}

// Create more bunnies
/*         for (int i = 0; i < 100; i++) {
            if (bunniesCount < MAX_BUNNIES) {
                bunnies[bunniesCount].position = GetMousePosition();
                bunnies[bunniesCount].speed.x = (float)GetRandomValue(-250, 250) / 60.0f;
                bunnies[bunniesCount].speed.y = (float)GetRandomValue(-250, 250) / 60.0f;
                bunnies[bunniesCount].color = (Color){GetRandomValue(50, 240),
                                                      GetRandomValue(80, 240),
                                                      GetRandomValue(100, 240), 255};
                bunniesCount++;
            }
        }

            // Update bunnies
    for (int i = 0; i < bunniesCount; i++) {
        bunnies[i].position.x += bunnies[i].speed.x;
        bunnies[i].position.y += bunnies[i].speed.y;

        if (((bunnies[i].position.x + texBunny.width / 2) > GetScreenWidth()) ||
            ((bunnies[i].position.x + texBunny.width / 2) < 0)) bunnies[i].speed.x *= -1;
        if (((bunnies[i].position.y + texBunny.height / 2) > GetScreenHeight()) ||
            ((bunnies[i].position.y + texBunny.height / 2 - 40) < 0)) bunnies[i].speed.y *= -1;
    }


        //DrawPixel(screenWidth/2 - 25 + (50 * cos(i * 3.1415f / 180.f)), screenHeight/2 - 25 + (50 * sin(i * 3.1415f / 180.f)), WHITE);

        float x = (float)(radius * Math.Cos(angleInDegrees * Math.PI / 180F)) + origin.X;
        float y = (float)(radius * Math.Sin(angleInDegrees * Math.PI / 180F)) + origin.Y;

            DrawTriangleLines(
        (Vector2){center_of_screen.x, center_of_screen.y-25},
        (Vector2){center_of_screen.x-25, center_of_screen.y+25},
        (Vector2){center_of_screen.x+25, center_of_screen.y+25},
        (Color){255, 255, 255, 255}
    );

        for (i16 i = 0; i < MAX_SPAWN_COUNT; i += 5) {
        DrawTexture(
            spawns[i].tex,
            screenWidth / 2 - texEnemy.width / 2 + (radius * cos(i * 3.1415f / 180.f)),
            screenHeight / 2 - texEnemy.height / 2 + (radius * sin(i * 3.1415f / 180.f)),
            WHITE);
    }

        */
