# C64-DIGGER: A Commodore 64 implementation of Digger

**IMPORTANT:** this project is licensed under the MIT License. See LICENSE file in the project root
for full license information. Please retain my copyright and license when using it fully or partially.
Please also let me know that you are going to use it. Thank you for understanding - I am open for
any collaboration!

## Overview

c64-digger is a complete reimplementation, from the ground up, of a famous PC game Digger dated 1983.
Digger was originally created by Windmill software in 1983. As they have never released it for
Commodore, I decided to fill the gap.

## Credits

Intellectual rights for the original game belong to Dude Solutions that acquired Windmill software 
in 2015.

c64-digger has been released by [Hokuto Force](http://www.hokutoforce.c64.org/) on Apr-7, 2018: https://csdb.dk/release/?id=163645

*   Code and graphics: Sergey Borodavkin
*   Music and sound effects: [Uctumi](http://uctumi.com)
*   Intro: [Grass](https://csdb.dk/scener/?id=2189)
*   PM: [The Overkiller](https://csdb.dk/scener/?id=1019)

## Approach 

*  This project tries to resemble the original as close as possible.
*  All game graphics and fonts have been recreated, pixel by pixel, to keep the "feel" of the original game, but not necesserily the "look", mostly due to platform limitations.
*  The code has been written from scratch, in the best effort of reproducing the logic of the original game as
   much as it was possible. However sometimes I decided to take a shortcut.
*  More information is available at https://sborodavkin.tumblr.com/tagged/c64digger   

## Technical Details

* Implemented with 100% C using CC65 toolchain
* Graphics: tiles done in CharPad, sprites - in SpritePad
* Multicolor Character mode
* Digger, monsters and Gold are done with sprite animation so they can move smoothly
* Custom memory layout for linker (sprites and charset graphics are loaded into high memory segments)
* Compileable with CC65 2.13 and 2.16.

## How to Build

You should have cc65 installed. Preferably you'd have Vice installed too, for the c1541 utility to create the D64 disk image but this isn't mandatory.

Run `./compile.sh` or `compile.bat` on the platform of your preference to get Digger compiled into `digger.prg` and [optionally] `digger.d64`.
