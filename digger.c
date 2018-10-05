//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

#include <stdint.h>
#include <c64.h>
#include <cbm.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <peekpoke.h>
#include <string.h>
#include "digger.h"
#include "charset.h"
#include "monster.h"
#include "gold.h"
#include "mapcoll.h"
#include "bullet.h"
#include "non_gameplay.h"


////////////////////////////////////////////////////////////////////////////////
// Definitions: global.
////////////////////////////////////////////////////////////////////////////////
uint8_t NUM_SPAWNS = 0;
clock_t global_clock;
uint8_t digger_x, digger_y;
int8_t digger_x_offset, digger_y_offset;
uint8_t spawn_x, spawn_y;
uint8_t num_monsters_alive = 0;
uint8_t step;
uint32_t total_score;
uint8_t num_lives;
uint8_t current_level;
uint8_t num_emeralds_left;
uint8_t sid_index;
uint8_t bonus_mode_state;
uint8_t num_killed_monsters;
clock_t last_bonus_clock;
uint8_t sprite_1, sprite_2;
bool reset_sprite_after_first_key;


// Maze map of the current level. Each level we copy here the corresponding map
// from mapcoll.h
uint8_t map[10][15];


////////////////////////////////////////////////////////////////////////////////
// Definitions: trainer.
////////////////////////////////////////////////////////////////////////////////
bool unlimited_lives = false;
bool invincibility = false;
bool shoot_without_delay = false;
bool can_skip_levels = false;
uint8_t override_level_start = 0;



#if __CC65__ < 0x2F0
uint8_t __fastcall__ (*cbm_k_getin)(void) = 0xFFE4;
uint8_t __fastcall__ (*cbm_k_scnkey)(void) = 0xFF9F;
#endif

void setup_sprites() {
  POKE(SPRITES_MULTICOLOR_REGISTER, 0xFF);  // All sprites are multicolor
  POKE(MULTICOLOR_1_ADDRESS, COLOR_RED);
  POKE(MULTICOLOR_2_ADDRESS, COLOR_YELLOW);
  POKE(SPRITE1_COLOR_ADDR, COLOR_GREEN);
  POKE(SPRITE2_COLOR_ADDR, COLOR_YELLOW);
  POKE(SPRITE3_COLOR_ADDR, COLOR_WHITE);
  POKE(SPRITE4_COLOR_ADDR, COLOR_GREEN);
  POKE(SPRITE5_COLOR_ADDR, COLOR_GREEN);
  POKE(SPRITE6_COLOR_ADDR, COLOR_GREEN);
  POKE(SPRITE7_COLOR_ADDR, COLOR_GREEN);
  POKE(SPRITE8_COLOR_ADDR, COLOR_GREEN);
}

void clear_screen() {
  uint16_t i = 0;
  for (; i < 1000; i++) {
    POKE(SCREEN + i, CHAR_BLANK);
    POKE(COLOR + i, 0x08); // Multicolor bit is on, rest is black.
  }
}

////////////////////////////////////////////////////////////////////////////////
// Map draw functions.
////////////////////////////////////////////////////////////////////////////////

// Draws a maze tile. X and Y are in field coord (0-9, 0-14).
void draw_tile(uint8_t x, uint8_t y) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  POKE(SCREEN + offset, LEVEL_ART[current_level %
                                  NUM_LEVEL_ART][CHAR_MAZE_IDX_1]);
  POKE(COLOR + offset, LEVEL_ART_PEN_COLOR[current_level % NUM_LEVEL_ART]);
  POKE(SCREEN + (++offset),
       LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_MAZE_IDX_2]);
  POKE(COLOR + offset, LEVEL_ART_PEN_COLOR[current_level % NUM_LEVEL_ART]);

  POKE(SCREEN + offset2, LEVEL_ART[current_level %
                                   NUM_LEVEL_ART][CHAR_MAZE_IDX_3]);
  POKE(COLOR + offset2, LEVEL_ART_PEN_COLOR[current_level % NUM_LEVEL_ART]);
  POKE(SCREEN + (++offset2),
       LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_MAZE_IDX_4]);
  POKE(COLOR + offset2, LEVEL_ART_PEN_COLOR[current_level % NUM_LEVEL_ART]);
}

// Draws emerald tile. X and Y are in field coord (0-9, 0-14).
void draw_emerald(uint8_t x, uint8_t y) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  POKE(SCREEN + offset, LEVEL_ART[current_level %
                                  NUM_LEVEL_ART][CHAR_EMERALD_IDX_1]);
  POKE(COLOR + offset, 0x09); // 1001, where 1 is multicolor and 001 is white.
  // This is always so because of blick.
  POKE(SCREEN + (++offset),
       LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_EMERALD_IDX_2]);
  POKE(COLOR + offset, 0x09);

  POKE(SCREEN + offset2, LEVEL_ART[current_level %
                                   NUM_LEVEL_ART][CHAR_EMERALD_IDX_3]);
  POKE(COLOR + offset2, 0x09);
  POKE(SCREEN + (++offset2),
       LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_EMERALD_IDX_4]);
  POKE(COLOR + offset2, 0x09);
}

