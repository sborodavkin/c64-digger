;
; File generated by cc65 v 2.16 - Git N/A
;
	.fopt		compiler,"cc65 v 2.16 - Git N/A"
	.setcpu		"6502"
	.smart		on
	.autoimport	on
	.case		on
	.debuginfo	on
	.importzp	sp, sreg, regsave, regbank
	.importzp	tmp1, tmp2, tmp3, tmp4, ptr1, ptr2, ptr3, ptr4
	.macpack	longbranch
	.dbg		file, "bullet.c", 5181, 1522415071
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/peekpoke.h", 3108, 1519303288
	.dbg		file, "bullet.h", 1935, 1522415059
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/stdint.h", 6196, 1519303289
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/stdbool.h", 2506, 1519303289
	.dbg		file, "digger.h", 13311, 1522743878
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/cbm.h", 10858, 1519303281
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/stddef.h", 3057, 1519303289
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/c64.h", 6729, 1519303281
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/_vic2.h", 10835, 1519303279
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/_sid.h", 3626, 1519303278
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/_6526.h", 3962, 1519303277
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/cbm_filetype.h", 4949, 1519303282
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/time.h", 5672, 1519303291
	.dbg		file, "monster.h", 4509, 1522770787
	.dbg		file, "non_gameplay.h", 2508, 1522153567
	.dbg		sym, "clock", "00", extern, "_clock"
	.dbg		sym, "bonus_mode_state", "00", extern, "_bonus_mode_state"
	.dbg		sym, "num_killed_monsters", "00", extern, "_num_killed_monsters"
	.dbg		sym, "last_bonus_clock", "00", extern, "_last_bonus_clock"
	.dbg		sym, "digger_x", "00", extern, "_digger_x"
	.dbg		sym, "digger_y", "00", extern, "_digger_y"
	.dbg		sym, "num_monsters_alive", "00", extern, "_num_monsters_alive"
	.dbg		sym, "total_score", "00", extern, "_total_score"
	.dbg		sym, "spawn_x", "00", extern, "_spawn_x"
	.dbg		sym, "spawn_y", "00", extern, "_spawn_y"
	.dbg		sym, "map", "00", extern, "_map"
	.dbg		sym, "shoot_without_delay", "00", extern, "_shoot_without_delay"
	.dbg		sym, "draw_bonus", "00", extern, "_draw_bonus"
	.dbg		sym, "adjust_game_step", "00", extern, "_adjust_game_step"
	.dbg		sym, "monsters", "00", extern, "_monsters"
	.dbg		sym, "check_bullet_collision_with_monster", "00", extern, "_check_bullet_collision_with_monster"
	.dbg		sym, "statusbar_redraw_score", "00", extern, "_statusbar_redraw_score"
	.export		_bullet_x
	.export		_bullet_y
	.export		_bullet_x_offset
	.export		_bullet_y_offset
	.export		_bullet_direction
	.export		_bullet_step
	.export		_is_bullet
	.export		_can_shoot
	.export		_shoot_bullet
	.export		_set_most_significant_bullet_x_coord
	.export		_set_bullet_sprite
	.export		_update_bullet
	.export		_check_bullet_collision_with_map
	.import		_clock
	.import		_bonus_mode_state
	.import		_num_killed_monsters
	.import		_last_bonus_clock
	.import		_digger_x
	.import		_digger_y
	.import		_num_monsters_alive
	.import		_total_score
	.import		_spawn_x
	.import		_spawn_y
	.import		_map
	.import		_shoot_without_delay
	.import		_draw_bonus
	.import		_adjust_game_step
	.import		_monsters
	.import		_check_bullet_collision_with_monster
	.import		_statusbar_redraw_score

.segment	"BSS"

.segment	"BSS"
_bullet_x:
	.res	1,$00
.segment	"BSS"
_bullet_y:
	.res	1,$00
.segment	"BSS"
_bullet_x_offset:
	.res	1,$00
.segment	"BSS"
_bullet_y_offset:
	.res	1,$00
.segment	"BSS"
_bullet_direction:
	.res	1,$00
.segment	"BSS"
_bullet_step:
	.res	1,$00
.segment	"BSS"
_is_bullet:
	.res	1,$00
.segment	"BSS"
_can_shoot:
	.res	1,$00

