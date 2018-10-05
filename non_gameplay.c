//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <peekpoke.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "charset.h"
#include "non_gameplay.h"
#include "digger.h"


uint8_t wait_for_key(void) {
  uint8_t c;
  uint8_t joystik_mask;
  cbm_k_chkin (0);
  c = cbm_k_getin();
  joystik_mask = PEEK(JOYSTIK_REGISTER);
  while (1) {
    c = cbm_k_getin();
    joystik_mask = PEEK(JOYSTIK_REGISTER);
    if (c) { break; }
    if (!(joystik_mask & JOYSTIK_SHOOT)) { break; }
  }
  return c;
}

void clear_kbd_buffer() {
  uint8_t c = 0;
  // Clear keyboard buffer.
  cbm_k_chkin (0);
  c = cbm_k_getin();
  while (c) {
    cbm_k_chkin (0);
    c = cbm_k_getin();
  }
}

void intro(void) {
  uint8_t c, joystik_mask;
  uint8_t cycle_counter;
  uint8_t last_color = COLOR_WHITE;
  uint8_t y_scroll_line = 19;
  uint8_t i = 0;
  uint16_t offset = 0;
  uint16_t screen_addr;
  uint8_t next_char_index = 0;
  bool exit_intro = false;

  char buf[190] = "\"\" STEER WITH JOYSTIK 2 OR W A S D \"\"\
SHOOT WITH FIRE OR SPACE \"\" PRESS P TO PAUSE \"\"\
KILL 3 MONSTERS TO GET BONUS \"\" GET EXTRA LIFE EVERY 3 LEVELS \"\"\
PRESS T NOW FOR TRAINER \"\"\
GOOD LUCK ";

  unload_wobble_gold_chars();

  while (!exit_intro) {  // Top-level loop.
    clear_kbd_buffer();

    // All strings should be UPPER case!
    draw_string_at(15, 0, COLOR_YELLOW, "DIGGER");
    draw_1x1_string_at(6, 4, COLOR_BLUE, "A DIGITAL MONASTERY PRODUCTION");
    draw_1x1_string_at(0, 8, COLOR_VIOLET,  " CODE & GFX");
    draw_1x1_string_at(14, 8, COLOR_GREEN, "SERGEY \"LW0000\" BORODAVKIN");
    draw_1x1_string_at(0, 10, COLOR_VIOLET, "MUSIC & SFX");
    draw_1x1_string_at(14, 10, COLOR_GREEN, "UCTUMI");
    draw_1x1_string_at(0, 12, COLOR_VIOLET, "      INTRO");
    draw_1x1_string_at(14, 12, COLOR_GREEN, "DAVID \"GRASS\" VIGH");

    draw_1x1_string_at(3, 22, COLOR_BLUE, " GAME BY SERGEY \"LW0000\" BORODAVKIN");
    draw_1x1_string_at(3, 23, COLOR_BLUE, "          ORIGINAL PC GAME");
    draw_1x1_string_at(3, 24, COLOR_BLUE, "      @ WINDMILL SOFTWARE 1983");

    offset = y_scroll_line * 40;
    // Prefill color RAM with scroll text color for the whole line.
    for (i = 0; i < 40; i++) {
      POKE(COLOR + offset + i, COLOR_CYAN);
    }

    while (!exit_intro) {  // Wait for Fire loop.
      cycle_counter++;
      if (cycle_counter % CLOCKS_PER_SEC == 0) {
        last_color = (last_color == COLOR_WHITE ? COLOR_YELLOW : COLOR_WHITE);
        draw_1x1_string_at(2, 16, last_color, "P R E S S   F I R E   T O   S T A R T");
      }
      if (cycle_counter % CLOCKS_PER_SEC * 2 == 0) {
        screen_addr = SCREEN + offset;
        for(i = 1; i < 40; i++) {
          screen_addr += 1;
          POKE(screen_addr - 1, PEEK(screen_addr));

        }
        POKE(screen_addr, convert1x1char(buf[next_char_index++]));
        if (next_char_index >= strlen(buf)) {
          next_char_index = 0;
        }
      }

      c = poll_keyboard();
      joystik_mask = PEEK(JOYSTIK_REGISTER);
      if (c == KEY_SHOOT) {
        exit_intro = true;
        break;
      }
      if (!(joystik_mask & JOYSTIK_SHOOT)) {
        exit_intro = true;
        break;
      }
      if (c == KEY_TRAINER) {
        // "T" key pressed, entering trainer menu
        show_trainer_menu();
        clear_kbd_buffer();
        exit_intro = false;
        next_char_index = 0;
        break;  // We break Wait for Fire loop and end up in top-;evel loop.
      }
    }
  }
}

