; Function Name :  multNaive (8x8-bit Multiply, 16-bit result)
; Arguments     :  First multiplier at x, second at y
; Modifies      :  $00 ~ $03
; Returns       :  $00 ~ $01 (little-endian)

BASE $200

multNaive:
   ; We first clear the space occupied by the temp variables.
  lda #0
  sta $0
  sta $1
  sta $2
  lda #8
  sta $3

  ; Now, we iterate this loop for all 8 rounds of multiplication.
  @mul_round:
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
    beq @endMult
    dec $3
    bne @mul_round
  @endMult:
  rts