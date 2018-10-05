//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <peekpoke.h>
#include <time.h>
#include "gold.h"
#include "digger.h"
#include "bullet.h"
#include "monster.h"
#include "non_gameplay.h"

struct Gold all_gold[MAX_NUM_GOLD];
uint8_t num_gold = 0;
uint8_t num_gold_exist = 0;
bool stop_gold_animation = false;
bool wobble_animation_flag = false;
uint8_t gold_falling_step;


void init_gold(uint8_t x, uint8_t y) {
  // 10 below stands not for any specific direction.
  struct Gold g = {true, 0, 0, 0, 0, false, false, 0, false, 0xFF, false, 0};
  g.x = x;
  g.y = y;
  all_gold[num_gold++] = g;  
}

void delete_gold(uint8_t x, uint8_t y) {
  uint8_t i = 0;
  for (; i < num_gold; i++) {
    if (all_gold[i].exists && all_gold[i].x == x && all_gold[i].y == y) {
      all_gold[i].is_wobbling = false; // Push overrides previous wobble.
      all_gold[i].is_animating = false;
      all_gold[i].is_falling = false;
      all_gold[i].exists = false;
      break;
    }
  }
}

void push_gold(uint8_t x, uint8_t y, uint8_t direction) {
  uint8_t i = 0;
  for (; i < num_gold; i++) {
    if (all_gold[i].exists && all_gold[i].x == x && all_gold[i].y == y) {
      all_gold[i].is_wobbling = false; // Push overrides previous wobble.
      all_gold[i].is_animating = true;
      all_gold[i].direction = direction;
      break;
    }
  }
}

void turnoff_gold_animation() {
  uint8_t i = 0;  
  set_gold_sprite(false);    
  for (; i < num_gold; i++) {
    all_gold[i].gold_wobble_time = 0;
    if (all_gold[i].is_animating && all_gold[i].direction == KEY_DOWN) {
      all_gold[i].exists = false;
    }
    all_gold[i].is_wobbling = false;
    all_gold[i].is_animating = false;
    all_gold[i].is_falling = false;
  }  
}

void update_gold(uint8_t cycle_counter) {
  struct Gold* gold;
  uint8_t i, j;
  uint8_t y_screen;
  uint16_t x_screen;

  if (stop_gold_animation) {
    // Stop animation when told to do so (e.g. game is restarted).    
    turnoff_gold_animation();
    stop_gold_animation = false;
    return;
  }

  // Go through all gold. Check if something has to start wobbling.
  // Then animate all wobbling gold.
  for(i = 0; i < num_gold; i++) {
    gold = &all_gold[i];
    if (gold->exists) {
      // Check if needs to start wobbling.
      if (!(gold->is_falling || gold->is_wobbling || gold->is_animating
          || gold->y >= 9 || map[gold->y+1][gold->x] != MAP_EMPTY
          || check_gold_right_above_digger(gold->x, 0, gold->y))) {
        gold->is_wobbling = true;
        gold->gold_wobble_time = global_clock;
      }

      // If wobbling, check if needs to start animating.
      if (gold->is_wobbling) {
        if (global_clock > gold->gold_wobble_time + CLOCKS_WOBBLE) {
          play_sfx_gold_fall();
          gold->is_animating = true;
          gold->is_wobbling = false;
          gold->gold_wobble_time = 0;
          gold->is_falling = true;
          gold->fall_distance = 0;
          gold->direction = KEY_DOWN;          
          clear_tile(gold->x, gold->y);
          adjust_game_step();
        } else if (cycle_counter % WOBBLE_ANIMATION_STEP == 0) {
          if (gold->is_on_blank) {
            draw_gold_on_blank(gold->x, gold->y, wobble_animation_flag);
          } else {
            draw_gold(gold->x, gold->y, wobble_animation_flag);        
          }
        }
      }

      // If gold is animating, check if it's arrived already.
      if (gold->is_animating) {
        if (gold->direction == KEY_LEFT) {
          if (gold->x_offset < -15) {
            gold->x--;
            gold->x_offset = 0;
            if (gold->y < 9 && map[gold->y + 1][gold->x] == MAP_EMPTY) {
              gold->is_falling = true;
              gold->is_animating = true;
              gold->direction = KEY_DOWN;
              gold->fall_distance = 0;
              adjust_game_step();
              play_sfx_gold_fall();
            } else {
              // Animation end, draw char there.
              gold->is_animating = false;
              draw_gold_on_blank(gold->x, gold->y, false);
              gold->is_on_blank = true;
              map[gold->y][gold->x] = MAP_GOLD;
              set_gold_sprite(false);
            }

          }
        } else if (gold->direction == KEY_RIGHT) {
          if (gold->x_offset > 15) {
            gold->x++;
            gold->x_offset = 0;
            if (gold->y < 9 && map[gold->y + 1][gold->x] == MAP_EMPTY) {
              gold->is_falling = true;
              gold->is_animating = true;
              gold->direction = KEY_DOWN;
              gold->fall_distance = 0;
              adjust_game_step();              
              play_sfx_gold_fall();
            } else {
              gold->is_animating = false;
              // Animation end, draw char there.
              draw_gold_on_blank(gold->x, gold->y, false);
              gold->is_on_blank = true;
              map[gold->y][gold->x] = MAP_GOLD;
              set_gold_sprite(false);
            }
          }
        } else if (gold->direction == KEY_DOWN) {
          if (gold->y_offset > 15) {
            gold->y++;
            gold->y_offset = 0;
            gold->fall_distance++;
            if (gold->y == 9 || map[gold->y + 1][gold->x] == MAP_MAZE ||
                map[gold->y + 1][gold->x] == MAP_GOLD
                || map[gold->y + 1][gold->x] == MAP_EMERALD) {
              gold->is_animating = false;
              gold->is_wobbling = false;
              if (gold->fall_distance > 1) {
                play_sfx_gold_broken();
                gold->exists = false;
                num_gold_exist--;
                draw_brokengold(gold->x, gold->y);
                map[gold->y][gold->x] = MAP_BROKENGOLD;
              } else {
                gold->fall_distance = 0;
                draw_gold_on_blank(gold->x, gold->y, false);       
                gold->is_on_blank = true;         
                gold->is_falling = false;
                gold->is_animating = false;
                map[gold->y][gold->x] = MAP_GOLD;
              }
              set_gold_sprite(false);
              adjust_game_step();              
            }
          }
        }        
      }

      // Animate.
      if (stop_gold_animation) {
        break;
      }
      if (gold->is_animating) {
        // Place sprite there.
        set_gold_sprite(true);
        POKE(SPRITE_GOLD_ADDR, GOLD_NUM);
        y_screen = gold->y * 16 + 71 + gold->y_offset;
        x_screen = gold->x * 16 + 60 + gold->x_offset +
                   1; // TODO: fix sprite clash with charset
        if (x_screen <= 255) {
          POKE(GOLD_X_ADDR, (uint8_t)x_screen);
          set_most_significant_gold_x_coord(false);
        } else {
          POKE(GOLD_X_ADDR, (uint8_t)(x_screen & 0xFF));
          set_most_significant_gold_x_coord(true);
        }
        POKE(GOLD_Y_ADDR, y_screen);
        // Move gold.
        switch(gold->direction) {
          case KEY_LEFT:
            gold->x_offset -= (step<<1);
            break;
          case KEY_RIGHT:
            gold->x_offset += (step<<1);
            break;
          case KEY_DOWN:
            gold->y_offset += gold_falling_step;
            // Collision check
            if (gold->is_falling && check_gold_collide_with_digger(gold->x, gold->y,
                  gold->x_offset, gold->y_offset)) {
              kill_digger_and_restart();
              break;
            }
            for (j = 0; j < NUM_MONSTERS; j++) {
              if (monsters[j].is_alive && check_gold_collide_with_monster(
                  gold->x, gold->y, gold->x_offset, gold->y_offset,
                  monsters[j].x, monsters[j].y, monsters[j].x_offset)) {
                monsters[j].is_alive = false;
                num_monsters_alive--;
                num_killed_monsters++;
                adjust_game_step();
                total_score += 250;
                statusbar_redraw_score();
              }  
            }
            if (num_killed_monsters >= NUM_KILLS_TO_BONUS 
                  && bonus_mode_state != BONUS_ON) {
              bonus_mode_state = BONUS_CAN_HAVE;
              draw_bonus(spawn_x, spawn_y);
              last_bonus_clock = clock();
            }
            break;
        }
      }
    }
  }
  wobble_animation_flag = !wobble_animation_flag;  
}

