; Function Name :  multNaiveUnrolled (8x8-bit Multiply, 16-bit result)
; Arguments     :  First multiplier at x, second at y
; Modifies      :  $00 ~ $02
; Returns       :  $00 ~ $01 (little-endian)

; Ideas:
; * Test if fallthrough code for Y register can speed-up operations!

BASE $200

multNaiveUnrolled:
   ; We first clear the space occupied by the temp variables.
  lda #0
  sta $00
  sta $01
  sta $02
   
   ; Now, we iterate this loop for all 8 rounds of multiplication.
  REPT 8
    tya
    lsr
    tay
    bcc @skip_round
    txa
    clc
    adc $0
    sta $0
    lda $2
    adc $1
    sta $1
    @skip_round:
    txa
    rol a
    rol $2
    tax
    ;tya
    ;beq endMult
  ENDR
  endMult:
  rts
