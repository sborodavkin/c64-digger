//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <stdbool.h>
#include <stdint.h>
#include <cbm.h>
#include <time.h>

#ifndef DIGGER_H
#define DIGGER_H

////////////////////////////////////////////////////////////////////////////////
// Definitions: memory addresses and registers.
////////////////////////////////////////////////////////////////////////////////

// Screen memory start, set up in main().
#define SCREEN 0x8000

// Color memory start.
#define COLOR 0xD800

// C64-specific registers to controls background color.
#define BORDER_COLOR_REGISTER 0xD020
#define BG_COLOR_REGISTER 0xD021


////////////////////////////////////////////////////////////////////////////////
// Definitions: sprites.
////////////////////////////////////////////////////////////////////////////////

// Bits 8-1 control whether a particular sprite is multicolor.
#define SPRITES_MULTICOLOR_REGISTER 0xD01C

// Shared multicolor 1 of multicolor sprites.
#define MULTICOLOR_1_ADDRESS 0xD025
// Shared multicolor 2 of multicolor sprites.
#define MULTICOLOR_2_ADDRESS 0xD026
// Own colors of sprites 1-8.
#define SPRITE1_COLOR_ADDR 0xD027
#define SPRITE2_COLOR_ADDR 0xD028
#define SPRITE3_COLOR_ADDR 0xD029
#define SPRITE4_COLOR_ADDR 0xD02A
#define SPRITE5_COLOR_ADDR 0xD02B
#define SPRITE6_COLOR_ADDR 0xD02C
#define SPRITE7_COLOR_ADDR 0xD02D
#define SPRITE8_COLOR_ADDR 0xD02E

// Sprite size (always 64 bytes).
#define SPRITE_SIZE 0x40

// 8 sprite pointers. Each one points to a sprite offset
// calculated as (SPRITE_ADDRESS+sprite_num)/64 and defined below.
// E.g. after POKE(SPRITE1_ADDR, DIG_R_2_NUM) sprite 1 will display what's
// loaded in DIG_R_2_NUM * 64.
#define SPRITE1_ADDR 0x83F8
#define SPRITE2_ADDR 0x83F9
#define SPRITE3_ADDR 0x83FA
#define SPRITE4_ADDR 0x83FB
#define SPRITE5_ADDR 0x83FC
#define SPRITE6_ADDR 0x83FD
#define SPRITE7_ADDR 0x83FE
#define SPRITE8_ADDR 0x83FF

#define SPRITE_DIGGER_ADDR SPRITE1_ADDR
#define SPRITE_BULLET_ADDR SPRITE3_ADDR

// Sprite offsets bank 1. See mem.cfg.
#define DIG_R_1_NUM 0x80
#define DIG_R_2_NUM 0x81
#define DIG_U_1_NUM 0x82
#define DIG_U_2_NUM 0x83
#define DIG_L_1_NUM 0x84
#define DIG_L_2_NUM 0x85
#define DIG_D_1_NUM 0x86
#define DIG_D_2_NUM 0x87
// Sprite offsets bank 2. See mem.cfg.
#define DIG_RE_1_NUM 0xB2
#define DIG_RE_2_NUM 0xB3
#define DIG_UE_1_NUM 0xB4
#define DIG_UE_2_NUM 0xB5
#define DIG_LE_1_NUM 0xB6
#define DIG_LE_2_NUM 0xB7
#define DIG_DE_1_NUM 0xB8
#define DIG_DE_2_NUM 0xB9
#define DIG_DEAD_NUM 0xBA
#define NOB_1_NUM 0xBB
#define NOB_2_NUM 0xBC
#define GOLD_NUM 0xBD
#define HOB_L_1_NUM 0xBE
#define HOB_L_2_NUM 0xBF
#define BULLET1_NUM 0xC0
#define BULLET2_NUM 0xC1


////////////////////////////////////////////////////////////////////////////////
// Definitions: input.
////////////////////////////////////////////////////////////////////////////////

// Determined experimentally by: 10 PRINTPEEK(197): GOTO 10
#define KEY_RIGHT 18
#define KEY_LEFT 10 
#define KEY_UP 9
#define KEY_DOWN 13
#define KEY_SHOOT 60
#define KEY_PAUSE 41
// Skip level is L
#define KEY_SKIP_LEVEL 42
// Trainer is T
#define KEY_TRAINER 22



// Joystik definitions.
#define JOYSTIK_REGISTER 0xDC00
#define JOYSTIK_UP 0x01
#define JOYSTIK_DOWN 0x02
#define JOYSTIK_LEFT 0x04
#define JOYSTIK_RIGHT 0x08
#define JOYSTIK_SHOOT 0x10


////////////////////////////////////////////////////////////////////////////////
// Definitions: animation, delays and steps.
////////////////////////////////////////////////////////////////////////////////

