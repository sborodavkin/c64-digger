;  
; Copyright (c) Sergey Borodavkin. All rights reserved.  
; Licensed under the MIT License. See LICENSE file in the project root
; for full license information.  
;

.importzp  sp
.import   _sid_index
.export   _play_popcorn
.export   _play_sfx_eat
.export   _play_sfx_gold_fall
.export   _play_sfx_gold_broken
.export   _play_sfx_eat_gold
.export   _sid_reset

; SID banks 1 & 2 addresses. See mem.cfg.
sid_init = $C000
sid_play = $C003
siddata = $C000

sid2_init = $A200
sid2_play = $A203
sid2data = $A200

; C64-specific IRQ control registers.
REG_RASTERLINE = $D012
REG_SCREENCTL_1 = $D011
REG_INSTATUS_1 = $DC0D
REG_INSTATUS_2 = $DD0D
REG_INTCONTROL = $D01A
REG_INTFLAG = $D019


.segment "CODE"

; Initializes SID and starts playing music set in sid_index.
.proc _play_popcorn: near        
        ldx #00               ; Reset CPU registers.
        ldy #00        
        lda _sid_index        ; See digger.h for list of tunes.
        cmp #03               ; sid_index >= 3 means SID bank 2.
        bmi bank_1
        sec
        sbc #03               ; Subtract 3 to get actual SID tune index in bank 2.
        jsr sid2_init
        sei
        lda #$7f
        sta REG_INSTATUS_1
        sta REG_INSTATUS_2
        and REG_SCREENCTL_1
        sta REG_SCREENCTL_1
        ldy #40               ; Music plays at raster line 40.
        sty REG_RASTERLINE
        lda #<_int2_music     ; Point IRQ vector to our custom IRQ routine.
        ldx #>_int2_music
        jmp cleanup
bank_1:
        jsr sid_init          ; This is identical to block above.
        sei
        lda #$7f
        sta REG_INSTATUS_1
        sta REG_INSTATUS_2
        and REG_SCREENCTL_1
        sta REG_SCREENCTL_1
        ldy #40  
        sty REG_RASTERLINE
        lda #<_int_music      ; Banks 1 & 2 have different IRQ handlers.
        ldx #>_int_music
cleanup:
        sta $314
        stx $315
        lda #$01
        sta REG_INTCONTROL
        cli                   ; Clear interrupt disable flag.
        rts
.endproc 


; ---------------------------------------------------------------
; IRQ handlers.
; ---------------------------------------------------------------

; IRQ handler for SID bank 2.
.proc _int2_music    
        jsr sid2_play
        asl REG_INTFLAG       ; Accept the interrupt.
        jmp $EA31             ; Do the normal interrupt service routine
.endproc

; IRQ handler for SID bank 1.
.proc _int_music   
        jsr sid_play  
        asl REG_INTFLAG
        jmp $EA31
.endproc


; ---------------------------------------------------------------
; SFX playback.
; ---------------------------------------------------------------

; SFX: eat emerald.
.proc _play_sfx_eat  
        lda _sid_index        ; Music from which bank is currently playing?
        cmp #03
        bmi sfxeatbank_1
        lda #<(_sfx_eat_data)         ; Start address of sound effect data
        ldy #>(_sfx_eat_data)
        ldx #14               ; Channel num: 0, 7 or 14 for channels 1-3.
        jsr sid2_play+3       ; If music from bank 2 is playing.
        rts
sfxeatbank_1:
        lda #<(_sfx_eat_data)        
        ldy #>(_sfx_eat_data)
        ldx #14
        jsr sid_play+3        ; If music from bank 1 is playing.
        rts
.endproc

; SFX: gold falls.
; See comments in _play_sfx_eat above.
.proc _play_sfx_gold_fall
        lda _sid_index
        cmp #03
        bmi sfxgoldfallbank_1
        lda #<(_sfx_gold_fall_data)
        ldy #>(_sfx_gold_fall_data)
        ldx #14
        jsr sid2_play+3
        rts
sfxgoldfallbank_1:
        lda #<(_sfx_gold_fall_data)
        ldy #>(_sfx_gold_fall_data)
        ldx #14
        jsr sid_play+3
        rts
.endproc

; SFX: gold broken.
; See comments in _play_sfx_eat above.
.proc _play_sfx_gold_broken        
        lda _sid_index
        cmp #03
        bmi sfxgoldbrokenbank_1
        lda #<(_sfx_gold_broken_data)
        ldy #>(_sfx_gold_broken_data)
        ldx #14
        bmi sfxgoldbrokenbank_1
        jsr sid2_play+3
        rts
sfxgoldbrokenbank_1:
        lda #<(_sfx_gold_broken_data)
        ldy #>(_sfx_gold_broken_data)
        ldx #14
        jsr sid_play+3
        rts
.endproc

; SFX: digger ate gold.
; See comments in _play_sfx_eat above.
.proc _play_sfx_eat_gold
        lda _sid_index
        cmp #03
        bmi sfxeatgoldbank_1
        lda #<(_sfx_eat_gold_data)
        ldy #>(_sfx_eat_gold_data)
        ldx #14
        bmi sfxeatgoldbank_1
        jsr sid2_play+3
        rts
sfxeatgoldbank_1:
        lda #<(_sfx_eat_gold_data)
        ldy #>(_sfx_eat_gold_data)
        ldx #14
        jsr sid_play+3
        rts
.endproc


; ---------------------------------------------------------------
; SID control.
; ---------------------------------------------------------------

; Reset SID registers (e.g. to stop SFX playback before playing a music).
.proc _sid_reset
        ldx #24
        lda #0
clear_loop:
        sta $d400, x
        dex
        bne clear_loop
        rts
.endproc


.segment "DATA"


; ---------------------------------------------------------------
; SFX - as they're small we just provide byte sequences for them.
; ---------------------------------------------------------------

_sfx_eat_data:
        .byte $00, $C7, $00, $9A, $11, $9A, $B2, $B2, $BE, $BE, $10, $00

_sfx_gold_fall_data:
        .byte $00, $F8, $00, $D2, $11, $D1, $D0, $CF, $CE, $CD, $CC, $CB
        .byte $CA, $C9, $C8, $C7, $C6, $C5, $C4, $10, $C3, $C2, $C0, $BF
        .byte $BE, $BD, $BC, $BB, $BA, $B9, $00

_sfx_gold_broken_data:
        .byte $00, $F8, $00, $A8, $11, $A5, $A0, $A6, $80, $00

_sfx_eat_gold_data:
        .byte $00, $F8, $00, $96, $11, $96, $9F, $9F, $A5, $A5, $A9, $A9
        .byte $AF, $AF, $B5, $B5, $BA, $BA, $BE, $BE, $C4, $10, $00