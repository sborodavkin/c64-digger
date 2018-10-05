//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

////////////////////////////////////////////////////////////////////////////////
// Monster routines.
////////////////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <peekpoke.h>
#include "digger.h"
#include "monster.h"
#include "non_gameplay.h"
#include "bullet.h"
#include "gold.h"



struct Monster monsters[NUM_MONSTERS];
struct CoordAddr MON_COORD_ADDR[NUM_MONSTERS];
// Convenient index-based accessor to monster sprite addresses.
#define SPRITE_MON1_ADDR SPRITE4_ADDR
#define SPRITE_MON2_ADDR SPRITE5_ADDR
#define SPRITE_MON3_ADDR SPRITE6_ADDR
#define SPRITE_MON4_ADDR SPRITE7_ADDR
#define SPRITE_MON5_ADDR SPRITE8_ADDR

uint8_t WALK_STEP32_MONSTER;

uint16_t RANDOM_DIR_FLIP[NUM_LEVELS_WITH_RANDOM_FLIP] = {24000, 26000, 28000, 30000};

uint16_t SPRITE_MON_ADDR[NUM_MONSTERS] = {SPRITE_MON1_ADDR, SPRITE_MON2_ADDR,
                                          SPRITE_MON3_ADDR, SPRITE_MON4_ADDR, SPRITE_MON5_ADDR
                                         };


void set_most_significant_x_coord(uint8_t idx, bool is_set) {
  uint8_t mask = 1 << (idx + 3);
  if (!is_set) {
    mask = ~mask;
    POKE(0xD010, PEEK(0xD010) & mask);
  } else {
    POKE(0xD010, PEEK(0xD010) | mask);
  }
}

void set_monster_sprite(uint8_t idx, bool is_enable) {
  // i is monster number; monster sprite bits are 3-7.
  uint8_t mask = 1 << (idx + 3);
  if (!is_enable) {
    mask = ~mask;
    POKE(0xD015, PEEK(0xD015) & mask);
  } else {
    POKE(0xD015, PEEK(0xD015) | mask);
  }
}

void init_monsters() {
  uint8_t i = 0;
  for (; i < NUM_MONSTERS; i++) {
    monsters[i].is_alive = false;
    MON_COORD_ADDR[i].x_addr = 0xD006 + i * 2;
    MON_COORD_ADDR[i].y_addr = 0xD007 + i * 2;
  }
}

void spawn_monster() {
  uint8_t i = 0;
  for (; i < NUM_MONSTERS; i++) {
    if (!monsters[i].is_alive) {
      monsters[i].is_alive = true;
      monsters[i].num_crosses = 0;
      monsters[i].last_cross_x = 0;
      monsters[i].last_cross_y = 0;
      monsters[i].dir_change_delay = 0;
      monsters[i].is_hobbin = false;
      monsters[i].x = spawn_x;
      monsters[i].y = spawn_y;
      monsters[i].x_offset = 0;
      monsters[i].y_offset = 0;
      monsters[i].last_direction = KEY_LEFT;      
      place_monster(&monsters[i], i);
      set_monster_sprite(i, true);
      POKE(SPRITE_MON_ADDR[i], NOB_1_NUM);
      break;
    }
  }
}

void place_monster(struct Monster* monster, uint8_t idx) {
  uint8_t y_screen;
  uint16_t x_screen;  

  if (monster->is_alive) {
    if (monster->x_offset >= 15) {
      monster->x += 1;
      monster->x_offset = 0;
    } else if (monster->x_offset <= -15) {
      monster->x -= 1;
      monster->x_offset = 0;
    }
    if (monster->y_offset >= 15) {
      monster->y += 1;
      monster->y_offset = 0;
    } else if (monster->y_offset <= -15) {
      monster->y -= 1;
      monster->y_offset = 0;
    }

    y_screen = monster->y * 16 + 71 + monsters[idx].y_offset;
    x_screen = monster->x * 16 + 60 + monsters[idx].x_offset;
    if (x_screen <= 0xFF) {
      POKE(MON_COORD_ADDR[idx].x_addr, (uint8_t)x_screen);
      set_most_significant_x_coord(idx, false);
    } else {
      POKE(MON_COORD_ADDR[idx].x_addr, (uint8_t)(x_screen & 0xFF));
      set_most_significant_x_coord(idx, true);
    }
    POKE(MON_COORD_ADDR[idx].y_addr, y_screen);
    update_cross(monster, idx);
  }
}