void ending(void) {
  uint8_t c, joystik_mask;
  uint8_t cycle_counter;
  uint8_t last_color = COLOR_WHITE;
  char score[6];

  sid_index = MUSIC_ENDING;
  play_popcorn();  

  clear_screen();
  clear_kbd_buffer();

  unload_wobble_gold_chars();

  draw_string_at(6, 0, 0x0F, "CONGRATULATIONS");
  draw_1x1_string_at(8, 5, COLOR_WHITE, "YOU HAVE NAVIGATED DIGGER");
  draw_1x1_string_at(9, 8, COLOR_WHITE, "THROUGH ALL");
  draw_string_at(21, 7, 0x0B, "255");
  draw_1x1_string_at(28, 8, COLOR_WHITE, "LEVELS");
  draw_1x1_string_at(5, 11, COLOR_WHITE, "AND COLLECTED EMERALDS AND GOLD");
  draw_1x1_string_at(6, 14, COLOR_WHITE, "FOR MANY GENERATIONS TO COME");

  draw_1x1_string_at(3, 17, COLOR_WHITE, "YOUR FINAL SCORE WAS ");
  sprintf(score, "%06lu", total_score);
  draw_string_at(24, 16, 0x0B, score);

  draw_bonus(7, 8);

  //draw_1x1_string_at(2, 7, COLOR_GREEN, "255 LEVELS AND COLLECTED ENOUGH");
  //draw_1x1_string_at(2, 9, COLOR_GREEN, "EMERALDS FOR ALL GENERATIONS");


  while (1) {
    cycle_counter++;
    if (cycle_counter % CLOCKS_PER_SEC == 0) {
      last_color = (last_color == COLOR_WHITE ? COLOR_YELLOW : COLOR_WHITE);
      draw_1x1_string_at(4, 23, last_color, "PRESS SPACE OR FIRE TO PLAY AGAIN");
    }
    c = poll_keyboard();
    joystik_mask = PEEK(JOYSTIK_REGISTER);
    if (c == KEY_SHOOT) { break; }
    if (!(joystik_mask & JOYSTIK_SHOOT)) { break; }
  }

  sid_index = MUSIC_POPCORN;
  play_popcorn();  
}

