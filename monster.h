//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <stdbool.h>
#include <stdint.h>


#ifndef MONSTER_H
#define MONSTER_H

// Theoretical maximum of monsters. We support only 3 atm because of speed.
#define NUM_MONSTERS 5

// How many times monsters have to cross each other so that one of them
// becomes Hobbin.
#define NUM_CROSS_TO_HOBBIN 3

// Defined the probability that monster makes a step as M/32.
// This value should be 0 <= M < 31. If 0, monster moves each cycle; if 31,
// monster doesn't move at all.
extern uint8_t WALK_STEP32_MONSTER;

// On levels 0-NUM_LEVELS_WITH_RANDOM_FLIP-1 we flip randomly monsters'
// direction. RANDOM_DIR_FLIP defines the probability.
#define NUM_LEVELS_WITH_RANDOM_FLIP 4
extern uint16_t RANDOM_DIR_FLIP[];

struct Monster {
  uint8_t x, y;                        // Current tile.
  int8_t x_offset, y_offset;           // Offset within a tile.  
  uint8_t dir_change_delay;            // If not 0, direction is not changed.
  uint8_t last_direction;              // Last move direction.
  uint8_t num_crosses;                 // Number of crosses since spawn 
                                       //     or last Nobbin/Hobbin switch.
  uint8_t last_cross_x, last_cross_y;  // Where on map the last cross happened.
  bool is_hobbin;                      // Is Hobbin?
  bool is_alive;                       // Is alive?
};
extern struct Monster monsters[];

// Stores addresses of registers that contain sprite coordinates for a specific
// monster.
// E.g. x_addr=$D006, y_addr=$D007 for monster 0 and so on.
struct CoordAddr {
  uint16_t x_addr;
  uint16_t y_addr;
};
extern struct CoordAddr MON_COORD_ADDR[];

// List of sprite pointer registers reserved for monsters.
extern uint16_t SPRITE_MON_ADDR[];

// Marks all monsters non alive and sets MON_COORD_ADDR in sync.
extern void init_monsters(void);

// Spawns a monsters in (spawn_x, spawn_y).
extern void spawn_monster(void);

// Turns on or off (is_enable) the monster sprite for monster with index idx.
extern void set_monster_sprite(uint8_t idx, bool is_enable);

// Updates given monster's x and y based on offset. idx is monster's index
// Also updates the sprite position.
// NOTE: here and below we often pass monster itself and its index in monsters
// array. This is because the callchain needs to eventually update monsters
// sprite or coordinates array, for which the index is needed too.
extern void place_monster(struct Monster* monster, uint8_t idx);

// Calculates the direction for given monster based on its location
// (and Digger's global location). Returns KEY_(UP|DOWN|LEFT|RIGHT). Takes into
// consideration whether Bonus mode is on (when monsters need to escape instead
// of chasing).
extern uint8_t get_monster_direction(struct Monster* monster);

// Helper function to reverse the given direction (returns KEY_UP for KEY_DOWN
// and so on).
extern uint8_t reverse_direction(uint8_t direction);

// Checks if a monster can enter a given map tile.
// x,y is tile where a monster is located.
// direction is where a monster "wants" to move.
// is_hobbin allows to move into MAZE tiles (as Hobbins can eat them).
// Returns true if move is possible.
extern bool is_map_clear(uint8_t x, uint8_t y, uint8_t direction,
    bool is_hobbin);

// Moves monster of given index into a given direction.
extern void move_monster(struct Monster* monster, uint8_t idx, uint8_t direction);

// Sets the bit in 0xD010 whether X-coord of a monster exceeds 255.
// idx is monster's index.
extern void set_most_significant_x_coord(uint8_t idx, bool is_set);

// Turns on or off a monster's sprite.
extern void set_monster_sprite(uint8_t idx, bool is_enable);

// Updates monsters positions and checks collisions. Is called each game cycle.
// cycle_counter is 0-255 game iterations loop counter. Inside % operator is
// used to check if monsters should be animated.
extern void update_monsters(uint8_t cycle_counter);

// Updates num_crosses for given monster if it crosses with other monsters. 
// If num_crosses exceed NUM_CROSS_TO_HOBBIN, switches it to Hobbin (or Nobbin
// if was Hobbin before).
extern void update_cross(struct Monster* monster, uint8_t idx);

// Checks if a bullet collided with monster. Returns monster index that was hit,
// or BULLET_MISS if no collision has been detected.
extern uint8_t check_bullet_collision_with_monster(void);

#endif