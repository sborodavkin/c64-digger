//
// Copyright (c) Sergey Borodavkin. All rights reserved.
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.
//

#include <stdint.h>
#include <stdbool.h>

#ifndef BULLET_H
#define BULLET_H

////////////////////////////////////////////////////////////////////////////////
// Definitions: bullet / shooting.
////////////////////////////////////////////////////////////////////////////////

#define BULLET_X_ADDR 0xD004
#define BULLET_Y_ADDR 0xD005
#define BULLET_MISS 0xFF

// Number of clocks (60*10) that digger can't shoot after he shoots.
#define CANNOT_SHOOT_CLK 600

// Bullet animation step.
#define ANIMATE_BULLET_STEP 4

// Read on digger_x / digger_y regarding coordinate/positioning system.
extern uint8_t bullet_x, bullet_y;
extern int8_t bullet_x_offset, bullet_y_offset;

// Bullet direction: KEY_UP, KEY_DOWN, KEY_LEFT or KEY_RIGHT.
extern uint8_t bullet_direction;

// Bullet speed. We keep it 2*step (see digger.h) throughout the code.
extern int8_t bullet_step;

// Is bullet now on screen.
extern bool is_bullet;

// Can digger shoot? Is false during CANNOT_SHOOT_SEC after previous shot.
extern bool can_shoot;


////////////////////////////////////////////////////////////////////////////////
// Functions: bullet / shooting.
////////////////////////////////////////////////////////////////////////////////

// Shoots the bullet in bullet_direction from the digger position.
extern void shoot_bullet(void);

// Sets the bit if bullet's X coord exceeds 255.
extern void set_most_significant_bullet_x_coord(bool is_set);

// Shows or hides bullet sprite.
extern void set_bullet_sprite(bool is_enable);

// Is called each game loop. Updates the bullet position and checks for
// collision with maze and monsters.
extern void update_bullet(uint8_t cycle_counter);

// Checks if bullet collided with map.
extern bool check_bullet_collision_with_map(void);



#endif