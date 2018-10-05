//  
// Copyright (c) Sergey Borodavkin. All rights reserved.  
// Licensed under the MIT License. See LICENSE file in the project root
// for full license information.  
//

#include "mapcoll.h"

// There was plenty of stuff but now it's empty.
// Reason is that BSS segment got full so I moved all
// levels data to levels.bin, which is linked through
// gfx_sound_data.s directly into the memory.