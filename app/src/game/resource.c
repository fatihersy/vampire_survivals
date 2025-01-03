#include "resource.h"

#include "core/fmemory.h"
#include "defines.h"
#include "raylib.h"

extern const unsigned char default_tile_png[146];

static resource_system_state *resource_system;

unsigned int load_texture(const char* _path, bool resize, Vector2 new_size, texture_type _type);
bool load_image(const char *_path, bool resize, Vector2 new_size, image_type type);
void load_spritesheet(const char* _path, spritesheet_type _type, u16 _fps, u16 _frame_width, u16 _frame_height, u16 _total_row, u16 _total_col);
void load_tilesheet(tilesheet_type _sheet_sheet_type, texture_type _sheet_tex_type, u16 _tile_count_x, u16 _tile_count_y, u16 _tile_size) ;

bool resource_system_initialize() {
  if (resource_system) return false;

  resource_system = (resource_system_state *)allocate_memory_linear(sizeof(resource_system_state), true);
  resource_system->texture_amouth = -1;

  //NOTE: _path = "%s%s", RESOURCE_PATH, _path
  load_texture("space_bg.png",         false, (Vector2){0,  0}, TEX_BACKGROUND);
  load_texture("fudesumi.png",         true,  (Vector2){64, 64},TEX_ENEMY_TEXTURE);
  load_texture("health_bar_edited.png",false, (Vector2){0,  0}, TEX_HEALTHBAR_TEXTURE);
  load_texture("health_perc.png",      false, (Vector2){0,  0}, TEX_HEALTH_PERC_TEXTURE);
  load_texture("map_tileset.png",      false, (Vector2){0,  0}, TEX_MAP_TILESET_TEXTURE);
  load_texture("panel.png",            false, (Vector2){0,  0}, TEX_PANEL);
  load_texture("panel_scaled.png",     false, (Vector2){0,  0}, TEX_PANEL_SCALED);
  load_spritesheet("level_up_sheet.png", LEVEL_UP_SHEET, 60, 150, 150, 8, 8);
  load_spritesheet("idle_left.png",      PLAYER_ANIMATION_IDLE_LEFT, 15, 86, 86, 1, 4);
  load_spritesheet("idle_right.png",     PLAYER_ANIMATION_IDLE_RIGHT, 15, 86, 86, 1, 4);
  load_spritesheet("move_left.png",      PLAYER_ANIMATION_MOVE_LEFT, 10, 86, 86, 1, 6);
  load_spritesheet("move_right.png",     PLAYER_ANIMATION_MOVE_RIGHT, 10, 86, 86, 1, 6);
  load_spritesheet("take_damage_left.png", PLAYER_ANIMATION_TAKE_DAMAGE_LEFT, 15, 86, 86, 1, 4);
  load_spritesheet("take_damage_right.png", PLAYER_ANIMATION_TAKE_DAMAGE_RIGHT, 15, 86, 86, 1, 4);
  load_spritesheet("wreck_left.png", PLAYER_ANIMATION_WRECK_LEFT, 15, 90, 110, 1, 4);
  load_spritesheet("wreck_right.png", PLAYER_ANIMATION_WRECK_RIGHT, 15, 90, 110, 1, 4);
  load_spritesheet("button_reflection.png", BUTTON_REFLECTION_SHEET, 30, 80, 16, 1, 9);
  load_spritesheet("button_crt.png", BUTTON_CRT_SHEET, 8, 78, 12, 1, 4);
  load_spritesheet("screen_crt_sheet.png", SCREEN_CRT_SHEET, 8, 1280, 720, 1, 4);
  load_spritesheet("slider_percent.png", SLIDER_PERCENT, 0, 21, 10, 1, 11);
  load_spritesheet("slider_option.png", SLIDER_OPTION, 0, 21, 10, 1, 2);
  load_spritesheet("slider_left_button.png", SLIDER_LEFT_BUTTON, 0, 10, 10, 1, 2);
  load_spritesheet("slider_right_button.png", SLIDER_RIGHT_BUTTON, 0, 10, 10, 1, 2);
  load_spritesheet("menu_button.png", MENU_BUTTON, 0, 80, 16, 1, 2);
  load_tilesheet(TILESHEET_TYPE_MAP, TEX_MAP_TILESET_TEXTURE, 16, 14, 16);

  return true;
}


Texture2D* get_texture_by_id(unsigned int id) {
  for (i16 i = 0; i <= resource_system->texture_amouth; ++i) {
    if (resource_system->textures[i].id == id)
      return &resource_system->textures[i];
  }

  return (Texture2D*){0};
}

Texture2D* get_texture_by_enum(texture_type type) {
  if (type >= TEXTURE_TYPE_MAX || type <= TEX_UNSPECIFIED){
    TraceLog(LOG_WARNING, "WARNING::resource::get_texture_by_enum()::Texture type out of bound");
    return (Texture2D*){0};
  }

  return &resource_system->textures[type];
}
Image* get_image_by_enum(image_type type) {

  if (type > MAX_IMAGE_SLOTS || type == IMG_UNSPECIFIED){
    TraceLog(LOG_WARNING, "WARNING::resource::get_image_by_enum()::Image type out of bound");
    return (Image*){0};
  }

  return &resource_system->images[type];
}

spritesheet get_spritesheet_by_enum(spritesheet_type type) {

  if (type == SPRITESHEET_UNSPECIFIED || type > MAX_SPRITESHEET_SLOTS){
    TraceLog(LOG_WARNING, "WARNING::resource::get_spritesheet_by_enum()::Spritesheet type out of bound");
    return (spritesheet){0};
  }

  return resource_system->sprites[type];
}