void update_monsters(uint8_t cycle_counter) {
  struct Monster *monster;
  uint8_t monster_idx;
  uint8_t monster_dir;
  uint8_t rand_factor;  
  bool animate = !(cycle_counter % ANIMATION_STEP);

  // Move all monsters.
  for (monster_idx = 0; monster_idx < NUM_MONSTERS; monster_idx++) {
    monster = &monsters[monster_idx];
    if (monster->is_alive) {
      if (animate) {
        if (!monster->is_hobbin) {
          if (PEEK(SPRITE_MON_ADDR[monster_idx]) == NOB_1_NUM) {
            POKE(SPRITE_MON_ADDR[monster_idx], NOB_2_NUM);
          } else {
            POKE(SPRITE_MON_ADDR[monster_idx], NOB_1_NUM);
          }
        } else {
          if (PEEK(SPRITE_MON_ADDR[monster_idx]) == HOB_L_1_NUM) {
            POKE(SPRITE_MON_ADDR[monster_idx], HOB_L_2_NUM);
          } else {
            POKE(SPRITE_MON_ADDR[monster_idx], HOB_L_1_NUM);
          }
        }
      }
      rand_factor = rand() >> 10; // This gives random number 0-31.
      if (monster->is_alive && rand_factor > WALK_STEP32_MONSTER) {
        monster_dir = monster->last_direction;
        if (monster->dir_change_delay == 0) {
          if (monster->x_offset == 0 && monster->y_offset == 0) {
            monster_dir = get_monster_direction(monster);
            if (monster_dir != monster->last_direction) {
              monster->dir_change_delay = 3;
            }          
          }
        } else {
          monster->dir_change_delay--;          
        } 
        if (monster->is_hobbin) {
          switch (monster_dir) {
            case KEY_LEFT:
              if (monster->x > 0) {
                clear_tile(monster->x - 1, monster->y);
              }
              break;
            case KEY_RIGHT:
              if (monster->x < 14) {
                clear_tile(monster->x + 1, monster->y);
              }
              break;
            case KEY_UP:
              if (monster->y > 0) {
                clear_tile(monster->x, monster->y - 1);
              }
              break;
            case KEY_DOWN:
              if (monster->y < 9) {
                clear_tile(monster->x, monster->y + 1);
              }
              break;
          } 
        }        
        move_monster(monster, monster_idx, monster_dir);
        monster->last_direction = monster_dir;
      }      
      if (monster->is_alive && monster->x == digger_x && monster->y == digger_y) {
        if (bonus_mode_state == BONUS_ON) {
          total_score += 200;
          monster->is_alive = false;
          num_monsters_alive--;
          adjust_game_step();          
          statusbar_redraw_score();
        } else {
          kill_digger_and_restart();
        }
      }
    } else {
      // Remove sprite.
      set_monster_sprite(monster_idx, false);
    }
  }
}

void update_cross(struct Monster* monster, uint8_t idx) {
  uint8_t i = 0;
  struct Monster* monster_other;
  for(; i < NUM_MONSTERS; i++) {
    monster_other = &monsters[i];
    if (i != idx && monster_other->is_alive) {
      if (monster_other->x == monster->x && monster_other->y == monster->y &&
          (monster->last_cross_x != monster->x ||
           monster->last_cross_y != monster->y)) {
        monster->num_crosses++;
        monster->last_cross_y = monster->y;
        monster->last_cross_x = monster->x;
        if (monster->num_crosses >= NUM_CROSS_TO_HOBBIN) {
          monster->is_hobbin = (monster->is_hobbin ? false : true);
          monster->num_crosses == 0;
          monster_other->num_crosses == 0;
        }
      }
    }
  }
}

// Reverses given direction.
uint8_t reverse_direction(uint8_t direction) {
  uint8_t res = KEY_LEFT;
  switch (direction) {
    case KEY_UP:
      res = KEY_DOWN;
      break;
    case KEY_DOWN:
      res = KEY_UP;
      break;
    case KEY_LEFT:
      res = KEY_RIGHT;
      break;
    case KEY_RIGHT:
      res = KEY_LEFT;
      break;
  }
  return res;
}

bool is_map_clear(uint8_t x, uint8_t y, uint8_t direction, bool is_hobbin) {
  bool nogo = 0;
  switch (direction) {
    case KEY_UP:
      nogo = y == 0 || (!is_hobbin && map[--y][x]);
      break;
    case KEY_DOWN:
      nogo = y == 9 || (!is_hobbin && map[++y][x]);
      break;
    case KEY_LEFT:
      nogo = x == 0 || (!is_hobbin && map[y][--x]);
      break;
    default:
      nogo = x == 14 || (!is_hobbin && map[y][++x]);;
      break;
  }
  return !nogo;
}

