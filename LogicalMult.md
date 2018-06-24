# Logical Analysis on *How to make a hardware multiplier* and *Implementing it in Software*

The idea to bring a hardware multiplier to the software world is essentially taking a MIMD approach to multiplication: for the 6502, we would be multiplying 8 pairs of 8-bit numbers with 8 16-bit results, all in parallel, instead of a single one; if the total time is less than eight times the number of cycles required to do a single 8x8->16 bit multiply, we have a winner.

## 1x1->1 bit

In this case, we have a single bit as output; the logical function *AND* suffices to make a multiplier.

| AND Gate | B = 0 | B = 1 |
| -------- | ----- | ----- |
|  A = 0   |   0   |   0   |
|  A = 1   |   0   |   1   |

## 2x2->4 bit

For this, we resort to the school algorithm: multiplying all 4 possible partial products, summing and shifting along the way when when necessary. The optimal way of doing this in circuitry is with the [Dadda multiplier](https://en.wikipedia.org/wiki/Binary_multiplier), which we can convert down to a series of sequential instructions in [SSA form](https://en.wikipedia.org/wiki/Static_single_assignment_form), then reduce the number of registers needed (down to five, in this case). It doesn't actually matter if each register contains a single or multiple bits; the same operation will be performed in all of them, performing this 2x2 multiplication in parallel to *all bits*. What matters is *interlacing* the bits along the input registers, and likewise *deinterlacing* the output, much like a chunky-to-planar conversion and back (if you've ever heard of planar graphics, like the Amiga OCS and PC EGA).

### SSA Form

Inputs are R0, R1, R2, and R3, containing A0, A1, B0, and B1, respectively. Ampersand ("&") denotes *logical AND*, pipe ("|") denotes *logical OR*, and hat ("^") denotes *logical XOR*.

| Original SSA  | Register Optimized | Order Optimized |
| ------------- | ------------------ | --------------- |
| R4 = R0 & R2  |    R4 = R0 & R2    |  R4 = R0 & R2   |
| R5 = R1 & R2  |    R2 = R1 & R2    |  R2 = R1 & R2   |
| R6 = R0 & R3  |    R0 = R0 & R3    |  R1 = R1 & R3   |
| R7 = R1 & R3  |    R1 = R1 & R3    |  R0 = R0 & R3   |
| R8 = R5 & R6  |    R3 = R2 & R0    |  R3 = R2 & R0   |
| R9 = R5 ^ R6  |    R2 = R2 ^ R0    |  R2 = R2 ^ R0   |
| R10 = R7 & R9 |    R0 = R1 & R2    |  R0 = R1 & R2   |
| R11 = R7 ^ R9 |    R2 = R1 ^ R2    |  R2 = R1 ^ R2   |

The outputs are in [R4 R8 R10 R11] for the original SSA, or [R4 R3 R0 R2] for the optimized lists. The second pass (order optimization) is needed because the 6502 needs two cycles to read "registers" (zero-page memory contents, or the first 256 bytes in RAM) into the accumulator; if we can reuse results from the immediate last operation, we save those cycles.

## 4x4->8 bit

Using the same recurrence equation, we can try assembling an optimized circuit that will allow us to do the next step in the operation. This one is a bit more involved, and requires us to optimize the circuit beforehand - either by sheer paper, pencil and will; or with a Karnaugh map. I chose by paper, which... [is a bit messy, but works](someimagewithcircuit).

### SSA Form

Inputs are R0 ~ R3 for A operand, and R4 ~ R7 for B operand.

| Original SSA  | Register Optimized | Order Optimized |
| ------------- | ------------------ | --------------- |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |
|  |  |  |

Outputs are [R?? R]