; ---------------------------------------------------------------
; void __near__ shoot_bullet (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_shoot_bullet: near

	.dbg	func, "shoot_bullet", "00", extern, "_shoot_bullet"

.segment	"CODE"

	.dbg	line, "bullet.c", 64
	lda     _shoot_without_delay
	bne     L006D
	.dbg	line, "bullet.c", 65
	sta     _can_shoot
	.dbg	line, "bullet.c", 67
L006D:	lda     _is_bullet
	.dbg	line, "bullet.c", 68
	beq     L016B
	.dbg	line, "bullet.c", 98
	rts
	.dbg	line, "bullet.c", 70
L016B:	lda     #$01
	sta     _is_bullet
	.dbg	line, "bullet.c", 71
	lda     _bullet_direction
	.dbg	line, "bullet.c", 96
	cmp     #$09
	beq     L0167
	cmp     #$0A
	beq     L0169
	cmp     #$0D
	beq     L0168
	cmp     #$12
	beq     L016A
	rts
	.dbg	line, "bullet.c", 73
L0167:	lda     _digger_x
	sta     _bullet_x
	.dbg	line, "bullet.c", 74
	lda     #$00
	sta     _bullet_x_offset
	.dbg	line, "bullet.c", 75
	lda     _digger_y
	sec
	sbc     #$01
	sta     _bullet_y
	.dbg	line, "bullet.c", 76
	lda     #$04
	sta     _bullet_y_offset
	.dbg	line, "bullet.c", 77
	rts
	.dbg	line, "bullet.c", 79
L0168:	lda     _digger_x
	sta     _bullet_x
	.dbg	line, "bullet.c", 80
	lda     #$00
	sta     _bullet_x_offset
	.dbg	line, "bullet.c", 81
	lda     _digger_y
	clc
	adc     #$01
	sta     _bullet_y
	.dbg	line, "bullet.c", 82
	lda     #$00
	sta     _bullet_y_offset
	.dbg	line, "bullet.c", 83
	rts
	.dbg	line, "bullet.c", 85
L0169:	lda     _digger_y
	sta     _bullet_y
	.dbg	line, "bullet.c", 86
	lda     #$00
	sta     _bullet_y_offset
	.dbg	line, "bullet.c", 87
	lda     _digger_x
	sec
	sbc     #$01
	sta     _bullet_x
	.dbg	line, "bullet.c", 88
	lda     #$04
	.dbg	line, "bullet.c", 89
	jmp     L0166
	.dbg	line, "bullet.c", 91
L016A:	lda     _digger_y
	sta     _bullet_y
	.dbg	line, "bullet.c", 92
	lda     #$00
	sta     _bullet_y_offset
	.dbg	line, "bullet.c", 93
	lda     _digger_x
	clc
	adc     #$01
	sta     _bullet_x
	.dbg	line, "bullet.c", 94
	lda     #$00
L0166:	sta     _bullet_x_offset
	.dbg	line, "bullet.c", 98
	rts
	.dbg	line

.endproc

; ---------------------------------------------------------------
; void __near__ set_most_significant_bullet_x_coord (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_set_most_significant_bullet_x_coord: near

	.dbg	func, "set_most_significant_bullet_x_coord", "00", extern, "_set_most_significant_bullet_x_coord"
	.dbg	sym, "is_set", "00", auto, 0
	.dbg	sym, "mask", "00", auto, -1

.segment	"CODE"

	.dbg	line, "bullet.c", 189
	jsr     pusha
	.dbg	line, "bullet.c", 190
	lda     #$04
	jsr     pusha
	.dbg	line, "bullet.c", 191
	ldy     #$01
	lda     (sp),y
	bne     L013E
	.dbg	line, "bullet.c", 192
	dey
	lda     (sp),y
	eor     #$FF
	sta     (sp),y
	.dbg	line, "bullet.c", 193
	and     $D010
	.dbg	line, "bullet.c", 194
	jmp     L016E
	.dbg	line, "bullet.c", 195
L013E:	dey
	lda     (sp),y
	ora     $D010
L016E:	sta     $D010
	.dbg	line, "bullet.c", 197
	jmp     incsp2
	.dbg	line

.endproc

; ---------------------------------------------------------------
; void __near__ set_bullet_sprite (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_set_bullet_sprite: near

	.dbg	func, "set_bullet_sprite", "00", extern, "_set_bullet_sprite"
	.dbg	sym, "is_enable", "00", auto, 0
	.dbg	sym, "mask", "00", auto, -1

.segment	"CODE"

	.dbg	line, "bullet.c", 199
	jsr     pusha
	.dbg	line, "bullet.c", 200
	lda     #$04
	jsr     pusha
	.dbg	line, "bullet.c", 201
	ldy     #$01
	lda     (sp),y
	bne     L0153
	.dbg	line, "bullet.c", 202
	dey
	lda     (sp),y
	eor     #$FF
	sta     (sp),y
	.dbg	line, "bullet.c", 203
	and     $D015
	.dbg	line, "bullet.c", 204
	jmp     L0171
	.dbg	line, "bullet.c", 205
L0153:	dey
	lda     (sp),y
	ora     $D015
L0171:	sta     $D015
	.dbg	line, "bullet.c", 207
	jmp     incsp2
	.dbg	line

.endproc

; ---------------------------------------------------------------
; void __near__ update_bullet (unsigned char)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_update_bullet: near

	.dbg	func, "update_bullet", "00", extern, "_update_bullet"
	.dbg	sym, "cycle_counter", "00", auto, 0
	.dbg	sym, "y_screen", "00", auto, -1
	.dbg	sym, "x_screen", "00", auto, -3
	.dbg	sym, "monster_idx", "00", auto, -4

.segment	"CODE"

	.dbg	line, "bullet.c", 100
	jsr     pusha
	.dbg	line, "bullet.c", 105
	jsr     decsp4
	lda     _is_bullet
	.dbg	line, "bullet.c", 106
	bne     L018C
	.dbg	line, "bullet.c", 187
	jmp     incsp5
	.dbg	line, "bullet.c", 110
L018C:	jsr     _check_bullet_collision_with_monster
	ldy     #$00
	sta     (sp),y
	.dbg	line, "bullet.c", 111
	cmp     #$FF
	beq     L00A9
	.dbg	line, "bullet.c", 112
	tya
	jsr     _set_bullet_sprite
	.dbg	line, "bullet.c", 113
	inc     _num_killed_monsters
	lda     _num_killed_monsters
	.dbg	line, "bullet.c", 114
	cmp     #$03
	bcc     L00AD
	lda     _bonus_mode_state
	cmp     #$02
	beq     L00AD
	.dbg	line, "bullet.c", 115
	lda     #$01
	sta     _bonus_mode_state
	.dbg	line, "bullet.c", 116
	lda     _spawn_x
	jsr     pusha
	lda     _spawn_y
	jsr     _draw_bonus
	.dbg	line, "bullet.c", 117
	jsr     _clock
	sta     _last_bonus_clock
	stx     _last_bonus_clock+1
	ldy     sreg
	sty     _last_bonus_clock+2
	ldy     sreg+1
	sty     _last_bonus_clock+3
	.dbg	line, "bullet.c", 119
L00AD:	ldx     #$00
	lda     (sp,x)
	jsr     pusha0
	lda     #$0B
	jsr     tosmula0
	clc
	adc     #<(_monsters)
	sta     ptr1
	txa
	adc     #>(_monsters)
	sta     ptr1+1
	lda     #$00
	ldy     #$0A
	sta     (ptr1),y
	.dbg	line, "bullet.c", 120
	lda     _num_monsters_alive
	sec
	sbc     #$01
	sta     _num_monsters_alive
	.dbg	line, "bullet.c", 121
	jsr     _adjust_game_step
	.dbg	line, "bullet.c", 122
	ldy     #<(_total_score)
	sty     ptr1
	ldy     #>(_total_score)
	lda     #$FA
	jsr     laddeqa
	.dbg	line, "bullet.c", 123
	jsr     _statusbar_redraw_score
	.dbg	line, "bullet.c", 124
	lda     #$00
	sta     _is_bullet
	.dbg	line, "bullet.c", 125
	jmp     incsp5
	.dbg	line, "bullet.c", 128
L00A9:	jsr     _check_bullet_collision_with_map
	tax
	beq     L0178
	.dbg	line, "bullet.c", 129
	lda     #$00
	jsr     _set_bullet_sprite
	.dbg	line, "bullet.c", 130
	lda     #$00
	sta     _is_bullet
	.dbg	line, "bullet.c", 131
	jmp     incsp5
	.dbg	line, "bullet.c", 135
L0178:	lda     _bullet_direction
	.dbg	line, "bullet.c", 168
	cmp     #$09
	beq     L0179
	cmp     #$0A
	jeq     L0181
	cmp     #$0D
	beq     L017D
	cmp     #$12
	jeq     L0185
	jmp     L0189
	.dbg	line, "bullet.c", 137
L0179:	lda     _bullet_y_offset
	bpl     L00D0
	dex
L00D0:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L00D1
	dex
L00D1:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L00D2
	eor     #$80
L00D2:	asl     a
	ldx     #$00
	bcc     L017A
	.dbg	line, "bullet.c", 138
	lda     _bullet_y
	sec
	sbc     #$01
	sta     _bullet_y
	.dbg	line, "bullet.c", 139
	stx     _bullet_y_offset
	.dbg	line, "bullet.c", 140
	jmp     L0189
	.dbg	line, "bullet.c", 141
L017A:	lda     _bullet_step
	cmp     #$80
	eor     #$FF
	sec
	adc     _bullet_y_offset
	sta     _bullet_y_offset
	jpl     L0189
	.dbg	line, "bullet.c", 143
	jmp     L0189
	.dbg	line, "bullet.c", 145
L017D:	lda     _bullet_y_offset
	bpl     L00E0
	dex
L00E0:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L00E1
	dex
L00E1:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L00E2
	eor     #$80
L00E2:	asl     a
	ldx     #$00
	bcc     L017E
	.dbg	line, "bullet.c", 146
	lda     _bullet_y
	clc
	adc     #$01
	sta     _bullet_y
	.dbg	line, "bullet.c", 147
	stx     _bullet_y_offset
	.dbg	line, "bullet.c", 148
	jmp     L0189
	.dbg	line, "bullet.c", 149
L017E:	lda     _bullet_step
	cmp     #$80
	clc
	adc     _bullet_y_offset
	sta     _bullet_y_offset
	jpl     L0189
	.dbg	line, "bullet.c", 151
	jmp     L0189
	.dbg	line, "bullet.c", 153
L0181:	lda     _bullet_x_offset
	bpl     L00F0
	dex
L00F0:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L00F1
	dex
L00F1:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L00F2
	eor     #$80
L00F2:	asl     a
	ldx     #$00
	bcc     L0182
	.dbg	line, "bullet.c", 154
	lda     _bullet_x
	sec
	sbc     #$01
	sta     _bullet_x
	.dbg	line, "bullet.c", 155
	txa
	.dbg	line, "bullet.c", 156
	jmp     L018B
	.dbg	line, "bullet.c", 157
L0182:	lda     _bullet_step
	cmp     #$80
	eor     #$FF
	sec
	adc     _bullet_x_offset
	sta     _bullet_x_offset
	bpl     L0189
	.dbg	line, "bullet.c", 159
	jmp     L0189
	.dbg	line, "bullet.c", 161
L0185:	lda     _bullet_x_offset
	bpl     L0100
	dex
L0100:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L0101
	dex
L0101:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L0102
	eor     #$80
L0102:	asl     a
	ldx     #$00
	bcc     L0186
	.dbg	line, "bullet.c", 162
	lda     _bullet_x
	clc
	adc     #$01
	sta     _bullet_x
	.dbg	line, "bullet.c", 163
	txa
	.dbg	line, "bullet.c", 164
	jmp     L018B
	.dbg	line, "bullet.c", 165
L0186:	lda     _bullet_step
	cmp     #$80
	clc
	adc     _bullet_x_offset
L018B:	sta     _bullet_x_offset
	.dbg	line, "bullet.c", 170
L0189:	lda     _bullet_y
	jsr     shlax4
	clc
	adc     #$47
	bcc     L010E
	inx
L010E:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_y_offset
	bpl     L010F
	dex
L010F:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	pla
	ldy     #$03
	sta     (sp),y
	.dbg	line, "bullet.c", 171
	ldx     #$00
	lda     _bullet_x
	jsr     shlax4
	clc
	adc     #$3C
	bcc     L0112
	inx
L0112:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_x_offset
	bpl     L0113
	dex
L0113:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	clc
	adc     #$01
	bcc     L0114
	inx
L0114:	ldy     #$01
	jsr     staxysp
	.dbg	line, "bullet.c", 173
	cpx     #$01
	bcs     L0115
	.dbg	line, "bullet.c", 174
	ldy     #$01
	lda     (sp),y
	sta     $D004
	.dbg	line, "bullet.c", 175
	lda     #$00
	.dbg	line, "bullet.c", 176
	jmp     L0173
	.dbg	line, "bullet.c", 177
L0115:	ldy     #$01
	lda     (sp),y
	sta     $D004
	.dbg	line, "bullet.c", 178
	tya
L0173:	jsr     _set_most_significant_bullet_x_coord
	.dbg	line, "bullet.c", 180
	ldy     #$03
	lda     (sp),y
	sta     $D005
	.dbg	line, "bullet.c", 181
	lda     #$01
	jsr     _set_bullet_sprite
	.dbg	line, "bullet.c", 182
	ldy     #$04
	lda     (sp),y
	and     #$03
	cmp     #$02
	bcs     L018A
	.dbg	line, "bullet.c", 183
	lda     #$C0
	.dbg	line, "bullet.c", 184
	jmp     L0174
	.dbg	line, "bullet.c", 185
L018A:	lda     #$C1
L0174:	sta     $83FA
	.dbg	line, "bullet.c", 187
	jmp     incsp5
	.dbg	line

.endproc

; ---------------------------------------------------------------
; unsigned char __near__ check_bullet_collision_with_map (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_check_bullet_collision_with_map: near

	.dbg	func, "check_bullet_collision_with_map", "00", extern, "_check_bullet_collision_with_map"

.segment	"CODE"

	.dbg	line, "bullet.c", 23
	ldx     #$00
	lda     _bullet_direction
	cmp     #$09
	jne     L01A1
	.dbg	line, "bullet.c", 25
	lda     _bullet_y
	beq     L0199
	cmp     #$0A
	bcs     L0199
	.dbg	line, "bullet.c", 26
	cmp     #$01
	bne     L019D
	lda     _bullet_y_offset
	bpl     L0010
	dex
L0010:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L0011
	dex
L0011:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L0012
	eor     #$80
L0012:	asl     a
	ldx     #$00
	bcc     L019D
	.dbg	line, "bullet.c", 27
L0199:	lda     #$01
	rts
	.dbg	line, "bullet.c", 29
L019D:	lda     _bullet_y
	sec
	sbc     #$02
	bcs     L0019
	dex
L0019:	jsr     pushax
	lda     #$0F
	jsr     tosmula0
	clc
	adc     #<(_map)
	sta     ptr1
	txa
	adc     #>(_map)
	sta     ptr1+1
	ldy     _bullet_x
	ldx     #$00
	lda     (ptr1),y
	.dbg	line, "bullet.c", 30
	jeq     L01C8
	lda     _bullet_y_offset
	bpl     L001C
	dex
L001C:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L001D
	dex
L001D:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L001E
	eor     #$80
L001E:	bmi     L01A0
	jmp     L01C8
	.dbg	line, "bullet.c", 31
L01A0:	ldx     #$00
	lda     #$01
	rts
	.dbg	line, "bullet.c", 33
L01A1:	lda     _bullet_direction
	cmp     #$0D
	jne     L01AE
	.dbg	line, "bullet.c", 34
	lda     _bullet_y
	cmp     #$09
	bcs     L01A6
	cmp     #$08
	bne     L01AA
	lda     _bullet_y_offset
	bpl     L0028
	dex
L0028:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L0029
	dex
L0029:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L002A
	eor     #$80
L002A:	asl     a
	ldx     #$00
	bcc     L01AA
	.dbg	line, "bullet.c", 35
L01A6:	lda     #$01
	rts
	.dbg	line, "bullet.c", 37
L01AA:	lda     _bullet_y
	clc
	adc     #$01
	bcc     L0031
	inx
L0031:	jsr     pushax
	lda     #$0F
	jsr     tosmula0
	clc
	adc     #<(_map)
	sta     ptr1
	txa
	adc     #>(_map)
	sta     ptr1+1
	ldy     _bullet_x
	ldx     #$00
	lda     (ptr1),y
	.dbg	line, "bullet.c", 38
	jeq     L01C8
	lda     _bullet_y_offset
	bpl     L0034
	dex
L0034:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L0035
	dex
L0035:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L0036
	eor     #$80
L0036:	bmi     L01AD
	jmp     L01C8
	.dbg	line, "bullet.c", 39
L01AD:	ldx     #$00
	lda     #$01
	rts
	.dbg	line, "bullet.c", 41
L01AE:	lda     _bullet_direction
	cmp     #$0A
	jne     L01BB
	.dbg	line, "bullet.c", 43
	lda     _bullet_x
	beq     L01B3
	cmp     #$0F
	bcs     L01B3
	.dbg	line, "bullet.c", 44
	cmp     #$01
	bne     L01B7
	lda     _bullet_x_offset
	bpl     L0040
	dex
L0040:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L0041
	dex
L0041:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L0042
	eor     #$80
L0042:	asl     a
	ldx     #$00
	bcc     L01B7
	.dbg	line, "bullet.c", 45
L01B3:	lda     #$01
	rts
	.dbg	line, "bullet.c", 47
L01B7:	lda     _bullet_y
	jsr     pusha0
	lda     #$0F
	jsr     tosmula0
	clc
	adc     #<(_map)
	sta     ptr1
	txa
	adc     #>(_map)
	sta     ptr1+1
	ldx     #$00
	lda     _bullet_x
	sec
	sbc     #$02
	bcs     L004A
	dex
L004A:	clc
	adc     ptr1
	sta     ptr1
	txa
	adc     ptr1+1
	sta     ptr1+1
	ldx     #$00
	lda     (ptr1,x)
	.dbg	line, "bullet.c", 48
	jeq     L01C8
	lda     _bullet_x_offset
	bpl     L004C
	dex
L004C:	jsr     pushax
	ldx     #$00
	lda     _bullet_step
	bpl     L004D
	dex
L004D:	jsr     tossubax
	cmp     #$F1
	txa
	sbc     #$FF
	bvc     L004E
	eor     #$80
L004E:	bmi     L01BA
	jmp     L01C8
	.dbg	line, "bullet.c", 49
L01BA:	ldx     #$00
	lda     #$01
	rts
	.dbg	line, "bullet.c", 51
L01BB:	lda     _bullet_direction
	cmp     #$12
	jne     L01C8
	.dbg	line, "bullet.c", 52
	lda     _bullet_x
	cmp     #$0E
	bcs     L01C0
	cmp     #$0D
	bne     L01C4
	lda     _bullet_x_offset
	bpl     L0058
	dex
L0058:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L0059
	dex
L0059:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L005A
	eor     #$80
L005A:	asl     a
	ldx     #$00
	bcc     L01C4
	.dbg	line, "bullet.c", 53
L01C0:	lda     #$01
	rts
	.dbg	line, "bullet.c", 55
L01C4:	lda     _bullet_y
	jsr     pusha0
	lda     #$0F
	jsr     tosmula0
	clc
	adc     #<(_map)
	sta     ptr1
	txa
	adc     #>(_map)
	sta     ptr1+1
	ldx     #$00
	lda     _bullet_x
	clc
	adc     #$01
	bcc     L0194
	inx
	clc
L0194:	adc     ptr1
	sta     ptr1
	txa
	adc     ptr1+1
	sta     ptr1+1
	ldx     #$00
	lda     (ptr1,x)
	.dbg	line, "bullet.c", 56
	beq     L01C8
	lda     _bullet_x_offset
	bpl     L0064
	dex
L0064:	sta     ptr1
	stx     ptr1+1
	ldx     #$00
	lda     _bullet_step
	bpl     L0065
	dex
L0065:	clc
	adc     ptr1
	pha
	txa
	adc     ptr1+1
	tax
	pla
	cmp     #$10
	txa
	sbc     #$00
	bvs     L0066
	eor     #$80
L0066:	bmi     L01C7
	jmp     L01C8
	.dbg	line, "bullet.c", 57
L01C7:	ldx     #$00
	lda     #$01
	rts
	.dbg	line, "bullet.c", 60
L01C8:	lda     _bullet_y
	jsr     pusha0
	lda     #$0F
	jsr     tosmula0
	clc
	adc     #<(_map)
	sta     ptr1
	txa
	adc     #>(_map)
	tax
	ldy     _bullet_x
	stx     ptr1+1
	lda     (ptr1),y
	jmp     boolne
	.dbg	line

.endproc
