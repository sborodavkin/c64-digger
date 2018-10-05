;  
; Copyright (c) Sergey Borodavkin. All rights reserved.  
; Licensed under the MIT License. See LICENSE file in the project root
; for full license information.  
;

; This code can be find in many places on the Internet.
; It turns off RESTORE key by disabling the NMI.


.export _disable_nmi

.segment "CODE"

.proc _disable_nmi: near
  lda #<nmi             ;Set NMI vector 
  sta $0318 
  sta $fffa 
  lda #>nmi 
  sta $0319 
  sta $fffb 
  lda #$81 
  sta $dd0d             ;Use Timer A 
  lda #$01              ;Timer A count ($0001) 
  sta $dd04 
  lda #$00 
  sta $dd05 
  lda #%00011001        ;Run Timer A 
  sta $dd0e 
  rts 
.endproc  

nmi:
  rti ;; exit interrupt not acknowledged 