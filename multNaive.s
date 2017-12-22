; Function Name :  multNaive (8x8-bit Multiply, 16-bit result)
; Arguments     :  First multiplier at x, second at y
; Modifies      :  $00 ~ $02
; Returns       :  $00 ~ $01 (little-endian)

; Ideas:
; * Test if fallthrough code for Y register can speed-up operations!

multNaive:
   ; We first clear the space occupied by the temp variables.
  lda #0
  sta $00
  sta $01
  sta $02
   
   ; Now, we iterate this loop for all 8 rounds of multiplication.
  mul_round1:
  tya
  lsr
  tay
  bcc skip_round1
  stx $00
  skip_round1:
  txa
  rol a
  rol $02
  tax

  mul_round2:
  tya
  lsr
  tay
  bcc skip_round2
  txa
  adc $00
  lda $02
  adc $01
  skip_round2:
  txa
  rol a
  rol $02
  tax

  mul_round3:
  tya
  lsr
  tay
  bcc skip_round3
  txa
  adc $00
  lda $02
  adc $01
  skip_round3:
  txa
  rol a
  rol $02
  tax

  mul_round4:
  tya
  lsr
  tay
  bcc skip_round4
  txa
  adc $00
  lda $02
  adc $01
  skip_round4:
  txa
  rol a
  rol $02
  tax

  mul_round5:
  tya
  lsr
  tay
  bcc skip_round5
  txa
  adc $00
  lda $02
  adc $01
  skip_round5:
  txa
  rol a
  rol $02
  tax

  mul_round6:
  tya
  lsr
  tay
  bcc skip_round6
  txa
  adc $00
  lda $02
  adc $01
  skip_round6:
  txa
  rol a
  rol $02
  tax

  mul_round7:
  tya
  lsr
  tay
  bcc skip_round7
  txa
  adc $00
  lda $02
  adc $01
  skip_round7:
  txa
  rol a
  rol $02
  tax

  mul_round8:
  tya
  lsr
  tay
  bcc skip_round8
  txa
  adc $00
  lda $02
  adc $01
  skip_round8:
  rts