void show_trainer_menu(void) {
  uint8_t joystik_mask, c, cycle_counter, last_color, i;
  char level[4] = "001";
  uint16_t parsed_level;
  char will_start_from[25];

  unload_wobble_gold_chars();
  clear_screen();
  draw_string_at(9, 0, COLOR_WHITE, "TRAINER MENU");
  draw_1x1_string_at(2, 8, COLOR_BLUE, "UNLIMITED LIVES \"Y\" OR \"N\"");
  c = 0;
  while (c != 'y' && c != 'n') {
    c = wait_for_key();
  }
  draw_1x1_string_at(37, 8, COLOR_WHITE, (c == 'y') ? "Y" : "N");
  unlimited_lives = (c == 'y');

  draw_1x1_string_at(2, 10, COLOR_BLUE, "INVINCIBILITY \"Y\" OR \"N\"");
  c = 0;
  while (c != 'y' && c != 'n') {
    c = wait_for_key();
  }
  draw_1x1_string_at(37, 10, COLOR_WHITE, (c == 'y') ? "Y" : "N");
  invincibility = (c == 'y');

  draw_1x1_string_at(2, 12, COLOR_BLUE, "SHOOT WITHOUT DELAY \"Y\" OR \"N\"");
  c = 0;
  while (c != 'y' && c != 'n') {
    c = wait_for_key();
  }
  draw_1x1_string_at(37, 12, COLOR_WHITE, (c == 'y') ? "Y" : "N");
  shoot_without_delay = (c == 'y');

  draw_1x1_string_at(2, 14, COLOR_BLUE, "PRESS \"L\" TO SKIP LEVEL \"Y\" OR \"N\"");
  c = 0;
  while (c != 'y' && c != 'n') {
    c = wait_for_key();
  }
  draw_1x1_string_at(37, 14, COLOR_WHITE, (c == 'y') ? "Y" : "N");
  can_skip_levels = (c == 'y');

  draw_1x1_string_at(2, 16, COLOR_BLUE, "START LEVEL 001 TO 255 THEN \"Y\"");
  c = 0;
  i = 0;
  while (1) {
    draw_1x1_string_at(37, 16, COLOR_WHITE, level);
    c = wait_for_key();
    if (c >= '0' && c <= '9') {
      level[i++] = c;
      draw_1x1_string_at(37, 16, COLOR_WHITE, level);
      if (i > 2) {
        i = 0;
      }
    }
    if (c == 'y') {
      break;
    }    
  }
  parsed_level = atoi(level);
  if (parsed_level == 0 || parsed_level > 255) {
    parsed_level = 1;
  }
  sprintf(will_start_from, "WILL START FROM %03d", parsed_level);  
  draw_1x1_string_at(2, 18, COLOR_WHITE, will_start_from);
  override_level_start = parsed_level - 1;  // Real level number is 0-254.

  cycle_counter = 0;
  while (1) {
    cycle_counter++;
    if (cycle_counter % CLOCKS_PER_SEC == 0) {
      last_color = (last_color == COLOR_WHITE ? COLOR_YELLOW : COLOR_WHITE);
      draw_1x1_string_at(2, 23, last_color, "PRESS FIRE OR SPACE TO RETURN TO INTRO");
    }
    c = poll_keyboard();
    joystik_mask = PEEK(JOYSTIK_REGISTER);
    if (c == KEY_SHOOT) { break; }
    if (!(joystik_mask & JOYSTIK_SHOOT)) { break; }
  }
  clear_screen();
}

void statusbar_redraw_score(void) {
  uint8_t i = 0;
  char buffer[7];
  sprintf(buffer, "%06lu", total_score);
  for(; i < 6; i++) {
    statusbar_draw_number(i, buffer[i] - 48);
  }
}

void statusbar_redraw_level(void) {
  uint8_t i = 0;
  char buffer[6];
  sprintf(buffer, "L%03d", current_level + 1);
  draw_string_at(27, 0, 0x0B, buffer);
}

void statusbar_redraw_lives(void) {
  uint8_t i = 0;
  for(; i < 5; i++) {
    statusbar_draw_digger(i, false);
  }
  for(i = 0; i < num_lives; i++) {
    statusbar_draw_digger(i, true);
  }
}


