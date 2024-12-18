#include "user_interface.h"

#include "core/event.h"
#include "core/fmemory.h"

#include "defines.h"
#include "raylib.h"

#define BTN_MENU_DIM_X 250
#define BTN_MENU_DIM_Y 50
#define BTN_MENU_DIM_X_DIV2 BTN_MENU_DIM_X / 2.f
#define BTN_MENU_DIM_Y_DIV2 BTN_MENU_DIM_Y / 2.f
#define BTN_SQUARE_DIM 70
#define BTN_SQUARE_DIM_DIV2 BTN_SQUARE_DIM / 2.f
#define BTN_SPACE_BTW(i) (BTN_MENU_DIM_Y + 15) * i

#define PAUSE_MENU_TOTAL_WIDTH 850
#define PAUSE_MENU_TOTAL_HEIGHT 480

static user_interface_system_state *state;

#define PSPRITESHEET_SYSTEM state
#include "game/spritesheet.h"

bool user_interface_on_event(u16 code, void *sender, void *listener_inst, event_context context);
void register_button(const char *_text, u16 _x, u16 _y, button_type _btn_type, texture_type _tex_type, Vector2 source_dim);

void user_interface_system_initialize() {
  if (state) return;

  state = (user_interface_system_state *)allocate_memory_linear(sizeof(user_interface_system_state), true);
  state->ui_font = LoadFont(rs_path("quantico_bold.ttf"));
  state->b_user_interface_system_initialized = true;


  // MAIN MENU
  register_button("Play", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(0),
                  BTN_TYPE_MAINMENU_BUTTON_PLAY, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Editor", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(1),
                  BTN_TYPE_MAINMENU_BUTTON_EDITOR, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Settings", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(2),
                  BTN_TYPE_MAINMENU_BUTTON_SETTINGS, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Extras", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(3),
                  BTN_TYPE_MAINMENU_BUTTON_EXTRAS, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Exit", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(4),
                  BTN_TYPE_MAINMENU_BUTTON_EXIT, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  // MAIN MENU


  // IN GAME -- PAUSE
  register_button("Resume", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(0),
                  BTN_TYPE_INGAME_PAUSEMENU_BUTTON_RESUME, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Settings", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(1),
                  BTN_TYPE_INGAME_PAUSEMENU_BUTTON_SETTINGS, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Main Menu", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(2),
                  BTN_TYPE_INGAME_PAUSEMENU_BUTTON_MAINMENU, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Exit", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(3),
                  BTN_TYPE_INGAME_PAUSEMENU_BUTTON_EXIT, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  // IN GAME -- PAUSE


  // EDITOR
  register_button("Save Map", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(0),
                  BTN_TYPE_EDITOR_BUTTON_SAVE_MAP, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Load Map", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(1),
                  BTN_TYPE_EDITOR_BUTTON_LOAD_MAP, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Settings", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(2),
                  BTN_TYPE_EDITOR_BUTTON_SETTINGS, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Main Menu", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(3),
                  BTN_TYPE_EDITOR_BUTTON_MAIN_MENU, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  register_button("Exit", SCREEN_WIDTH_DIV2, SCREEN_HEIGHT_DIV2 + BTN_SPACE_BTW(4),
                  BTN_TYPE_EDITOR_BUTTON_EXIT, BUTTON_TEXTURE, 
                  (Vector2){80, 16});
  // EDITOR


  event_register(EVENT_CODE_UI_SHOW_PAUSE_SCREEN, 0, user_interface_on_event);
  event_register(EVENT_CODE_UI_SHOW_UNPAUSE_SCREEN, 0, user_interface_on_event);
}

bool set_player_user_interface(player_state* player) {
  if (player->initialized) {
    state->p_player = player;
    return true;
  }

  return false;
}

void update_user_interface() {
  state->mouse_pos = GetMousePosition();
  state->offset = (Vector2) { SCREEN_OFFSET, SCREEN_OFFSET};
  update_sprite_renderqueue();

  for (int i = 0; i < BTN_TYPE_MAX; ++i) {
    if (state->buttons[i].btn_type == BTN_TYPE_UNDEFINED) {
      continue;
    }
    if (!state->buttons[i].show) { continue; }

    button btn = state->buttons[i];
    if (CheckCollisionPointRec(state->mouse_pos, btn.dest)) {

      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        btn.state = BTN_STATE_PRESSED;
        btn.source.x = btn.source.width;
        stop_sprite(state->buttons[btn.btn_type].reflection_render_index, true);
      } else {
        if (btn.state != BTN_STATE_HOVER) {
          btn.state = BTN_STATE_HOVER;
          btn.source.x = 0;
        }
      }
    } else {
      if (btn.state != BTN_STATE_UP) {
        reset_sprite(btn.reflection_render_index, true);
        btn.state = BTN_STATE_UP;
        btn.source.x = 0;
      }
    }

    state->buttons[i] = btn;
  }
}

void render_user_interface() {

}

void draw_to_background(texture_type _type) {
  DrawTexturePro(
  *get_texture_by_enum(_type),
  (Rectangle){
    .x = 0,
    .y = 0,
    .width = get_texture_by_enum(_type)->width,
    .height = get_texture_by_enum(_type)->height},
    (Rectangle){
    .x = 0,
    .y = 0,
    .width = GetScreenWidth(),
    .height = GetScreenHeight()},
    (Vector2){.x = 0, .y = 0}, 0,
    WHITE
  ); // Draws the background
}

bool gui_button(button_type _type) {
  if (_type == BTN_TYPE_UNDEFINED) {
    return false;
  }
  button _btn = state->buttons[_type];
  _btn.show = true;

  DrawTexturePro(*get_texture_by_enum(BUTTON_TEXTURE), _btn.source, _btn.dest,
                 (Vector2){0}, 0, WHITE
  );
  play_sprite_on_site(_btn.crt_render_index, _btn.dest);

  if (_btn.state == BTN_STATE_PRESSED) {
    DrawTextEx(state->ui_font, _btn.text,
               (Vector2){.x = _btn.text_pos.x, .y = _btn.text_pos.y + 3},
               state->ui_font.baseSize, _btn.text_spacing,
               MYYELLOW);
  } else {
    if (_btn.state == BTN_STATE_HOVER) {
      play_sprite_on_site(_btn.reflection_render_index, _btn.dest);
    }
    DrawTextEx(state->ui_font, _btn.text,
               (Vector2){.x = _btn.text_pos.x, .y = _btn.text_pos.y - 3},
               state->ui_font.baseSize, _btn.text_spacing,
               MYYELLOW);
  }
  state->buttons[_type] = _btn;

  return _btn.state == BTN_STATE_PRESSED;
}

void gui_healthbar(f32 percent) {
  const u16 iter = 10*percent;
  DrawTexturePro(
    *get_texture_by_enum(HEALTHBAR_TEXTURE),
    (Rectangle){.x = 0, .y = 0, .width = 72, .height = 12},
    (Rectangle){.x = 15, .y = 15, .width = 216, .height = 36},
    (Vector2){0, 0}, 0, WHITE
  );
  for (int i=0; i < iter; ++i) {
    DrawTexturePro(
    *get_texture_by_enum(HEALTH_PERC_TEXTURE),
    (Rectangle){.x = 0, .y = 0, .width = 5, .height = 7},
    (Rectangle){.x = 33+(15+3)*i, .y = 24, .width = 15, .height = 21},
    (Vector2){0, 0}, 0, WHITE
    );
  }
}

void register_button(const char *_text, u16 _x, u16 _y, button_type _btn_type, texture_type _tex_type, Vector2 source_dim) {
  if (_btn_type == BTN_TYPE_UNDEFINED || !state->b_user_interface_system_initialized)
    return;
  Vector2 text_measure =
      MeasureTextEx(state->ui_font, _text,
                    state->ui_font.baseSize, UI_FONT_SPACING);
  button btn = {
      .id = _btn_type,
      .btn_type = _btn_type,
      .text = _text,
      .show = false,
      .crt_render_index = register_sprite(BUTTON_CRT_SHEET, true, false, false),
      .reflection_render_index = register_sprite(BUTTON_REFLECTION_SHEET, false, true, false),
      .text_spacing = UI_FONT_SPACING,
      .tex_type = _tex_type,
      .text_pos.x = _x - text_measure.x / 2.f,
      .text_pos.y = _y - text_measure.y / 2.f,
      .state = BTN_STATE_UP,
      .dest = (Rectangle){.x = _x - BTN_MENU_DIM_X_DIV2,
                          .y = _y - BTN_MENU_DIM_Y_DIV2,
                          .width = BTN_MENU_DIM_X,
                          .height = BTN_MENU_DIM_Y},
      .source =
          (Rectangle){
              .x = 0, .y = 0, .width = source_dim.x, .height = source_dim.y},
  };

  state->buttons[_btn_type] = btn;
}

void clear_interface_state() {
  for (int i = 0; i < BTN_TYPE_MAX; ++i) {
    state->buttons[i].show = false;
  }

  state->b_show_pause_screen = false;
}

bool user_interface_on_event(u16 code, void *sender, void *listener_inst,
                             event_context context) {
  switch (code) {
  case EVENT_CODE_UI_SHOW_PAUSE_SCREEN: { // DON'T FIRE GAME_ENGINE PAUSE EVENTS HERE
    state->b_show_pause_screen = true;
    return true;
    break;
  }
  case EVENT_CODE_UI_SHOW_UNPAUSE_SCREEN: { // DON'T FIRE GAME_ENGINE PAUSE EVENTS HERE
    state->b_show_pause_screen = false;
    return true;
    break;
  }
  };

  return false;
}

#undef PSPRITESHEET_SYSTEM
