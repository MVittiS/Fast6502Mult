main:
  ldx #3
loop:
  lda $fe
  sta $0, x
  dex
  bpl loop
  jsr mult22

  ldx #4
loop2:
  lda $fe
  sta $200, x
  dex
  bpl loop2
  rts

mult22par:
; R4 = R0 & R2
  lda 0
  and 2
  sta 4
; R2 = R1 & R2
  lda 2
  and 1
  sta 2
; R0 = R0 & R3
  lda 0
  and 3
  sta 0
; R1 = R1 & R3
  lda 1
  and 3
  sta 1
; R3 = R2 & R0
  lda 0
  and 2
  sta 3
; R2 = R2 ^ R0
  lda 2
  eor 0
  sta 2
; R0 = R1 & R2
  and 1
  sta 0
; R2 = R1 ^ R2
  lda 2
  eor 1
  sta 2
  rts