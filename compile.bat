@echo off
cc65 -Oirs -t c64 -g mapcoll.c
ca65 mapcoll.s

cc65 -Oirs -t c64 -g monster.c
ca65 monster.s

cc65 -Oirs -t c64 -g gold.c
ca65 gold.s

cc65 -Oirs -t c64 -g charset.c
ca65 charset.s
                         
cc65 -Oirs -t c64 -g digger.c
ca65 digger.s

cc65 -Oirs -t c64 -g bullet.c
ca65 bullet.s

cc65 -Oirs -t c64 -g non_gameplay.c
ca65 non_gameplay.s

ca65 gfx_sound_data.s
ca65 gfx_sound_data_2.s

ca65 -g popcorn_player.s
ca65 -g disable_nmi.s

ld65 -C mem.cfg -m mem.out -Ln label.out -o digger.prg^
    digger.o monster.o non_gameplay.o gold.o charset.o mapcoll.o^
    bullet.o popcorn_player.o gfx_sound_data.o gfx_sound_data_2.o^
    disable_nmi.o c64.lib

del digger.d64
c1541 -format "digger,1" d64 digger.d64 -attach digger.d64 -write digger.prg digger