// Global step size of Digger and monsters. It is variable based on complexity
// of action on the screen. When scene is complex (more monsters, gold is
// falling etc.) we increase step so that Digger and monsters move more than
// 1 pixel per game loop iteration. This keeps game pace more or less
// constant.
extern uint8_t step;

// Animation counters
#define ANIMATION_STEP 7

#define SEC_BETWEEN_MONSTERS 5

// How many monsters are spawned per level.
extern uint8_t NUM_SPAWNS;

// Calling clock() every iteration of game loop is expensive.
// That's why we call clock() only every CLOCK_STEP iteration.
#define CLOCK_STEP 10

// Global clock. Updated every CLOCK_STEP.
extern clock_t global_clock;

// Number of clocks that bonus mode lasts. 900 = 15 sec * 60
#define BONUS_DURATION_CLK 900

// Number of seconds we show dead digger.
#define DEATH_DELAY_SEC 5

// Number of seconds we play SUCCESS tone.
#define SUCCESS_TONE_SEC 3


////////////////////////////////////////////////////////////////////////////////
// Definitions: bonus mode specific.
////////////////////////////////////////////////////////////////////////////////

#define NUM_KILLS_TO_BONUS 3

#define BONUS_OFF 0
#define BONUS_CAN_HAVE 1
#define BONUS_ON 2

// Global state of bonus: no bonus mode (BONUS_OFF), bonus is placed so that
// Digger can eat it (BONUS_CAN_HAVE) and activated (BONUS_ON).
extern uint8_t bonus_mode_state;

// Per-level, per-digger life number of killed monsters. When it reaches
// NUM_KILLS_TO_BONUS, bonus appears (bonus_mode_state is set to
// BONUS_CAN_HAVE state)
extern uint8_t num_killed_monsters;

// Controls duration of bonus mode. If not BONUS_OFF, is changed after
// BONUS_DURATION_SEC.
extern clock_t last_bonus_clock;


////////////////////////////////////////////////////////////////////////////////
// Definitions: other global definitions.
////////////////////////////////////////////////////////////////////////////////

// Global position of digger is defined as follows:
// digger_x, digger_y are tile-indices, e.g. (4, 7). X is 0..14 and Y is 0..9.
// x_ and y_offset are offsets (positive or negative) from current tile, in
// pixels. When e.g. x_offset reaches 16, it gets reset to 0 and x tile
// coordinate is increased by 1.
// This system has a redundancy, e.g. digger_x=5, digger_x_offset=-4 is the same
// as digger_x=4, digger_x_offset=12. Using plain uint8's and bit
// shifts << 4 to determine a tile might be more efficient, but I didn't have
// resources to explore this.
extern uint8_t digger_x, digger_y;
extern int8_t digger_x_offset, digger_y_offset;

// Indices of Digger sprite 1 and 2 for animation.
// These are made global because Digger can die and they should change outside
// of the main game loop in main().
extern uint8_t sprite_1, sprite_2;

extern bool reset_sprite_after_first_key;

// How many monsters are currently alive.
extern uint8_t num_monsters_alive;

// Number of lives. When 0, digger dies. When >3, we can't draw them properly.
extern uint8_t num_lives;

// Current level 0-254. When reaches 255, game ends.
extern uint8_t current_level;

// Emeralds left till end of level. When 0 we go to next level.
extern uint8_t num_emeralds_left;

// Total score for the game.
extern uint32_t total_score;

// Tile coordinate, where monsters spawn. In original Digger it's top right
// corner always. Here we initialize it in draw_map().
extern uint8_t spawn_x, spawn_y;

// Map elements.
#define MAP_EMPTY 0
#define MAP_MAZE 1
#define MAP_DIGGER 2
#define MAP_SPAWN 3
#define MAP_EMERALD 4
#define MAP_GOLD 5
#define MAP_BROKENGOLD 6


// Current level map. Data is copied here from ALL_MAPS defined in mapcoll.h
extern uint8_t map[][15];

// SID tunes.
#define MUSIC_POPCORN 0
#define MUSIC_DEATH 1
#define MUSIC_SUCCESS 2
#define MUSIC_BONUS 3
#define MUSIC_INTRO 4
#define MUSIC_ENDING 5

// Global index of SID tune currently played.
// After setting this, call play_popcorn() to start playing the music.
extern uint8_t sid_index;


////////////////////////////////////////////////////////////////////////////////
// Definitions: trainer
////////////////////////////////////////////////////////////////////////////////

extern bool unlimited_lives;
extern bool invincibility;
extern bool shoot_without_delay;
extern bool can_skip_levels;
extern uint8_t override_level_start;


////////////////////////////////////////////////////////////////////////////////
// Functions: init.
////////////////////////////////////////////////////////////////////////////////

// Configures sprites mode & color.
extern void setup_sprites(void);
extern void prepare_game(bool reset_all, bool advance_to_next_level);


////////////////////////////////////////////////////////////////////////////////
// Functions: input.
////////////////////////////////////////////////////////////////////////////////

