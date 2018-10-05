//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include <stdint.h>

#ifndef MAPCOLL_H
#define MAPCOLL_H


// Number of unique levels.
#define NUM_LEVELS 13

// All levels data. A 3D array [NUM_LEVELS][10][15].
// Refer to mem.cfg for the actual memory segment location.
#define ALL_MAPS ((uint8_t*)0xB300)

#endif