// Draws emerald tile. X and Y are in field coord (0-9, 0-14).
void draw_bonus(uint8_t x, uint8_t y) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  POKE(SCREEN + offset, CHAR_BONUS_1);
  POKE(COLOR + offset, 0x09);
  POKE(SCREEN + (++offset), CHAR_BONUS_2);
  POKE(COLOR + offset, 0x09);
  POKE(SCREEN + offset2, CHAR_BONUS_3);
  POKE(COLOR + offset2, 0x09);
  POKE(SCREEN + (++offset2), CHAR_BONUS_4);
  POKE(COLOR + offset2, 0x09);
}

void draw_gold(uint8_t x, uint8_t y, bool is_wobbled) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  if (is_wobbled) {
    POKE(SCREEN + offset, LEVEL_ART[current_level %
                                  NUM_LEVEL_ART][CHAR_GOLD_WBL_IDX_1]);
    POKE(COLOR + offset, 0x0F); // 1111, where 1 is multicolor and 111 is yellow.
    // This is always so because gold bag is yellow.
    POKE(SCREEN + (++offset),
         LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_GOLD_WBL_IDX_2]);
    POKE(COLOR + offset, 0x0F);
    POKE(SCREEN + offset2, LEVEL_ART[current_level %
                                     NUM_LEVEL_ART][CHAR_GOLD_WBL_IDX_3]);
    POKE(COLOR + offset2, 0x0F);
    POKE(SCREEN + (++offset2),
         LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_GOLD_WBL_IDX_4]);
    POKE(COLOR + offset2, 0x0F);
  } else {
    POKE(SCREEN + offset, LEVEL_ART[current_level %
                                    NUM_LEVEL_ART][CHAR_GOLD_IDX_1]);
    POKE(COLOR + offset, 0x0F); // 1111, where 1 is multicolor and 111 is yellow.
    // This is always so because gold bag is yellow.
    POKE(SCREEN + (++offset),
         LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_GOLD_IDX_2]);
    POKE(COLOR + offset, 0x0F);
    POKE(SCREEN + offset2, LEVEL_ART[current_level %
                                     NUM_LEVEL_ART][CHAR_GOLD_IDX_3]);
    POKE(COLOR + offset2, 0x0F);
    POKE(SCREEN + (++offset2),
         LEVEL_ART[current_level % NUM_LEVEL_ART][CHAR_GOLD_IDX_4]);
    POKE(COLOR + offset2, 0x0F);
  }
}

void draw_gold_on_blank(uint8_t x, uint8_t y, bool is_wobbled) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  if (is_wobbled) {
    POKE(SCREEN + offset, CHAR_WBL_GOLD_BLK_1);
    POKE(COLOR + offset, 0x0F); // 1111, where 1 is multicolor and 111 is yellow.
    POKE(SCREEN + (++offset), CHAR_WBL_GOLD_BLK_2);
    POKE(COLOR + offset, 0x0F);

    POKE(SCREEN + offset2, CHAR_WBL_GOLD_BLK_3);
    POKE(COLOR + offset2, 0x0F);
    POKE(SCREEN + (++offset2), CHAR_WBL_GOLD_BLK_4);
    POKE(COLOR + offset2, 0x0F);
  } else {
    POKE(SCREEN + offset, CHAR_GOLD_ON_BLANK_1);
    POKE(COLOR + offset, 0x0F); // 1111, where 1 is multicolor and 111 is yellow.
    POKE(SCREEN + (++offset), CHAR_GOLD_ON_BLANK_2);
    POKE(COLOR + offset, 0x0F);

    POKE(SCREEN + offset2, CHAR_GOLD_ON_BLANK_3);
    POKE(COLOR + offset2, 0x0F);
    POKE(SCREEN + (++offset2), CHAR_GOLD_ON_BLANK_4);
    POKE(COLOR + offset2, 0x0F);
  }
}

void draw_brokengold(uint8_t x, uint8_t y) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  POKE(SCREEN + offset, CHAR_BLANK);
  POKE(COLOR + offset, 0x0F); // 1111, where 1 is multicolor and 111 is yellow.
  POKE(SCREEN + (++offset), CHAR_BLANK);
  POKE(COLOR + offset, 0x0F);

  POKE(SCREEN + offset2, CHAR_BROKEN_GOLD_1);
  POKE(COLOR + offset2, 0x0F);
  POKE(SCREEN + (++offset2), CHAR_BROKEN_GOLD_2);
  POKE(COLOR + offset2, 0x0F);
}

