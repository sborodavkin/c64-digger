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
	.dbg		file, "charset.c", 2914, 1522759564
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/stdint.h", 6196, 1519303289
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/string.h", 5041, 1519303290
	.dbg		file, "/usr/local/google/home/sborodavkin/cc65-2.16/include/stddef.h", 3057, 1519303289
	.dbg		file, "charset.h", 7790, 1523024027
	.dbg		file, "mapcoll.h", 441, 1523025000
	.dbg		sym, "memcpy", "00", extern, "_memcpy"
	.import		_memcpy
	.export		_LEVEL_ART
	.export		_LEVEL_ART_PEN_COLOR
	.export		_load_wobble_gold_chars
	.export		_unload_wobble_gold_chars
	.export		_backup_alphabet

.segment	"DATA"

_LEVEL_ART:
	.byte	$00
	.byte	$01
	.byte	$00
	.byte	$01
	.byte	$0F
	.byte	$10
	.byte	$11
	.byte	$12
	.byte	$71
	.byte	$72
	.byte	$73
	.byte	$74
	.byte	$0B
	.byte	$0C
	.byte	$0D
	.byte	$0E
	.byte	$38
	.byte	$39
	.byte	$3A
	.byte	$3B
	.byte	$3C
	.byte	$3D
	.byte	$3E
	.byte	$3F
	.byte	$75
	.byte	$76
	.byte	$77
	.byte	$78
	.byte	$40
	.byte	$41
	.byte	$42
	.byte	$43
	.byte	$44
	.byte	$45
	.byte	$44
	.byte	$45
	.byte	$46
	.byte	$47
	.byte	$48
	.byte	$49
	.byte	$79
	.byte	$7A
	.byte	$7B
	.byte	$7C
	.byte	$4A
	.byte	$4B
	.byte	$4C
	.byte	$4D
	.byte	$4E
	.byte	$4F
	.byte	$4F
	.byte	$4E
	.byte	$50
	.byte	$51
	.byte	$52
	.byte	$53
	.byte	$7D
	.byte	$7E
	.byte	$7F
	.byte	$80
	.byte	$54
	.byte	$55
	.byte	$56
	.byte	$57
	.byte	$58
	.byte	$59
	.byte	$59
	.byte	$5A
	.byte	$5B
	.byte	$5C
	.byte	$5D
	.byte	$5E
	.byte	$81
	.byte	$82
	.byte	$83
	.byte	$84
	.byte	$5F
	.byte	$60
	.byte	$61
	.byte	$62
	.byte	$63
	.byte	$63
	.byte	$64
	.byte	$64
	.byte	$65
	.byte	$66
	.byte	$67
	.byte	$68
	.byte	$85
	.byte	$86
	.byte	$87
	.byte	$88
	.byte	$69
	.byte	$6A
	.byte	$6B
	.byte	$6C
_LEVEL_ART_PEN_COLOR:
	.byte	$0F
	.byte	$0F
	.byte	$0F
	.byte	$0F
	.byte	$0E
	.byte	$0F

; ---------------------------------------------------------------
; void __near__ load_wobble_gold_chars (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_load_wobble_gold_chars: near

	.dbg	func, "load_wobble_gold_chars", "00", extern, "_load_wobble_gold_chars"

.segment	"CODE"

	.dbg	line, "charset.c", 45
	ldy     #$00
L0071:	lda     $B080,y
	sta     $8B88,y
	iny
	cpy     #$E0
	bne     L0071
	.dbg	line, "charset.c", 46
	rts
	.dbg	line

.endproc

; ---------------------------------------------------------------
; void __near__ unload_wobble_gold_chars (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_unload_wobble_gold_chars: near

	.dbg	func, "unload_wobble_gold_chars", "00", extern, "_unload_wobble_gold_chars"

.segment	"CODE"

	.dbg	line, "charset.c", 52
	ldy     #$00
L0079:	lda     $B160,y
	sta     $8B88,y
	iny
	cpy     #$E0
	bne     L0079
	.dbg	line, "charset.c", 53
	rts
	.dbg	line

.endproc

; ---------------------------------------------------------------
; void __near__ backup_alphabet (void)
; ---------------------------------------------------------------

.segment	"CODE"

.proc	_backup_alphabet: near

	.dbg	func, "backup_alphabet", "00", extern, "_backup_alphabet"

.segment	"CODE"

	.dbg	line, "charset.c", 58
	ldy     #$00
L0081:	lda     $8B88,y
	sta     $B160,y
	iny
	cpy     #$E0
	bne     L0081
	.dbg	line, "charset.c", 59
	rts
	.dbg	line

.endproc