bool check_gold_same_column_with_digger(uint8_t gx, int8_t gox) {
  bool is_gold_same_column = false;
  if (gx == digger_x) {
    is_gold_same_column = abs(gox - digger_x_offset) < 15;
  }
  if (gx == digger_x + 1) {
    is_gold_same_column = digger_x_offset >= 0 && gox < -2; 
  }
  if (gx == digger_x - 1) {
    is_gold_same_column = digger_x_offset <= 0 && gox > 2; 
  }
  return is_gold_same_column;
}

bool check_gold_right_above_digger(uint8_t gx, int8_t gox, uint8_t gy) {
  if (check_gold_same_column_with_digger(gx, gox)) {
    if (gy == digger_y - 1 || (gy == digger_y - 2 && digger_y_offset <= 0)) {
      return true;
    }
  }
  return false;  
}

bool check_gold_collide_with_digger(uint8_t gx, uint8_t gy,
    int8_t gox, int8_t goy) {  
  if (check_gold_same_column_with_digger(gx, gox)) {
    if ((gy == digger_y - 1 && goy >= 7) ||
        (gy == digger_y && goy < digger_y_offset)) {
      return true;
    }
  }
  return false;
}

bool check_gold_collide_with_monster(uint8_t gx, uint8_t gy, int8_t gox,
    int8_t goy, uint8_t mx, uint8_t my, int8_t mox) {
  bool is_gold_same_column = false;
  if (gx == mx) {
    is_gold_same_column = abs(gox - mox) < 15;
  }
  if (gx == mx + 1) {
    is_gold_same_column = mox >= 2;  // To be honest gox is always 0 
  }                                  // when we need to check for collision.
  if (gx == mx - 1) {
    is_gold_same_column = mox <= -2; 
  }
  if (is_gold_same_column) {  
    if ((gy == my - 1 && goy >= 7) || (gy == my)) {
      return true;
    }
  }
  return false;   
}

void set_most_significant_gold_x_coord(bool is_set) {
  uint8_t mask = 2;
  if (!is_set) {
    mask = ~mask;
    POKE(0xD010, PEEK(0xD010) & mask);
  } else {
    POKE(0xD010, PEEK(0xD010) | mask);
  }
}

void set_gold_sprite(bool is_enable) {
  uint8_t mask = 2;
  if (!is_enable) {
    mask = ~mask;
    POKE(0xD015, PEEK(0xD015) & mask);
  } else {
    POKE(0xD015, PEEK(0xD015) | mask);
  }
}