// Draws the map stored in map[][].
void draw_map() {
  uint8_t x, y, map_element;
  num_emeralds_left = 0;
  for (y = 0; y < 10; y++) {
    for (x = 0; x < 15; x++) {
      map_element = map[y][x];
      switch (map_element) {
        case MAP_EMPTY:
          clear_tile(x, y);
          break;
        case MAP_MAZE:
          draw_tile(x, y);
          break;
        case MAP_EMERALD:
          draw_emerald(x, y);
          num_emeralds_left++;
          break;
        case MAP_GOLD:
          draw_gold(x, y, false);
          init_gold(x, y);
          break;
        case MAP_SPAWN:
          spawn_x = x;
          spawn_y = y;
          break;
      }
    }
  }

  num_gold_exist = num_gold;

  // Draw map border
  y = 2;
  x = 4;
  POKE(SCREEN + y * 40 + x, CHAR_MAPBORDER_TL);
  POKE(COLOR + y * 40 + x, 0x0F);
  y = 2;
  x = 35;
  POKE(SCREEN + y * 40 + x, CHAR_MAPBORDER_TR);
  POKE(COLOR + y * 40 + x, 0x0F);
  y = 23;
  x = 4;
  POKE(SCREEN + y * 40 + x, CHAR_MAPBORDER_BL);
  POKE(COLOR + y * 40 + x, 0x0F);
  y = 23;
  x = 35;
  POKE(SCREEN + y * 40 + x, CHAR_MAPBORDER_BR);
  POKE(COLOR + y * 40 + x, 0x0F);
  for (x = 5; x < 35; x++) {
    POKE(SCREEN + 80 + x, CHAR_MAPBORDER_T);
    POKE(COLOR + 80 + x, 0x0F);
    POKE(SCREEN + 920 + x, CHAR_MAPBORDER_B);
    POKE(COLOR + 920 + x, 0x0F);
  }
  for (y = 3; y < 23; y++) {
    POKE(SCREEN + y * 40 + 4, CHAR_MAPBORDER_L);
    POKE(COLOR + y * 40 + 4, 0x0F);
    POKE(SCREEN + y * 40 + 35, CHAR_MAPBORDER_R);
    POKE(COLOR + y * 40 + 35, 0x0F);
  }
}

// Converts given tile coords into screen memory offset.
uint16_t field_xy_to_offset(uint8_t x, uint8_t y) {
  // 120 is 40 * 3 (1 row for empty, 2 for score)
  // 80 is 40*2
  // 5 is free chars to the left of field.
  return 125 + 80 * y + 2 * x;
}

// Called when Digger eats the given tile.
void eat_tile(uint8_t x, uint8_t y) {
  if (map[y][x] == MAP_SPAWN && bonus_mode_state == BONUS_CAN_HAVE) {
    flash_screen();
    play_sfx_eat();
    play_sfx_eat();
    play_sfx_eat();
    total_score += 1000;
    bonus_mode_state = BONUS_ON;
    num_killed_monsters = 0;
    POKE(BG_COLOR_REGISTER, 0x00);
    POKE(BORDER_COLOR_REGISTER, 0x02);
    sid_index = MUSIC_BONUS;
    play_popcorn();
    last_bonus_clock = clock();
    statusbar_redraw_score();
  }
  if (map[y][x] == MAP_EMERALD) {
    play_sfx_eat();
    total_score += 25;
    statusbar_redraw_score();
  } else if (map[y][x] == MAP_BROKENGOLD) {
    play_sfx_eat_gold();
    total_score += 500;
    statusbar_redraw_score();
  }
  clear_tile(x, y);
}

// Clears the given tile.
void clear_tile(uint8_t x, uint8_t y) {
  uint16_t offset = field_xy_to_offset(x, y);
  uint16_t offset2 = offset + 40;
  POKE(SCREEN + offset, CHAR_BLANK);
  POKE(COLOR + offset, 0x08);
  POKE(SCREEN + (++offset), CHAR_BLANK);
  POKE(COLOR + offset, 0x08);

  POKE(SCREEN + offset2, CHAR_BLANK);
  POKE(COLOR + offset2, 0x08);
  POKE(SCREEN + (++offset2), CHAR_BLANK);
  POKE(COLOR + offset2, 0x08);

  if (map[y][x] == MAP_EMERALD) {
    num_emeralds_left--;
    if (num_emeralds_left == 0) {
      level_complete();      
      return;
    }
  }
  if (x != spawn_x || y != spawn_y) {
    map[y][x] = MAP_EMPTY;
  } else {
    map[y][x] = MAP_SPAWN;
  }
}

