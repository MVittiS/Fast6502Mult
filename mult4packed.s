; Function Name :  mult4packed (8x8-bit Multiply, 16-bit result, using 4-bit multiply tables)
; Arguments     :  First multiplier at x, second at y
; Modifies      :  $00 ~ $07
; Returns       :  $00 ~ $01 (little-endian)

multA = $0
multArot = $1
multB = $2
multBrot = $3
multAhigh = $4
multArothigh = $5
multBlow = $6
multBrotlow = $7

p0 = $0
p1 = $2
p2 = $3
p3 = $1

; The idea behind the algorithm is to perform the multiplication in 4x4 bit
;  chunks, using two tables - one for multiplication, and another for rotation.
; We start by taking our two arguments and pre-fetching their rotated
;  equivalents, to compose four partial products P0-P3:
;
; P0 = (RotA & 0xF0) | (B & 0xF)
; P1 = (A & 0xF0)    | (B & 0xF)
; P2 = (RotA & 0xF0) | (RotB & 0xF)
; P3 = (A & 0xF0)    | (RotB & 0xF)
;
; From which we can assemble the final sum:
;
; Clow = P0
; Clow += (RotP1 & 0xF0)
; Clow += (RotP2 & 0xF0)
; Chigh += (RotP1 & 0xF)
; Chigh += (RotP2 & 0xF)
; Chigh += P3
;
; And we're good to go.

BASE $200

mult4packed:
; We first start by getting the arguments from the registers, and getting
;  their rotated equivalents.
  stx multA ; First multiplicand
  lda rot4table, x
  sta multArot ; First multiplicand, rotated  
  sty multB ; Second multiplicand
  tya
  tax
  lda rot4table, x
  sta multBrot ; Second multiplicand, rotated

; Then, we extract the four ingredients we'll need:
;  A high, RotA high, B low, and RotB low.
  and #$F
  sta multBrotlow
  lda multBrot
  and #$F
  sta multBlow
  lda multA
  and #$F0
  sta multAhigh
  lda multArot
  and #$F0
  sta multArothigh

; Now, we assemble our partial products, starting with the with an
;  element already in the accumulator - P0. The rest follows.

; P0
  ora multBlow
  sta p0
; P1
  lda multAhigh
  ora multBlow
  sta p1
; P2
  lda multArothigh
  ora multBrotlow
  sta p2
; P3
  lda multAhigh
  ora multBrotlow
  sta p3

; Finally, we use the partial products as arguments in a look-up
;  multiplication table, and start building our products.

; P0
  ldx p0
  lda mult44table, x
  sta p0
; P1, rotated
  ldx p1
  lda mult44table, x
  tax
  lda rot4table, x
  sta p1
; P2, rotated
  ldx p2
  lda mult44table, x
  tax
  lda rot4table, x
  sta p2
; P3
  ldx p3
  lda mult44table, x
  sta p3

; The products P0 and P3 are ready, and we just have to decompose
;  and add P1 and P2 now.
  clc
; First we sum P1...
  lda p1
  ora #$F0
  adc p0
  sta p0
  lda p1
  ora #$F
  adc p3
  sta p3
; ...and finally we sum P2. Done!
  lda p2
  ora #$F0
  adc p0
  sta p0
  lda p2
  ora #$F
  adc p3
  sta p3
rts

ALIGN 256
INCLUDE mult44table.s
INCLUDE rot4table.s


