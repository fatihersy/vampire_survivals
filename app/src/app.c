#include "app.h"

#include "core/event.h"
#include "core/window.h"
#include "core/ftime.h"
#include "core/fmemory.h"
#include "core/lexer.h"

#include "defines.h"
#include "game/camera.h"
#include "game/resource.h"
#include "game/scenes/scene_manager.h"
#include "raylib.h"

bool application_on_event(u16 code, void* sender, void* listener_inst, event_context context);

bool app_runing = false;

bool app_initialize() {
    app_settings _settings;
    parse_app_settings_ini("config.ini", &_settings);

    // Essentials
    create_window("title");
    create_camera((Vector2) {SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2}, 0);

    // Subsystems
    memory_system_initialize();
    event_system_initialize();
    time_system_initialize();
    resource_system_initialize();
    
    // Game
    if (!scene_manager_initialize(get_screen_size(), get_screen_half_size())) {
        TraceLog(LOG_ERROR, "scene_manager() initialization failed");
        return false;
    }

    event_register(EVENT_CODE_APPLICATION_QUIT, 0, application_on_event);

    app_runing = true;

    return true;
}

bool window_should_close() {
    return app_runing;
}

bool app_update() {
    if (GetFPS() > TARGET_FPS) return true;

    update_scene_scene();

    update_time();

    return true;
}

bool app_render() {
    if (GetFPS() > TARGET_FPS) return true;

    BeginDrawing();
    ClearBackground(CLEAR_BACKGROUND_COLOR);
    BeginMode2D(*get_active_camera());

    render_scene_world();

    EndMode2D();

    render_scene_interface();

    EndDrawing();

    return true;
}

bool application_on_event(u16 code, void* sender, void* listener_inst, event_context context) {
    switch (code)
    {
    case EVENT_CODE_APPLICATION_QUIT:
        app_runing = false;
        return true;
    default:
        break;
    }

    return false;
}
