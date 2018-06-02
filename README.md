Fast6502Mult
============

This project aims to assess the fastest way of multiplying two numbers on the 6502 architecture, on a standard, flat 64k address space (so no pre-calculated 8x8 multiplication table, though other kinds of tables are taken into account). Contibutions and suggestions are more than welcome!

The speed will be judged initially by the average number of cycles required to perform all possible multiplications (so 64k for an 8x8 or 4.4x4.4 bit, 16M for 8.8x8.8 or 4.12x4.12 bit, and so on). If we can also manage to generate more accurate timing information, the decision criterion will later be changed to the analysis of histogram plots for the number of cycles. There are plans for later extensions to fixed-point math and perhaps some small, practical 3D demos using the [6502asm](www.6502asm.com) platform. Speed is a priority, but of course **correctness is a must**. Speed-size tradeoffs will be studied later, especially in regards to unrolled vs. looped code.

Current project status
----------------------

| 8x8->16 bit   | Avg. Cycles  | Max Cycles   | Min Cycles   | Size [bytes] |
| ------------- |:------------:|:------------:|:------------:|:------------:|
| Naive Implementation | 317 | 389 | 245 | 39 |
| Naive Implementation w/ Early Exit | 305.25 | 409 | 41 | 42 |
| Naive Implementation Unrolled | 249 | 321 | 177 | 185 |
| Swapping argument with less zeroes using table | ?? | ?? | ?? | 512 |
| 4x4 bit precalc + rotation table | 174 | 174 | 174 | 768 |

TODOs
-----

[X] Settle on a toolkit for command-line compiling and testing code - Decided on [asm6](http://www.romhacking.net/utilities/674/) for assembly and [fake6502](http://rubbermallet.org/fake6502.c) for emulation
[ ] Preparing R script for automatically generating histograms with tests
[ ] 4.4x4.4 -> 4.4 multiply
[ ] 8.8x8.8 -> 8.8 multiply
[ ] 4.12x4.12 -> 4.12 multiply
[ ] 4.4x0.8 -> 4.4 multiply
[ ] Sin/Cos table
[ ] Cube point table
[ ] Teapot point table
[ ] (crackpot idea) 8-way parallel multiply, implementing logic gates with instructions - start with 2x2x8, then 4x4x8, to finally 8x8x8. *Might* be faster than 8 separate multiplies…?