void level_complete(void) {
  clock_t clk;  
  current_level = ++current_level;
  clk = clock();
  sid_index = MUSIC_SUCCESS;
  play_popcorn();
  flash_screen();
  while (clock() < clk + SUCCESS_TONE_SEC * CLOCKS_PER_SEC) {};
  sid_index = MUSIC_POPCORN;
  play_popcorn();
  prepare_game(false, true);
}

// Sets digger position according to digger_(x|y)[_offset].
// Returns true if tile (i.e. digger_x / digger_y) changed place
// as a place result.
bool place_digger() {
  uint8_t y_screen;
  uint16_t x_screen;
  bool res = false;

  if (digger_x_offset > 15) {
    digger_x += 1;
    digger_x_offset = 0;
    res = true;
  } else if (digger_x_offset < -15) {
    digger_x -= 1;
    digger_x_offset = 0;
    res = true;
  }
  if (digger_y_offset > 15) {
    digger_y += 1;
    digger_y_offset = 0;
    res = true;
  } else if (digger_y_offset < -15) {
    digger_y -= 1;
    digger_y_offset = 0;
    res = true;
  }

  y_screen = digger_y * 16 + 71 + digger_y_offset;
  x_screen = digger_x * 16 + 60 + digger_x_offset;
  if (x_screen <= 255) {
    POKE(0xD000, (uint8_t)x_screen);
    POKE(0xD010, PEEK(0xD010) & ~0x01);
  } else {
    POKE(0xD000, (uint8_t)(x_screen & 0xFF));
    POKE(0xD010, PEEK(0xD010) | 0x01);
  }
  POKE(0xD001, y_screen);
  return res;
}

// Forces digger to turn only in tiles and not "between" them.
void adjust_digger_pos(uint8_t last_action, uint8_t action) {
  if (last_action == KEY_RIGHT && (action == KEY_UP || action == KEY_DOWN)) {
    if (digger_x_offset >= 5) {
      digger_x++;
    }
    digger_x_offset = 0;
  } else if (last_action == KEY_LEFT && (action == KEY_UP
                                         || action == KEY_DOWN)) {
    if (digger_x_offset <= -5) {
      digger_x--;
    }
    digger_x_offset = 0;
  } else if (last_action == KEY_UP && (action == KEY_LEFT
                                       || action == KEY_RIGHT)) {
    if (digger_y_offset <= -10) {
      digger_y--;
    }
    digger_y_offset = 0;
  } else if (last_action == KEY_DOWN && (action == KEY_LEFT
                                         || action == KEY_RIGHT)) {
    if (digger_y_offset >= 10) {
      digger_y++;
    }
    digger_y_offset = 0;
  }
}

// Moves the digger.
// Return: 0 if couldn't move, 1 if move was successful, 2 if move was
// successful AND digger shifted to another tile in direction of the move.
uint8_t move_digger(uint8_t direction) {
  uint8_t can_move = 0;
  int8_t real_step = (direction == KEY_UP || direction == KEY_LEFT) ? -step : step;
  int8_t x_offset = digger_x_offset + real_step;
  int8_t y_offset = digger_y_offset + real_step;  

  switch (direction) {
    case KEY_UP:
      if (digger_y == 0 || (digger_y == 1 && y_offset < -15)) {
        break;
      }
      if ((y_offset < -15 && map[digger_y-2][digger_x] == MAP_GOLD) ||
          (digger_y_offset == 0 && map[digger_y-1][digger_x] == MAP_GOLD)) {
        break;
      }
      can_move = 1;
      digger_y_offset = y_offset;
      break;
    case KEY_DOWN:
      if ((digger_y == 9 && y_offset >= 0) || (digger_y == 8 && y_offset > 15)) {
        break;
      }
      if ((y_offset > 15 && map[digger_y+2][digger_x] == MAP_GOLD) ||
          (digger_y_offset == 0 && map[digger_y+1][digger_x] == MAP_GOLD)) {
        break;
      }
      can_move = 1;
      digger_y_offset = y_offset;
      break;
    case KEY_LEFT:
      if (digger_x == 0 || (digger_x == 1 && x_offset < -15)) {
        break;
      }
      if (digger_x > 2) {
        // Can't push 2 gold to the left.
        if ((x_offset < -15 && map[digger_y][digger_x-2] == MAP_GOLD &&
             map[digger_y][digger_x-3] == MAP_GOLD) ||
            (digger_x_offset == 0 && map[digger_y][digger_x-1] == MAP_GOLD &&
             map[digger_y][digger_x-2] == MAP_GOLD)) {
          break;
        }
      } else {
        if ((digger_x == 2 && x_offset < -15
             && map[digger_y][digger_x-2] == MAP_GOLD) ||
            (digger_x == 1 && digger_x_offset == 0
             && map[digger_y][digger_x-1] == MAP_GOLD)) {
          break;
        }
      }
      can_move = 1;
      digger_x_offset = x_offset;
      break;
    case KEY_RIGHT:
      if (digger_x == 14 || (digger_x == 13 && x_offset > 15)) {
        break;
      }
      if (digger_x < 12) {
        if ((x_offset > 15 && map[digger_y][digger_x+2] == MAP_GOLD &&
             map[digger_y][digger_x+3] == MAP_GOLD) ||
            (digger_x_offset == 0 && map[digger_y][digger_x+1] == MAP_GOLD &&
             map[digger_y][digger_x+2] == MAP_GOLD)) {
          break;
        }
      } else {
        if ((digger_x == 12 && x_offset > 15
             && map[digger_y][digger_x+2] == MAP_GOLD) ||
            (digger_x == 13 && digger_x_offset == 0
             && map[digger_y][digger_x+1] == MAP_GOLD)) {
          break;
        }
      }
      can_move = 1;
      digger_x_offset = x_offset;
      break;
  }
  if (can_move) {
    can_move += place_digger();
  }  
  return can_move;
}