// KERNAL function GETIN - get a character from default input device.
#if __CC65__ < 0x2F0
extern uint8_t __fastcall__ (*cbm_k_getin)(void);
#endif

extern uint8_t poll_keyboard(void);
extern uint8_t poll_joystik(void);

////////////////////////////////////////////////////////////////////////////////
// Functions: drawing / rendering.
////////////////////////////////////////////////////////////////////////////////

// Clears the screen.
extern void clear_screen(void);

// Draws the maze tile.
// x [0-14] and y [0-9] are the tile coordinates.
extern void draw_tile(uint8_t x, uint8_t y);

// Draws the emerald tile.
// x [0-14] and y [0-9] are the tile coordinates.
extern void draw_emerald(uint8_t x, uint8_t y);

// Draws the gold tile.
// x [0-14] and y [0-9] are the tile coordinates.
// When is_wobbled is true, draws the variation of gold tile that
// is 1px shifted.
extern void draw_gold(uint8_t x, uint8_t y, bool is_wobbled);

// Draws the gold on black tile.
// These appear when a gold falls down by 1 square - it doesn't break then.
// x [0-14] and y [0-9] are the tile coordinates.
// When is_wobbled is true, draws the variation of gold tile that
// is 1px shifted.
extern void draw_gold_on_blank(uint8_t x, uint8_t y, bool is_wobbled);

// Draws the broken gold tile.
// These appear when a gold falls down and breaks.
// x [0-14] and y [0-9] are the tile coordinates.
extern void draw_brokengold(uint8_t x, uint8_t y);

// Draws the bonus tile.
// Appears in bonus_mode_state = CAN_HAVE
// x [0-14] and y [0-9] are the tile coordinates.
extern void draw_bonus(uint8_t x, uint8_t y);

// Draws the map stored in map[][] before the game starts.
// Also initializes game global variables:
// - num_emeralds_left
// - all_gold global array (see gold.h)
// - spawn_x and spawn_y
extern void draw_map(void);

// Converts the x [0-14] and y [0-9] tile coordinates to a character offset
// from the screen memory start.
// Returns a uint16 value [0-2000].
extern uint16_t field_xy_to_offset(uint8_t x, uint8_t y);

// Flashes screen with different colors for some fractions of a second.
// Is called when we go to new level, enter bonus mode etc.
extern void flash_screen(void);


////////////////////////////////////////////////////////////////////////////////
// Functions: gameplay control routines.
////////////////////////////////////////////////////////////////////////////////

// Main routine - entry point for the game.
extern int main(void);

// Adjusts game speed (step, bullet_step, gold_falling_step) according
// to the complexity of scene.
extern void adjust_game_step(void);

// Clears the tile with given coords.
extern void clear_tile(uint8_t x, uint8_t y);

// Is called when no more emeralds left and we should go to next level.
extern void level_complete(void);

// "Eats" the tile with given coord.
// This one is called when a Digger "eats" the tile. Internally it calls
// clear_tile and does extra job like updating the score.
extern void eat_tile(uint8_t x, uint8_t y);

// Sets digger position according to digger_(x|y)[_offset].
// Returns true if tile (i.e. digger_x / digger_y) changed place
// as a place result.
extern bool place_digger(void);

// When Digger turns, he is "glued" to the closest column / row of tiles so that
// he never turns "in between" of tiles. This function adjusts digger_x
// when Digger turns up or down, and digger_y - when Digger turns left or right.
extern void adjust_digger_pos(uint8_t last_direction, uint8_t direction);

// Updates digger_x, digger_y, digger_x_offset, digger_y_offset.
// Returns 0 if no movement was performed, 1 if Digger moved, 2 if Digger moved
// to a new tile (i.e. digger_y_offset became 0 and digger_y changed).
extern uint8_t move_digger(uint8_t direction);

// Gets the digger sprite number (DIG_(R|L|U|D)[E]_(1|2)_NUM) based on
// Digger direction and which sprite index (1 or 2) to display.
extern uint8_t get_digger_sprite_num(uint8_t direction, uint8_t sprite_idx);

// This is called when Digger is killed by monster. Can trigger the "game over"
// screen or restart the level depending on how many lives are left.
extern void kill_digger_and_restart(void);


////////////////////////////////////////////////////////////////////////////////
// Functions: music and SFX.
////////////////////////////////////////////////////////////////////////////////

// Starts playing music with number sid_index (set that variable before calling
// this method). The implementation is in popcorn_player.s
extern int play_popcorn();

// The following functions play different SFX. The implementation is in
// popcorn_player.s
extern int play_sfx_eat();
extern int play_sfx_gold_fall();
extern int play_sfx_gold_broken();
extern int play_sfx_eat_gold();


// Reset SID registers. Is useful when we need to interrupt playing of
// e.g. SFX and start playing a music.
extern int sid_reset();

extern void disable_nmi();


#endif