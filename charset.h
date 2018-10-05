//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <stdint.h>

#ifndef CHARSET_H
#define CHARSET_H


////////////////////////////////////////////////////////////////////////////////
// Definitions: global charset defs.
////////////////////////////////////////////////////////////////////////////////

#define CHARSET_START 0x8800
#define GOLD_WOBBLE_CHARSET 0xB080
#define GOLD_WOBBLE_CHARSET_SIZE 224

// How many bytes the game charset takes without 2x2 alphabet
// and 1x1 alphabet.
#define CHARSET_SIZE 904

// How many bytes the 2x2 alphabet takes.
#define CHARSET_ALPHABET_SIZE 832

// How many bytes the 1x1 alphabet takes.
#define CHARSET_FONT1_SIZE 312

// We provide 6 decoration sets for levels and rotate between them.
#define NUM_LEVEL_ART 6

// This is 2D array. For each level it contains 12 indices
// that correspond to:
// - 4 char indices for maze tile
// - 4 char indices for gold tile
// - 4 char indices for gold wobbled tile
// - 4 char indices for emerald tile
// This approach allows to have distinct graphics per each level.
extern uint8_t LEVEL_ART[][16];

// Pen color for decorations at given level index.
// On level 5 we use some BLUE (!) color :)
extern uint8_t LEVEL_ART_PEN_COLOR[];

//
// Level 1 decorations.
//

// Tiles (_1, _2, _3, _4) are defined as follows:
// +-----------------+-----------------+
// |                 |                 |
// |                 |                 |
// | CHAR_MAZE_IDX_1 | CHAR_MAZE_IDX_2 |
// |                 |                 |
// |                 |                 |
// +-----------------+-----------------+
// |                 |                 |
// |                 |                 |
// | CHAR_MAZE_IDX_3 | CHAR_MAZE_IDX_4 |
// |                 |                 |
// |                 |                 |
// +-----------------+-----------------+
// Indices of elements inside LEVEL_ART. These elements are actual
// CHAR_MAZE_LVLi_j values. For any level, LEVEL_ART[NUM_LEVEL_ART]
// contains 12 elements.  So CHAR_MAZE_IDX_1 = 0 means that CHARSET index
// of maze tile 1 will be stored in index 0 in LEVEL_ART[NUM_LEVEL_ART].
#define CHAR_MAZE_IDX_1 0
#define CHAR_MAZE_IDX_2 1
#define CHAR_MAZE_IDX_3 2
#define CHAR_MAZE_IDX_4 3
// Gold on maze tile.
#define CHAR_GOLD_IDX_1 4
#define CHAR_GOLD_IDX_2 5
#define CHAR_GOLD_IDX_3 6
#define CHAR_GOLD_IDX_4 7
// Wobbled gold on maze tile.
#define CHAR_GOLD_WBL_IDX_1 8
#define CHAR_GOLD_WBL_IDX_2 9
#define CHAR_GOLD_WBL_IDX_3 10
#define CHAR_GOLD_WBL_IDX_4 11
// Emerald on maze tile.
#define CHAR_EMERALD_IDX_1 12
#define CHAR_EMERALD_IDX_2 13
#define CHAR_EMERALD_IDX_3 14
#define CHAR_EMERALD_IDX_4 15

#define CHAR_BLANK 2
#define CHAR_MAPBORDER_TL 3
#define CHAR_MAPBORDER_T 4
#define CHAR_MAPBORDER_TR 5
#define CHAR_MAPBORDER_R 6
#define CHAR_MAPBORDER_BR 7
#define CHAR_MAPBORDER_B 8
#define CHAR_MAPBORDER_BL 9
#define CHAR_MAPBORDER_L 10

#define CHAR_GOLD_ON_BLANK_1 21
#define CHAR_GOLD_ON_BLANK_2 22
#define CHAR_GOLD_ON_BLANK_3 23
#define CHAR_GOLD_ON_BLANK_4 24