// Reads the current char.
uint8_t poll_keyboard() {
  return PEEK(197);
}

uint8_t poll_joystik() {
  uint8_t mask = PEEK(JOYSTIK_REGISTER);
  if (!(mask & JOYSTIK_SHOOT)) {
    return KEY_SHOOT;
  } else if (!(mask & JOYSTIK_UP)) {
    return KEY_UP;
  } else if (!(mask & JOYSTIK_DOWN)) {
    return KEY_DOWN;
  } else if (!(mask & JOYSTIK_LEFT)) {
    return KEY_LEFT;
  } else if (!(mask & JOYSTIK_RIGHT)) {
    return KEY_RIGHT;
  }
  return 0xFF;  // Just an undefined value.
}

uint8_t get_digger_sprite_num(uint8_t direction, uint8_t sprite_idx) {
  if (can_shoot) {
    if (sprite_idx == 1) {
      switch (direction) {
        case KEY_UP:
          return DIG_U_1_NUM;
        case KEY_DOWN:
          return DIG_D_1_NUM;
        case KEY_LEFT:
          return DIG_L_1_NUM;
        case KEY_RIGHT:
          return DIG_R_1_NUM;
      }
    } else {
      switch (direction) {
        case KEY_UP:
          return DIG_U_2_NUM;
        case KEY_DOWN:
          return DIG_D_2_NUM;
        case KEY_LEFT:
          return DIG_L_2_NUM;
        case KEY_RIGHT:
          return DIG_R_2_NUM;
      }
    }
  } else {
    if (sprite_idx == 1) {
      switch (direction) {
        case KEY_UP:
          return DIG_UE_1_NUM;
        case KEY_DOWN:
          return DIG_DE_1_NUM;
        case KEY_LEFT:
          return DIG_LE_1_NUM;
        case KEY_RIGHT:
          return DIG_RE_1_NUM;
      }
    } else {
      switch (direction) {
        case KEY_UP:
          return DIG_UE_2_NUM;
        case KEY_DOWN:
          return DIG_DE_2_NUM;
        case KEY_LEFT:
          return DIG_LE_2_NUM;
        case KEY_RIGHT:
          return DIG_RE_2_NUM;
      }
    }
  }
}

void kill_digger_and_restart(void) {
  clock_t clk;
  uint8_t i = 0, cycle_counter = 0, last_color, joystik_mask = 0;
  char you_scored[18];
  char c;

  if (invincibility) {
    return;
  }

  sid_reset();
  clk = clock();
  POKE(SPRITE_DIGGER_ADDR, DIG_DEAD_NUM);
  sid_index = MUSIC_DEATH;
  play_popcorn();
  while (clock() < clk + DEATH_DELAY_SEC * CLOCKS_PER_SEC) {};
  sid_index = MUSIC_POPCORN;
  play_popcorn();

  if (!unlimited_lives) {
    num_lives--;
  }
  statusbar_redraw_lives();

  clear_kbd_buffer();

  if (num_lives == 0) {
    unload_wobble_gold_chars();
    POKE(0xD015, 0x00); // Hide all sprites.
    sprintf(you_scored, "YOU SCORED %06lu", total_score);
    draw_string_at(0, 6,  0x0A, "                    ");
    draw_string_at(0, 8,  0x0A, "      GAME OVER     ");
    draw_string_at(0, 10, 0x0A, "                    ");
    draw_string_at(3, 12, 0x0A, you_scored);
    draw_string_at(0, 14, 0x0A, "                    ");
    draw_string_at(0, 16, 0x0A, "                    ");
    draw_string_at(0, 18, 0x0A, "                    ");

    while (1) {
      cycle_counter++;
      if (cycle_counter % CLOCKS_PER_SEC == 0) {
        last_color = (last_color == COLOR_WHITE ? COLOR_YELLOW : COLOR_WHITE);
        draw_1x1_string_at(4, 17, last_color, "PRESS SPACE OR FIRE TO PLAY AGAIN");
      }
      c = cbm_k_getin();
      joystik_mask = PEEK(JOYSTIK_REGISTER);
      c = poll_keyboard();
      if (c == KEY_SHOOT) { break; }
      if (!(joystik_mask & JOYSTIK_SHOOT)) { break; }
    }

    current_level = 0;
    clear_screen();
    prepare_game(true, false);
  } else {
    prepare_game(false, false);
  }
}