uint8_t get_monster_direction(struct Monster* monster) {
  // We reuse KEY_* constants just for convenience.
  uint8_t p0, p1, p2, p3, reverse_dir;
  if (abs(monster->y - digger_y) > abs(monster->x - digger_x)) {
    if (digger_y > monster->y) {
      p0 = KEY_DOWN;
      p3 = KEY_UP;
    } else {
      p0 = KEY_UP;
      p3 = KEY_DOWN;
    }
    if (digger_x < monster->x) {
      p1 = KEY_LEFT;
      p2 = KEY_RIGHT;
    } else {
      p1 = KEY_RIGHT;
      p2 = KEY_LEFT;
    }
  } else {
    if (digger_x < monster->x) {
      p0 = KEY_LEFT;
      p3 = KEY_RIGHT;
    } else {
      p0 = KEY_RIGHT;
      p3 = KEY_LEFT;
    }
    if (digger_y > monster->y) {
      p1 = KEY_DOWN;
      p2 = KEY_UP;
    } else {
      p1 = KEY_UP;
      p2 = KEY_DOWN;
    }
  }

  if (bonus_mode_state == BONUS_ON) {
    reverse_dir = p3; p3 = p0; p0 = reverse_dir;
    reverse_dir = p2; p2 = p1; p1 = reverse_dir;
  } else {
    reverse_dir = reverse_direction(monster->last_direction);
    if (reverse_dir == p0) {
      p0 = p1;
      p1 = p2;
      p2 = p3;
      p3 = reverse_dir;
    }
    if (reverse_dir == p1) {
      p1 = p2;
      p2 = p3;
      p3 = reverse_dir;
    }
    if (reverse_dir == p2) {
      p2 = p3;
      p3 = reverse_dir;
    }
  }

  // On levels 0-4 randomly swap p0 and p2.
  if (current_level < NUM_LEVELS_WITH_RANDOM_FLIP
      && rand() > RANDOM_DIR_FLIP[current_level])  {
    reverse_dir = p2;
    p2 = p0;
    p0 = reverse_dir;
  }

  if (is_map_clear(monster->x, monster->y, p0, monster->is_hobbin)) {
    return p0;
  }
  if (is_map_clear(monster->x, monster->y, p1, monster->is_hobbin)) {
    return p1;
  }
  if (is_map_clear(monster->x, monster->y, p2, monster->is_hobbin)) {
    return p2;
  }
  if (is_map_clear(monster->x, monster->y, p3, monster->is_hobbin)) {
    return p3;
  }  
  return monster->last_direction;
}

void move_monster(struct Monster* monster, uint8_t idx, uint8_t direction) {  
  if (monster->is_hobbin) {
    switch (direction) {
      case KEY_RIGHT: monster->x_offset += step; break;
      case KEY_UP: monster->y_offset -= step; break;
      case KEY_LEFT: monster->x_offset -= step; break;
      case KEY_DOWN: monster->y_offset += step; break;
    }
    place_monster(monster, idx);
    delete_gold(monster->x, monster->y);
  } else {
    if (direction == KEY_RIGHT && is_map_clear(monsters[idx].x, monsters[idx].y, KEY_RIGHT, false)) {
      monster->x_offset += step;      
    }
    if (direction == KEY_UP && is_map_clear(monsters[idx].x, monsters[idx].y, KEY_UP, false)) {
      monster->y_offset -= step;      
    }
    if (direction == KEY_LEFT && is_map_clear(monsters[idx].x, monsters[idx].y, KEY_LEFT, false)) {
      monster->x_offset -= step;      
    }
    if (direction == KEY_DOWN && is_map_clear(monsters[idx].x, monsters[idx].y, KEY_DOWN, false)) {
      monster->y_offset += step;      
    }
    place_monster(monster, idx);
  }
}

uint8_t check_bullet_collision_with_monster(void) {
  struct Monster* monster;
  uint8_t i = 0;
  for (; i < NUM_MONSTERS; i++) {
    if (monsters[i].is_alive) {
      monster = &monsters[i];
      if (bullet_direction == KEY_UP && bullet_x == monster->x) {
        if (bullet_y == monster->y ||
            (bullet_y == monster->y + 1 && bullet_y_offset <= -7)) {
          return i;
        }
      } else if (bullet_direction == KEY_DOWN && bullet_x == monster->x) {
        if (bullet_y == monster->y ||
            (bullet_y == monster->y - 1 && bullet_y_offset >= 7)) {
          return i;
        }
      } else if (bullet_direction == KEY_LEFT && bullet_y == monster->y) {
        if (bullet_x == monster->x ||
            (bullet_x == monster->x + 1 && bullet_x_offset <= -7)) {
          return i;
        }
      } else if (bullet_direction == KEY_RIGHT && bullet_y == monster->y) {
        if (bullet_x == monster->x ||
            (bullet_x == monster->x - 1 && bullet_x_offset >= 7)) {
          return i;
        }
      }
    }
  }
  return BULLET_MISS;
}