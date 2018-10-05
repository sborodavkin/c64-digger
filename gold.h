//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef GOLD_H
#define GOLD_H


////////////////////////////////////////////////////////////////////////////////
// Definitions: gold.
////////////////////////////////////////////////////////////////////////////////

// Sprite index that serves gold.
#define SPRITE_GOLD_ADDR SPRITE2_ADDR

// Clock cycle count between wobble gold animation.
#define WOBBLE_ANIMATION_STEP 5

// Total number of gold on map.
extern uint8_t num_gold;

// Number of non-broken, non-eaten gold bags on level.
extern uint8_t num_gold_exist;

// Fall step count of falling gold. Effectively this is number of tiles
// that a bag fell down since it took off.
extern uint8_t gold_falling_step;

// Global flag serving wobble gold animation. It is flipped each
// WOBBLE_ANIMATION_STEP game cycle, and depending on its value we 
// draw one or another wobbled gold character.
extern bool wobble_animation_flag;

// Maximum number of gold bags on level.
#define MAX_NUM_GOLD 20

struct Gold {
  bool exists;                 // Is gold exist? (not broken, not eaten?)
  uint8_t x, y;                // Tile coordinates
  int8_t x_offset, y_offset;   // Offset within tile.
  bool is_wobbling;            // Is gold wobbling?
  bool is_falling;             // Is gold falling down?
  uint8_t fall_distance;       // How far down had the gold fallen?
  bool is_animating;           // Is the gold animating?
  uint8_t direction;           // Where is the gold sprite moving?
  bool is_on_blank;            // Is it rendered on blank? (i.e. after falling down 1 square)
  clock_t gold_wobble_time;    // How much time is gold wobbling already?
};

// Registers to store gold sprite X and Y coords.
#define GOLD_X_ADDR 0xD002
#define GOLD_Y_ADDR 0xD003

// How much clocks should the gold wobble before falling down?
// 90 = 1.5 x CLOCKS_PER_SEC
#define CLOCKS_WOBBLE 90

// All gold on level.
extern struct Gold all_gold[];

// Becomes true if we need to stop all animations
// (e.g. when entering a new level or when Digger is killed).
extern bool stop_gold_animation;


////////////////////////////////////////////////////////////////////////////////
// Definitions: gold.
////////////////////////////////////////////////////////////////////////////////

// Initializes a gold bag at given tile.
extern void init_gold(uint8_t x, uint8_t y);

// Deletes gold from a given tile by marking it .is_exist = false
// Happens when Digger or Hobbin eat it.
extern void delete_gold(uint8_t x, uint8_t y);

// Digger had pushed the gold left or right.
extern void push_gold(uint8_t x, uint8_t y, uint8_t direction);

// Is called each game cycle to update gold state and on screen.
extern void update_gold(uint8_t cycle_counter);

// If gold sprite X>255, is_set should be true.
extern void set_most_significant_gold_x_coord(bool is_set);

// Enables or disables gold bag sprite.
extern void set_gold_sprite(bool is_enable);

// Is the gold located in the same column of tiles with Digger? Returns
// true or false.
extern bool check_gold_same_column_with_digger(uint8_t gx, int8_t gox);

// Is the gold located right above Digger?
extern bool check_gold_right_above_digger(uint8_t gx, int8_t gox, uint8_t gy);

// Has the falling gold collided with Digger. Returns true or false.
extern bool check_gold_collide_with_digger(uint8_t gx, uint8_t gy,
    int8_t gox, int8_t goy);

// Has the falling gold collided with monster. Returns true or false.
extern bool check_gold_collide_with_monster(uint8_t gx, uint8_t gy,
    int8_t gox, int8_t goy, uint8_t mx, uint8_t my, int8_t mox);

// Turns gold animation off. Is invoked when stop_gold_animation
// becomes true.
extern void turnoff_gold_animation(void);

#endif