void flash_screen() {
  uint16_t i = 0;
  uint8_t color = 0;
  while(i < 0x1FF) {
    if (++i % CLOCKS_PER_SEC == 0) {
      POKE(BG_COLOR_REGISTER, i);
      POKE(BORDER_COLOR_REGISTER, i);
    }
  }
  POKE(BG_COLOR_REGISTER, 0x00);
  POKE(BORDER_COLOR_REGISTER, 0x00);
}

void prepare_game(bool reset_all, bool advance_to_next_level) {
  // This we need to do anyway.
  uint8_t map_index;

  POKE(0xD015, 0);

  if (current_level == 0xFF && advance_to_next_level) {
    ending();
    current_level = 0;
    total_score = 0;
    num_lives = 3;
  }

  // Control difficulty
  switch (current_level) {
    case 0: NUM_SPAWNS = 2; WALK_STEP32_MONSTER = 10; break;
    case 1: NUM_SPAWNS = 2; WALK_STEP32_MONSTER = 10; break;
    case 2: NUM_SPAWNS = 3; WALK_STEP32_MONSTER = 8; break;
    case 3: NUM_SPAWNS = 3; WALK_STEP32_MONSTER = 8; break;
    case 4: NUM_SPAWNS = 3; WALK_STEP32_MONSTER = 6; break;
    default: NUM_SPAWNS = 4; WALK_STEP32_MONSTER = 4; break;
  }

  step = 1;
  bullet_step = 3;
  gold_falling_step = 3;
  can_shoot = true;
  is_bullet = false;
  bullet_direction = 0;
  digger_x = 7;
  digger_y = 9;
  spawn_x = 14;
  spawn_y = 0;
  digger_x_offset = 0;
  digger_y_offset = 0;
  bonus_mode_state = BONUS_OFF;
  POKE(BG_COLOR_REGISTER, 0x00);
  POKE(BORDER_COLOR_REGISTER, 0x00);
  last_bonus_clock = 0;  
  num_killed_monsters = 0;
  global_clock = clock();
  init_monsters();
  num_monsters_alive = 0;

  // 0-1-2-3-4-5-6, then 7-8-9-10-11-12, 3-...-12, etc.
  if (current_level < NUM_LEVELS) {
    map_index = current_level;
  } else {
    map_index = 3 + ((current_level + 1) % 10);
  }
  if (current_level == 0xFE) {
    map_index = 13;
  }

  if (reset_all) {
    // This we do only if game is restarted from level 0.
    num_lives = 3;
    total_score = 0;
    num_gold = 0;    
  }
  if (advance_to_next_level) {
    // This we do only if a digger goes to next level.
    // Give extra life every 3 levels.
    if (current_level % 3 == 0 && num_lives < 3) {
      num_lives++;
    }
    num_gold = 0;
  }
  if (reset_all || advance_to_next_level) {
    // This we do only we don't stay on the same level.
    memcpy(map, (uint8_t*)(ALL_MAPS+10*15*map_index), 10 * 15);
    clear_screen();
    draw_map();
    init_monsters();
  }
  clear_tile(spawn_x, spawn_y);
  clear_tile(digger_x, digger_y);
  POKE(0xD015, 1); //Enable sprite 0
  statusbar_redraw_score();
  statusbar_redraw_lives();
  statusbar_redraw_level();
  place_digger();
  sprite_1 = get_digger_sprite_num(KEY_RIGHT, 1);
  sprite_2 = get_digger_sprite_num(KEY_RIGHT, 2);
  reset_sprite_after_first_key = true;
  turnoff_gold_animation(); //stop_gold_animation = true;
  load_wobble_gold_chars();
}