tilesheet* get_tilesheet_by_enum(tilesheet_type type) {
  if (type == TILESHEET_TYPE_UNSPECIFIED || type > MAX_TILESHEET_SLOTS){
    TraceLog(LOG_WARNING, "WARNING::resource::get_tilesheet_by_enum()::Tilesheet type out of bound");
    return (tilesheet*){0};
  }

  return &resource_system->tilesheets[type];
}

const char *rs_path(const char *_path) {
  return TextFormat("%s%s", RESOURCE_PATH, _path);
}

unsigned int load_texture(const char *_path, bool resize, Vector2 new_size, texture_type _type) {
  const char *path = rs_path(_path);
  if (!FileExists(path)) { TraceLog(
  LOG_ERROR,"ERROR::resource::load_texture():: Path:'%s' Cannot find", path);
    return INVALID_ID32;
  } 
  else if (_type >= TEXTURE_TYPE_MAX || _type <= TEX_UNSPECIFIED) { TraceLog(
  LOG_ERROR,
  "ERROR::resource::load_texture()::texture type out of bound");
    return INVALID_ID32;
  }

  Texture2D tex;
  if (resize) {
    Image img = LoadImage(path);
    ImageResize(&img, new_size.x, new_size.y);
    tex = LoadTextureFromImage(img);
  } else {
    tex = LoadTexture(path);
  }
  resource_system->texture_amouth++;

  resource_system->textures[_type] = tex;
  return resource_system->textures[_type].id;
}

bool load_image(const char *_path, bool resize, Vector2 new_size, image_type type) {
  const char *path = rs_path(_path);
  if (!FileExists(path) || type == IMG_UNSPECIFIED)
    return false;

  Image img;
  if (resize) {
    img = LoadImage(path);
    ImageResize(&img, new_size.x, new_size.y);
  } else {
    img = LoadImage(path);
  }

  resource_system->image_amouth++;

  resource_system->images[type] = img;
  return true;
}


void load_spritesheet(const char *_path, spritesheet_type _type, u16 _fps, u16 _frame_width, u16 _frame_height, u16 _total_row, u16 _total_col) {
  const char *path = rs_path(_path);
  if (!FileExists(path)) { TraceLog(
  LOG_ERROR,"ERROR::resource::load_spritesheet():: Path:'%s' Cannot find", path);
    return;
  } 
  else if (_type > MAX_SPRITESHEET_SLOTS) { TraceLog(LOG_ERROR,
  "ERROR::resource::load_spritesheet()::Unknown spritesheet slot");
    return;
  }

  spritesheet _sheet = {0};
  Texture2D tex = LoadTexture(path);
  resource_system->sprite_amouth++;

  _sheet.handle = tex;
  _sheet.type = _type;
  _sheet.is_started = false;
  _sheet.row_total = _total_row;
  _sheet.col_total = _total_col;
  _sheet.frame_total = _total_col * _total_row;
  _sheet.current_col = 0;
  _sheet.current_row = 0;
  _sheet.current_frame = 0;
  _sheet.attached_spawn = 0;
  _sheet.current_frame_rect = (Rectangle){
      .x = 0, .y = 0, .width = _frame_width, .height = _frame_height};
  _sheet.coord = (Rectangle){
      .x = 0, .y = 0, .width = _frame_width, .height = _frame_height};
  _sheet.fps = _fps;

  if (_type == SPRITESHEET_UNSPECIFIED) {
    resource_system->sprites[resource_system->sprite_amouth] = _sheet;
    TraceLog(
        LOG_WARNING,
        "resource::load_spritesheet()::WARNING! Spritesheet type was not set");
  }

  resource_system->sprites[_type] = _sheet;
}

void load_tilesheet(tilesheet_type _sheet_sheet_type, texture_type _sheet_tex_type, u16 _tile_count_x, u16 _tile_count_y, u16 _tile_size) {
  if (_sheet_sheet_type >= MAX_TILESHEET_SLOTS || _sheet_sheet_type <= 0) {
    TraceLog(LOG_ERROR,
             "ERROR::resource::load_tilesheet()::Sheet type out of bound");
    return;
  }
  else if (_sheet_tex_type >= TEXTURE_TYPE_MAX || _sheet_tex_type <= 0) {
    TraceLog(
        LOG_ERROR,
        "ERROR::resource::load_tilesheet()::texture type out of bound");
  }

  tilesheet _tilesheet = {0};
  resource_system->tilesheet_amouth++;

  _tilesheet.tex = get_texture_by_enum(_sheet_tex_type);
  _tilesheet.sheet_type = _sheet_sheet_type;
  _tilesheet.tex_type = _sheet_tex_type;
  _tilesheet.tile_count_x = _tile_count_x;
  _tilesheet.tile_count_y = _tile_count_y;
  _tilesheet.tile_count = _tilesheet.tile_count_x * _tilesheet.tile_count_y;
  _tilesheet.tile_size = _tile_size;

  for (u16 i = 0; i < _tilesheet.tile_count; ++i) {
    u8 x = i % _tilesheet.tile_count_x;
    u8 y = i / _tilesheet.tile_count_x;
    u8 x_symbol = TILEMAP_TILE_START_SYMBOL + x;
    u8 y_symbol = TILEMAP_TILE_START_SYMBOL + y;

    _tilesheet.tile_symbols[x][y] = (tile_symbol) {
      x_symbol,
      y_symbol,
      _sheet_sheet_type
    };
  }

  resource_system->tilesheets[_sheet_sheet_type] = _tilesheet;
}

