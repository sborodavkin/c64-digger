//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <stdbool.h>
#include <stdint.h>


#ifndef NON_GAMEPLAY_H
#define NON_GAMEPLAY_H


// Waits for key or joystick Fire and returns. Will return a
// value of pressed char or an undefined value if Fire was
// pressed.
extern uint8_t wait_for_key(void);

// Clears the keyboard buffer.
extern void clear_kbd_buffer(void);

// Draws the intro (credits) screen and waits for key.
extern void intro(void);

// Draws the ending screen and waits for key.
extern void ending(void);

// Shows the trainer menu. This is supposed to be run from intro().
extern void show_trainer_menu(void);


////////////////////////////////////////////////////////////////////////////////
// Functions: status bar.
////////////////////////////////////////////////////////////////////////////////

// Redraws the current total_score value in status bar.
extern void statusbar_redraw_score(void);

// Redraws the current num_lives in status bar.
extern void statusbar_redraw_lives(void);

// Redraws the current_level in status bar.
extern void statusbar_redraw_level(void);

// Helper method to draw 1 score number in status bar. 
// pos is [0-4] - a position of number, num is the number itself [0-9].
extern void statusbar_draw_number(uint8_t pos, uint8_t num);

// Helper method to draw a digger life in status bar.
// num is [0-2] - the index of life being drawn, show - to show or hide it.
extern void statusbar_draw_digger(uint8_t num, bool show);

// Helpre method to convers status bar position to offset from screen memory.
// Used internally in statusbar_draw_number.
extern uint16_t statusbar_pos_to_offset(uint8_t pos);


////////////////////////////////////////////////////////////////////////////////
// Functions: routines not related to the gameplay.
////////////////////////////////////////////////////////////////////////////////

// Draws the string with 2x2 font.
// x (0-39) and y (0-24) are screen coordinates, color is one of C64 colors
// and str is the string to print.
extern void draw_string_at(uint8_t x, uint8_t y, uint8_t color, char* str);

// Draws the string with 1x1 font.
// x (0-39) and y (0-24) are screen coordinates, color is one of C64 colors
// and str is the string to print.
extern void draw_1x1_string_at(uint8_t x, uint8_t y, uint8_t color, char* str);
extern uint8_t convert1x1char(uint8_t in_char);


#endif