// Draws a number in the status bar as a part of current score.
void statusbar_draw_number(uint8_t pos, uint8_t num) {
  uint8_t char1, char2, char3, char4;
  uint16_t offset = statusbar_pos_to_offset(pos);
  uint16_t offset2 = offset + 40;
  switch(num) {
    case 0:
      char1 = CHAR_0_1;
      char2 = CHAR_0_2;
      char3 = CHAR_0_3;
      char4 = CHAR_0_4;
      break;
    case 1:
      char1 = CHAR_1_1;
      char2 = CHAR_1_2;
      char3 = CHAR_1_3;
      char4 = CHAR_1_4;
      break;
    case 2:
      char1 = CHAR_2_1;
      char2 = CHAR_2_2;
      char3 = CHAR_2_3;
      char4 = CHAR_2_4;
      break;
    case 3:
      char1 = CHAR_3_1;
      char2 = CHAR_3_2;
      char3 = CHAR_3_3;
      char4 = CHAR_3_4;
      break;
    case 4:
      char1 = CHAR_4_1;
      char2 = CHAR_4_2;
      char3 = CHAR_4_3;
      char4 = CHAR_4_4;
      break;
    case 5:
      char1 = CHAR_5_1;
      char2 = CHAR_5_2;
      char3 = CHAR_5_3;
      char4 = CHAR_5_4;
      break;
    case 6:
      char1 = CHAR_6_1;
      char2 = CHAR_6_2;
      char3 = CHAR_6_3;
      char4 = CHAR_6_4;
      break;
    case 7:
      char1 = CHAR_7_1;
      char2 = CHAR_7_2;
      char3 = CHAR_7_3;
      char4 = CHAR_7_4;
      break;
    case 8:
      char1 = CHAR_8_1;
      char2 = CHAR_8_2;
      char3 = CHAR_8_3;
      char4 = CHAR_8_4;
      break;
    case 9:
      char1 = CHAR_9_1;
      char2 = CHAR_9_2;
      char3 = CHAR_9_3;
      char4 = CHAR_9_4;
      break;
  }

  POKE(SCREEN + offset, char1);
  POKE(COLOR + offset, 0x0F);
  POKE(SCREEN + (++offset), char2);
  POKE(COLOR + offset, 0x0F);

  POKE(SCREEN + offset2, char3);
  POKE(COLOR + offset2, 0x0F);
  POKE(SCREEN + (++offset2), char4);
  POKE(COLOR + offset2, 0x0F);
}

// Draws a digger life.
// Pos values are: 7: life 1; pos = 8: life 2; pos = 9: life 3.
void statusbar_draw_digger(uint8_t num, bool show) {
  uint8_t char1, char2, char3, char4;
  uint16_t offset = 16 + 3 * num + 2;
  uint16_t offset2 = offset + 40;
  if (show) {
    char1 = CHAR_DGR_1;
    char2 = CHAR_DGR_2;
    char3 = CHAR_DGR_3;
    char4 = CHAR_DGR_4;
  } else {
    char1 = CHAR_BLANK;
    char2 = char1;
    char3 = char1;
    char4 = char1;
  }
  POKE(SCREEN + offset, char1);
  POKE(COLOR + offset, 0x0F);
  POKE(SCREEN + (++offset), char2);
  POKE(COLOR + offset, 0x0F);
  POKE(SCREEN + offset2, char3);
  POKE(COLOR + offset2, 0x0F);
  POKE(SCREEN + (++offset2), char4);
  POKE(COLOR + offset2, 0x0F);
}

// Converts given tile coords into screen memory offset.
uint16_t statusbar_pos_to_offset(uint8_t pos) {
  // 80 is 40*2
  // 5 is free chars to the left of field.
  return 5 + 2 * pos;
}