#define CHAR_DGR_1 25
#define CHAR_DGR_2 26
#define CHAR_DGR_3 27
#define CHAR_DGR_4 28

#define CHAR_0_1 29
#define CHAR_0_2 30
#define CHAR_0_3 31
#define CHAR_0_4 32

#define CHAR_1_1 2
#define CHAR_1_2 33
#define CHAR_1_3 34
#define CHAR_1_4 35

#define CHAR_2_1 36
#define CHAR_2_2 37
#define CHAR_2_3 38
#define CHAR_2_4 39

#define CHAR_3_1 36
#define CHAR_3_2 40
#define CHAR_3_3 41
#define CHAR_3_4 42

#define CHAR_4_1 43
#define CHAR_4_2 44
#define CHAR_4_3 45
#define CHAR_4_4 46

#define CHAR_5_1 47
#define CHAR_5_2 48
#define CHAR_5_3 41
#define CHAR_5_4 42

#define CHAR_6_1 47
#define CHAR_6_2 49
#define CHAR_6_3 50
#define CHAR_6_4 42

#define CHAR_7_1 51
#define CHAR_7_2 30
#define CHAR_7_3 2
#define CHAR_7_4 52

#define CHAR_8_1 53
#define CHAR_8_2 40
#define CHAR_8_3 54
#define CHAR_8_4 42

#define CHAR_9_1 29
#define CHAR_9_2 30
#define CHAR_9_3 45
#define CHAR_9_4 55

// Level 1 art.
// Maze tiles are hardcoded in charset.c
#define CHAR_EMERALD_1 11
#define CHAR_EMERALD_2 12
#define CHAR_EMERALD_3 13
#define CHAR_EMERALD_4 14
#define CHAR_GOLD_1 15
#define CHAR_GOLD_2 16
#define CHAR_GOLD_3 17
#define CHAR_GOLD_4 18
#define CHAR_WBL_GOLD_1 113
#define CHAR_WBL_GOLD_2 114
#define CHAR_WBL_GOLD_3 115
#define CHAR_WBL_GOLD_4 116
#define CHAR_BROKEN_GOLD_1 19
#define CHAR_BROKEN_GOLD_2 20

// Level 2 art
#define CHAR_MAZE_LVL2_1 56
#define CHAR_MAZE_LVL2_2 57
#define CHAR_MAZE_LVL2_3 58
#define CHAR_MAZE_LVL2_4 59
#define CHAR_GOLD_LVL2_1 60
#define CHAR_GOLD_LVL2_2 61
#define CHAR_GOLD_LVL2_3 62
#define CHAR_GOLD_LVL2_4 63
#define CHAR_WBL_GOLD_LVL2_1 117
#define CHAR_WBL_GOLD_LVL2_2 118
#define CHAR_WBL_GOLD_LVL2_3 119
#define CHAR_WBL_GOLD_LVL2_4 120
#define CHAR_EMERALD_LVL2_1 64
#define CHAR_EMERALD_LVL2_2 65
#define CHAR_EMERALD_LVL2_3 66
#define CHAR_EMERALD_LVL2_4 67

// Level 3 art
#define CHAR_MAZE_LVL3_1 68
#define CHAR_MAZE_LVL3_2 69
#define CHAR_MAZE_LVL3_3 68
#define CHAR_MAZE_LVL3_4 69
#define CHAR_GOLD_LVL3_1 70
#define CHAR_GOLD_LVL3_2 71
#define CHAR_GOLD_LVL3_3 72
#define CHAR_GOLD_LVL3_4 73
#define CHAR_WBL_GOLD_LVL3_1 121
#define CHAR_WBL_GOLD_LVL3_2 122
#define CHAR_WBL_GOLD_LVL3_3 123
#define CHAR_WBL_GOLD_LVL3_4 124
#define CHAR_EMERALD_LVL3_1 74
#define CHAR_EMERALD_LVL3_2 75
#define CHAR_EMERALD_LVL3_3 76
#define CHAR_EMERALD_LVL3_4 77