int main() {
  uint8_t c, last_c = KEY_RIGHT;
  uint8_t sprite_num = 0;
  uint16_t digger_sprite_addr = SPRITE1_ADDR;
  uint8_t cycle_counter = 0;
  clock_t monster_spawn_clock = 0;
  clock_t last_monster_spawn_clock = 0;
  clock_t last_shoot_clock = 0;

  clock_t last_shoot_clock_diff = 0;
  clock_t last_bonus_clock_diff = 0;
  clock_t last_monster_spawn_clock_diff = 0;

  uint8_t move_res = 0;

  clear_screen();

  POKE(BORDER_COLOR_REGISTER, 0);
  POKE(BG_COLOR_REGISTER, 0);

  // Disable RUN/STOP and RESTORE keys.
  POKE(808, 225);
  disable_nmi();
  
  setup_sprites();
 

  POKE(0xDD00, (PEEK(0xDD00) & 0xFC) | 1); // Use VIC bank 2, $8000-$bfff
  POKE(0xD016, PEEK(0xD016) | 0x10);       // Switch to multicolor mode.

  // 0x04 = 0000 0010. "0000" means Screen RAM starts from VIC bank beginning
  // (in our case 0x8000). "0010" means "%001" - character RAM starts from $0800 offset
  // or $8800.
  POKE(0xD018, 0x02);

  POKE(0xD021, 0x00); // Background is black.
  POKE(0xD022, 0x05); // Background 1 is 5.
  POKE(0xD023, 0x02); // Background 2 is 2.

  sid_index = MUSIC_INTRO;  // Intro
  play_popcorn();

  backup_alphabet();

  // Intro will also wait for key.  
  intro();
  
  sid_index = MUSIC_POPCORN;
  play_popcorn();


  current_level = override_level_start;

  prepare_game(true, true);
  sprite_1 = get_digger_sprite_num(KEY_RIGHT, 1);
  sprite_2 = get_digger_sprite_num(KEY_RIGHT, 2);
  sprite_num = get_digger_sprite_num(KEY_RIGHT, 1);

  while(1) {
    if (cycle_counter % CLOCK_STEP == 0) {
      global_clock = clock();
    }
    // Check if Digger can shoot again.
    if (!can_shoot && global_clock > last_shoot_clock + CANNOT_SHOOT_CLK) {
      can_shoot = true;
      sprite_1 = get_digger_sprite_num(last_c, 1);
      sprite_2 = get_digger_sprite_num(last_c, 2);
      POKE(digger_sprite_addr, sprite_1);
    }
    // Check if Bonus mode or offer should end.
    if (last_bonus_clock > 0 &&
        global_clock > last_bonus_clock + BONUS_DURATION_CLK) {
      if (bonus_mode_state == BONUS_ON) {
        flash_screen();
        sid_index = MUSIC_POPCORN;
        play_popcorn();
      }
      num_killed_monsters = 0;
      POKE(BORDER_COLOR_REGISTER, 0);
      POKE(BG_COLOR_REGISTER, 0);
      clear_tile(spawn_x, spawn_y);
      last_bonus_clock = 0;
      bonus_mode_state = BONUS_OFF;
    }
    
    // We want to poll keyboard each game cycle even though movement might
    // not happen each game cycle. This is because VICE seems to depend
    // on real HW speed a bit.
    c = poll_joystik();   // Joystik overrides keyboard
    if (c == 0xFF) {
      c = poll_keyboard();
    }    
    if (c == KEY_SHOOT) {
      if (can_shoot && !is_bullet) {
        bullet_direction = last_c;
        shoot_bullet();
        sprite_1 = get_digger_sprite_num(bullet_direction, 1);
        sprite_2 = get_digger_sprite_num(bullet_direction, 2);
        POKE(digger_sprite_addr, sprite_1);
        last_shoot_clock = global_clock;
      }
    }
    if (c == KEY_RIGHT) {
      if (last_c != c || reset_sprite_after_first_key) {
        adjust_digger_pos(last_c, c);
        sprite_1 = get_digger_sprite_num(KEY_RIGHT, 1);
        sprite_2 = get_digger_sprite_num(KEY_RIGHT, 2);
        POKE(digger_sprite_addr, sprite_1);
        reset_sprite_after_first_key = false;
      }
      if (move_res = move_digger(c)) {
        if (map[digger_y][digger_x + 1] == MAP_GOLD && digger_x < 13) {
          clear_tile(digger_x + 2, digger_y);
          push_gold(digger_x + 1, digger_y, c);
        }
        if (digger_x < 14) {
          eat_tile(digger_x + 1, digger_y);
        }
      }
      last_c = KEY_RIGHT;
    }
    if (c == KEY_UP) {
      if (last_c != c || reset_sprite_after_first_key) {
        adjust_digger_pos(last_c, c);
        sprite_1 = get_digger_sprite_num(KEY_UP, 1);
        sprite_2 = get_digger_sprite_num(KEY_UP, 2);
        POKE(digger_sprite_addr, sprite_1);
        reset_sprite_after_first_key = false;
      }
      if (move_digger(c) && digger_y > 0) {
        eat_tile(digger_x, digger_y - 1);
      }
      last_c = KEY_UP;
    }
    if (c == KEY_LEFT) {
      if (last_c != c || reset_sprite_after_first_key) {
        adjust_digger_pos(last_c, c);
        sprite_1 = get_digger_sprite_num(KEY_LEFT, 1);
        sprite_2 = get_digger_sprite_num(KEY_LEFT, 2);
        POKE(digger_sprite_addr, sprite_1);
        reset_sprite_after_first_key = false;
      }
      if (move_res = move_digger(c)) {
        if (map[digger_y][digger_x - 1] == MAP_GOLD && digger_x > 1) {
          clear_tile(digger_x - 2, digger_y);
          push_gold(digger_x - 1, digger_y, c);
        }
        if (digger_x > 0) {
          eat_tile(digger_x - 1, digger_y);
        }
      }
      last_c = KEY_LEFT;
    }
    if (c == KEY_DOWN) {
      if (last_c != c || reset_sprite_after_first_key) {
        adjust_digger_pos(last_c, c);
        sprite_1 = get_digger_sprite_num(KEY_DOWN, 1);
        sprite_2 = get_digger_sprite_num(KEY_DOWN, 2);
        POKE(digger_sprite_addr, sprite_1);
        reset_sprite_after_first_key = false;
      }
      if (move_digger(c) && digger_y < 9) {
        eat_tile(digger_x, digger_y + 1);
      }        
      last_c = KEY_DOWN;
    }
    if (c == KEY_PAUSE) {
      clear_kbd_buffer();
      // Save clock-dependent variables.
      global_clock = clock();
      last_shoot_clock_diff = global_clock - last_shoot_clock;
      last_bonus_clock_diff = global_clock - last_bonus_clock;
      last_monster_spawn_clock_diff = global_clock - last_monster_spawn_clock;
      wait_for_key();
      // This trick skips PAUSE and FIRE keys when exiting pause.
      c = 0;
      while (c == 0 || c == KEY_PAUSE || c == KEY_SHOOT) {
        c = poll_keyboard();
      }
      // Restore clock-dependent variables.
      global_clock = clock();
      last_shoot_clock = global_clock - last_shoot_clock_diff;
      last_bonus_clock = global_clock - last_bonus_clock_diff;
      last_monster_spawn_clock = global_clock - last_monster_spawn_clock_diff;
    }
    if (c == KEY_SKIP_LEVEL && can_skip_levels) {
      level_complete();
    }
    
    update_bullet(cycle_counter);

    // It is important to update monsters before gold!
    // Otherwise the following can happen:
    // 1. Digger looks at monster to the right, between them is gold
    // 2. Digger pushes gold through monster
    // 3. Gold animation is performed, after which digger_x becomes > monster_y
    // 4. Therefore collision check in move_monster() doesn't work :)
    update_monsters(cycle_counter);

    // Animate gold.
    update_gold(cycle_counter);

    if (cycle_counter % ANIMATION_STEP == 0) {
      // Animate digger.
      if (PEEK(SPRITE_DIGGER_ADDR) == sprite_1) {
        POKE(SPRITE_DIGGER_ADDR, sprite_2);
      } else {
        POKE(SPRITE_DIGGER_ADDR, sprite_1);
      }
    }

    if (num_monsters_alive < NUM_SPAWNS && last_monster_spawn_clock +
        SEC_BETWEEN_MONSTERS * CLOCKS_PER_SEC < global_clock
        && bonus_mode_state != BONUS_ON) {
      spawn_monster();
      last_monster_spawn_clock = global_clock;
      num_monsters_alive++;
      adjust_game_step();        
    }

    cycle_counter++;
  }
  return 0;
}

void adjust_game_step() {
  // Calculate base step.
  switch (num_monsters_alive) {
    case 0:
      step = 1;
      break;
    case 1:
      step = 1;
      break;
    case 2:
      step = 1;
      break;
    case 3:
      step = 2;
      break;
    case 4:
      step = 2;
      break;
  }
  // If too little gold left and not many monsters,
  // decrease speed not to become too fast.
  if (num_gold_exist < 2 && num_monsters_alive < 2) {
    step = 1;  
  }  

  bullet_step = step * 3;
  gold_falling_step = step * 2;
}