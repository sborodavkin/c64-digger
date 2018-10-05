//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

#include <peekpoke.h>
#include "bullet.h"
#include "digger.h"
#include "monster.h"
#include "non_gameplay.h"


bool can_shoot;
uint8_t bullet_x, bullet_y;
int8_t bullet_x_offset, bullet_y_offset;
int8_t bullet_step;
bool is_bullet;
uint8_t bullet_direction;


bool check_bullet_collision_with_map() {
  if (bullet_direction == KEY_UP) {
    // bullet_y > 9 is overflow check for 0-1.
    if (bullet_y == 0 || bullet_y > 9 ||
          (bullet_y == 1 && bullet_y_offset - bullet_step <= -16)) {
      return true;
    }
    if (map[bullet_y - 2][bullet_x] != MAP_EMPTY
        && bullet_y_offset - bullet_step <= -16) {
      return true;
    }
  } else if (bullet_direction == KEY_DOWN) {
    if (bullet_y >= 9 || (bullet_y == 8 && bullet_y_offset + bullet_step >= 16)) {
      return true;
    }
    if (map[bullet_y + 1][bullet_x] != MAP_EMPTY
        && bullet_y_offset + bullet_step >= 16) {
      return true;
    }
  } else if (bullet_direction == KEY_LEFT) {
    // bullet_x > 14 is overflow check for 0-1.
    if (bullet_x == 0 || bullet_x > 14 ||
          (bullet_x == 1 && bullet_x_offset - bullet_step <= -16)) {
      return true;
    }
    if (map[bullet_y][bullet_x - 2] != MAP_EMPTY
        && bullet_x_offset - bullet_step <= -16) {
      return true;
    }
  } else if (bullet_direction == KEY_RIGHT) {    
    if (bullet_x >= 14 || (bullet_x == 13 && bullet_x_offset + bullet_step >= 16)) {
      return true;
    }
    if (map[bullet_y][bullet_x + 1] != MAP_EMPTY
        && bullet_x_offset + bullet_step >= 16) {
      return true;
    }
  }
  return map[bullet_y][bullet_x] != MAP_EMPTY;
}

void shoot_bullet() {
  if (!shoot_without_delay) {
    can_shoot = false;
  }
  if (is_bullet) {
    return;
  }
  is_bullet = true;  
  switch (bullet_direction) {
    case KEY_UP:
      bullet_x = digger_x;
      bullet_x_offset = 0;
      bullet_y = digger_y - 1;
      bullet_y_offset = 4;
      break;
    case KEY_DOWN:
      bullet_x = digger_x;
      bullet_x_offset = 0;
      bullet_y = digger_y + 1;
      bullet_y_offset = 0;
      break;
    case KEY_LEFT:
      bullet_y = digger_y;
      bullet_y_offset = 0;
      bullet_x = digger_x - 1;
      bullet_x_offset = 4;
      break;
    case KEY_RIGHT:
      bullet_y = digger_y;
      bullet_y_offset = 0;
      bullet_x = digger_x + 1;
      bullet_x_offset = 0;
      break;
  }
  
}

void update_bullet(uint8_t cycle_counter) {
  uint8_t y_screen;
  uint16_t x_screen;
  uint8_t monster_idx;

  if (!is_bullet) {
    return;
  }

  // Check if bullet has hit the monster.
  monster_idx = check_bullet_collision_with_monster();
  if (monster_idx != BULLET_MISS) {
    set_bullet_sprite(false);
    if ((++num_killed_monsters) >= NUM_KILLS_TO_BONUS
        && bonus_mode_state != BONUS_ON) {
      bonus_mode_state = BONUS_CAN_HAVE;
      draw_bonus(spawn_x, spawn_y);
      last_bonus_clock = clock();
    }
    monsters[monster_idx].is_alive = false;    
    num_monsters_alive--;
    adjust_game_step();
    total_score += 250;
    statusbar_redraw_score();
    is_bullet = false;
    return;
  }
  // Check if bullet collided with map.
  if (check_bullet_collision_with_map()) {
    set_bullet_sprite(false);
    is_bullet = false;
    return;
  }

  // Animate bullet
  switch (bullet_direction) {
    case KEY_UP:
      if (bullet_y_offset - bullet_step <= -16) {
        bullet_y--;
        bullet_y_offset = 0;
      } else {
        bullet_y_offset -= bullet_step;
      }
      break;
    case KEY_DOWN:
      if (bullet_y_offset + bullet_step >= 16) {
        bullet_y++;
        bullet_y_offset = 0;
      } else {
        bullet_y_offset += bullet_step;
      }
      break;
    case KEY_LEFT:
      if (bullet_x_offset - bullet_step <= -16) {
        bullet_x--;
        bullet_x_offset = 0;
      } else {
        bullet_x_offset -= bullet_step;
      }
      break;
    case KEY_RIGHT:
      if (bullet_x_offset + bullet_step >= 16) {
        bullet_x++;
        bullet_x_offset = 0;
      } else {
        bullet_x_offset += bullet_step;
      }
      break;
  }

  y_screen = bullet_y * 16 + 71 + bullet_y_offset;
  x_screen = bullet_x * 16 + 60 + bullet_x_offset + 1;

  if (x_screen <= 255) {
    POKE(BULLET_X_ADDR, (uint8_t)x_screen);
    set_most_significant_bullet_x_coord(false);
  } else {
    POKE(BULLET_X_ADDR, (uint8_t)(x_screen & 0xFF));
    set_most_significant_bullet_x_coord(true);
  }
  POKE(BULLET_Y_ADDR, y_screen);
  set_bullet_sprite(true);
  if (cycle_counter % ANIMATE_BULLET_STEP < 2) {
    POKE(SPRITE_BULLET_ADDR, BULLET1_NUM);
  } else {
    POKE(SPRITE_BULLET_ADDR, BULLET2_NUM);
  }
}

void set_most_significant_bullet_x_coord(bool is_set) {
  uint8_t mask = 4;
  if (!is_set) {
    mask = ~mask;
    POKE(0xD010, PEEK(0xD010) & mask);
  } else {
    POKE(0xD010, PEEK(0xD010) | mask);
  }
}

void set_bullet_sprite(bool is_enable) {
  uint8_t mask = 4;
  if (!is_enable) {
    mask = ~mask;
    POKE(0xD015, PEEK(0xD015) & mask);
  } else {
    POKE(0xD015, PEEK(0xD015) | mask);
  }
}