void draw_string_at(uint8_t x, uint8_t y, uint8_t color, char *str) {
  uint8_t char1, char2, char3, char4;  // char index in charset
  uint16_t offset = x + y * 40;   // offset in screen RAM.
  uint8_t alphabet_offset = CHARSET_SIZE / 8;
  uint8_t i = 0;
  for(; i < strlen(str); i++) {
    if (str[i] >= 65) {
      char1 = 4 * (str[i] - 65);
      POKE(SCREEN + offset, alphabet_offset + char1);
      POKE(COLOR + offset, color);
      POKE(SCREEN + offset + 1, alphabet_offset + char1 + 1);
      POKE(COLOR + offset + 1, color);
      POKE(SCREEN + offset + 40, alphabet_offset + char1 + 2);
      POKE(COLOR + offset + 40, color);
      POKE(SCREEN + offset + 41, alphabet_offset + char1 + 3);
      POKE(COLOR + offset + 41, color);
    } else if (str[i] >= 48 && str[i] <= 57) {
      switch(str[i] - 48) {
        case 0:
          char1 = CHAR_0_1;
          char2 = CHAR_0_2;
          char3 = CHAR_0_3;
          char4 = CHAR_0_4;
          break;
        case 1:
          char1 = CHAR_1_1;
          char2 = CHAR_1_2;
          char3 = CHAR_1_3;
          char4 = CHAR_1_4;
          break;
        case 2:
          char1 = CHAR_2_1;
          char2 = CHAR_2_2;
          char3 = CHAR_2_3;
          char4 = CHAR_2_4;
          break;
        case 3:
          char1 = CHAR_3_1;
          char2 = CHAR_3_2;
          char3 = CHAR_3_3;
          char4 = CHAR_3_4;
          break;
        case 4:
          char1 = CHAR_4_1;
          char2 = CHAR_4_2;
          char3 = CHAR_4_3;
          char4 = CHAR_4_4;
          break;
        case 5:
          char1 = CHAR_5_1;
          char2 = CHAR_5_2;
          char3 = CHAR_5_3;
          char4 = CHAR_5_4;
          break;
        case 6:
          char1 = CHAR_6_1;
          char2 = CHAR_6_2;
          char3 = CHAR_6_3;
          char4 = CHAR_6_4;
          break;
        case 7:
          char1 = CHAR_7_1;
          char2 = CHAR_7_2;
          char3 = CHAR_7_3;
          char4 = CHAR_7_4;
          break;
        case 8:
          char1 = CHAR_8_1;
          char2 = CHAR_8_2;
          char3 = CHAR_8_3;
          char4 = CHAR_8_4;
          break;
        case 9:
          char1 = CHAR_9_1;
          char2 = CHAR_9_2;
          char3 = CHAR_9_3;
          char4 = CHAR_9_4;
          break;
      }
      POKE(SCREEN + offset, char1);
      POKE(COLOR + offset, color);
      POKE(SCREEN + offset + 1, char2);
      POKE(COLOR + offset + 1, color);
      POKE(SCREEN + offset + 40, char3);
      POKE(COLOR + offset + 40, color);
      POKE(SCREEN + offset + 41, char4);
      POKE(COLOR + offset + 41, color);
    } else {
      char1 = CHAR_BLANK;
      POKE(SCREEN + offset, char1);
      POKE(COLOR + offset, color);
      POKE(SCREEN + offset + 1, char1);
      POKE(COLOR + offset + 1, color);
      POKE(SCREEN + offset + 40, char1);
      POKE(COLOR + offset + 40, color);
      POKE(SCREEN + offset + 41, char1);
      POKE(COLOR + offset + 41, color);
    }
    offset = offset + 2;
  }
}

uint8_t convert1x1char(uint8_t in_char) {
  uint8_t out_char;
  uint8_t font_offset = (CHARSET_SIZE + CHARSET_ALPHABET_SIZE) / 8;
  if (in_char >= 'A' && in_char <= 'Z') { // A-Z
    out_char = font_offset + in_char - 'A';
  } else if (in_char >= '0' && in_char <= '9') {
    out_char = 29 + font_offset + in_char - '0';
  } else {
    // Brutal switch for each of remaining chars ;)
    switch (in_char) {
      case '\"':
        out_char = font_offset + 26;
        break;
      case '&':
        out_char = font_offset + 27;
        break;
      case '@':
        out_char = font_offset + 28;
        break;   // @ replaces copyright sign.      
      default:
        out_char = CHAR_BLANK;
        break;
    }
  }
  return out_char;
}

void draw_1x1_string_at(uint8_t x, uint8_t y, uint8_t color, char *str) {
  uint16_t offset = x + y * 40;   // offset in screen RAM.
  uint8_t i = 0;
  for(; i < strlen(str); i++) {
    POKE(SCREEN + offset, convert1x1char(str[i]));
    POKE(COLOR + offset++, color);
  }
}