// Level 4 art
#define CHAR_MAZE_LVL4_1 78
#define CHAR_MAZE_LVL4_2 79
#define CHAR_MAZE_LVL4_3 79
#define CHAR_MAZE_LVL4_4 78
#define CHAR_GOLD_LVL4_1 80
#define CHAR_GOLD_LVL4_2 81
#define CHAR_GOLD_LVL4_3 82
#define CHAR_GOLD_LVL4_4 83
#define CHAR_WBL_GOLD_LVL4_1 125
#define CHAR_WBL_GOLD_LVL4_2 126
#define CHAR_WBL_GOLD_LVL4_3 127
#define CHAR_WBL_GOLD_LVL4_4 128
#define CHAR_EMERALD_LVL4_1 84
#define CHAR_EMERALD_LVL4_2 85
#define CHAR_EMERALD_LVL4_3 86
#define CHAR_EMERALD_LVL4_4 87

// Level 5 art
#define CHAR_MAZE_LVL5_1 88
#define CHAR_MAZE_LVL5_2 89
#define CHAR_MAZE_LVL5_3 89
#define CHAR_MAZE_LVL5_4 90
#define CHAR_GOLD_LVL5_1 91
#define CHAR_GOLD_LVL5_2 92
#define CHAR_GOLD_LVL5_3 93
#define CHAR_GOLD_LVL5_4 94
#define CHAR_WBL_GOLD_LVL5_1 129
#define CHAR_WBL_GOLD_LVL5_2 130
#define CHAR_WBL_GOLD_LVL5_3 131
#define CHAR_WBL_GOLD_LVL5_4 132
#define CHAR_EMERALD_LVL5_1 95
#define CHAR_EMERALD_LVL5_2 96
#define CHAR_EMERALD_LVL5_3 97
#define CHAR_EMERALD_LVL5_4 98

// Level 6 art
#define CHAR_MAZE_LVL6_1 99
#define CHAR_MAZE_LVL6_2 99
#define CHAR_MAZE_LVL6_3 100
#define CHAR_MAZE_LVL6_4 100
#define CHAR_GOLD_LVL6_1 101
#define CHAR_GOLD_LVL6_2 102
#define CHAR_GOLD_LVL6_3 103
#define CHAR_GOLD_LVL6_4 104
#define CHAR_WBL_GOLD_LVL6_1 133
#define CHAR_WBL_GOLD_LVL6_2 134
#define CHAR_WBL_GOLD_LVL6_3 135
#define CHAR_WBL_GOLD_LVL6_4 136
#define CHAR_EMERALD_LVL6_1 105
#define CHAR_EMERALD_LVL6_2 106
#define CHAR_EMERALD_LVL6_3 107
#define CHAR_EMERALD_LVL6_4 108

// Wobbled gold on black
#define CHAR_WBL_GOLD_BLK_1 137
#define CHAR_WBL_GOLD_BLK_2 138
#define CHAR_WBL_GOLD_BLK_3 139
#define CHAR_WBL_GOLD_BLK_4 140

// Bonus
#define CHAR_BONUS_1 109
#define CHAR_BONUS_2 110
#define CHAR_BONUS_3 111
#define CHAR_BONUS_4 112


////////////////////////////////////////////////////////////////////////////////
// Functions.
////////////////////////////////////////////////////////////////////////////////

// Loads wobbled gold characters into the charset.
extern void load_wobble_gold_chars(void);

// Overwrites wobbled gold characters in the charset with 2x2 alphabet
// chars from the back buffer (previously backed up by backup_alphabet()).
extern void unload_wobble_gold_chars(void);

// Backs up the 2x2 alphabet charset part into back buffer.
extern void backup_alphabet(void);


#endif