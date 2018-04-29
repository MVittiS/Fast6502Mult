#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

uint8_t ram[0x10000];

extern void reset6502();
extern void exec6502(uint32_t tickcount);
extern void step6502();
extern void irq6502();
extern void nmi6502();
extern void hookexternal(void *funcptr);
// CPUstate
extern uint8_t a, x, y, status;
extern uint16_t pc;
extern uint32_t clockticks6502;
extern uint32_t instructions;

uint8_t read6502(uint16_t address) {
    return ram[address];
}

void write6502(uint16_t address, uint8_t value) {
    ram[address] = value;
}

const char opString[][4] = {
/*        |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  A  |  B  |  C  |  D  |  E  |  F  |      */
/* 0 */    "brk","ora","nop","slo","nop","ora","asl","slo","php","ora","asl","nop","nop","ora","asl","slo", /* 0 */
/* 1 */    "bpl","ora","nop","slo","nop","ora","asl","slo","clc","ora","nop","slo","nop","ora","asl","slo", /* 1 */
/* 2 */    "jsr","and","nop","rla","bit","and","rol","rla","plp","and","rol","nop","bit","and","rol","rla", /* 2 */
/* 3 */    "bmi","and","nop","rla","nop","and","rol","rla","sec","and","nop","rla","nop","and","rol","rla", /* 3 */
/* 4 */    "rti","eor","nop","sre","nop","eor","lsr","sre","pha","eor","lsr","nop","jmp","eor","lsr","sre", /* 4 */
/* 5 */    "bvc","eor","nop","sre","nop","eor","lsr","sre","cli","eor","nop","sre","nop","eor","lsr","sre", /* 5 */
/* 6 */    "rts","adc","nop","rra","nop","adc","ror","rra","pla","adc","ror","nop","jmp","adc","ror","rra", /* 6 */
/* 7 */    "bvs","adc","nop","rra","nop","adc","ror","rra","sei","adc","nop","rra","nop","adc","ror","rra", /* 7 */
/* 8 */    "nop","sta","nop","sax","sty","sta","stx","sax","dey","nop","txa","nop","sty","sta","stx","sax", /* 8 */
/* 9 */    "bcc","sta","nop","nop","sty","sta","stx","sax","tya","sta","txs","nop","nop","sta","nop","nop", /* 9 */
/* A */    "ldy","lda","ldx","lax","ldy","lda","ldx","lax","tay","lda","tax","nop","ldy","lda","ldx","lax", /* A */
/* B */    "bcs","lda","nop","lax","ldy","lda","ldx","lax","clv","lda","tsx","lax","ldy","lda","ldx","lax", /* B */
/* C */    "cpy","cmp","nop","dcp","cpy","cmp","dec","dcp","iny","cmp","dex","nop","cpy","cmp","dec","dcp", /* C */
/* D */    "bne","cmp","nop","dcp","nop","cmp","dec","dcp","cld","cmp","nop","dcp","nop","cmp","dec","dcp", /* D */
/* E */    "cpx","sbc","nop","isb","cpx","sbc","inc","isb","inx","sbc","nop","sbc","cpx","sbc","inc","isb", /* E */
/* F */    "beq","sbc","nop","isb","nop","sbc","inc","isb","sed","sbc","nop","isb","nop","sbc","inc","isb"  /* F */
};

int main(int argc, char **args) {
    // First, we load a binary file to memory, and wait until execution reaches the end of it.
    if (argc < 2) {
        printf("Please specify a binary file to load!\nExiting.\n");
        exit(1);
    }

    if  (argc < 3) {
        printf("Please specify how many registers are to be watched!\nExiting.\n");
        exit(1);
    }
    int numRegs = atoi(args[2]);

    FILE *file = fopen(args[1], "rb");
    if (file == NULL) {
        printf("Error: file not found!\nExiting.\n");
        exit(2);
    }

    const int binSize = fread(ram + 0x200, 1, 4096, file);

    char outputFileName[256];
    char dumpFileName[256];
    strcpy(outputFileName, args[1]);
    strcpy(dumpFileName, args[1]);
    strcat(outputFileName, ".cycles.csv");
    strcat(dumpFileName, ".execLog.csv");
    FILE *output = fopen(outputFileName, "w");
    FILE *dump = fopen(dumpFileName, "w");
    fprintf(dump, "Address,A,X,Y,P");
    for (int r = 0; r < numRegs; r++) {
        fprintf(dump, ",R%d", r);
    }
    fprintf(dump, ",Opcode\n");
    
    for (int rX = 0xAB; rX < 256; rX++) {
        for (int rY = 0xCD; rY < 256; rY++) {
            const int limit = 4096;
            reset6502();
            pc = 0x200;
            x = rX;
            y = rY;
            
            int c;
            for (c = 0; c < limit; c++) {
                fprintf(dump, "%04x,%02x,%02x,%02x,%02x", pc, a, x, y, status);
                for (int r = 0; r < numRegs; r++) {
                    fprintf(dump, ",%02x", ram[r]);
                }
                fprintf(dump, ",%s\n", opString[ram[pc]]);

                step6502();
                // The correct way of testing for termination is to see if
                //  the PC points to an RTS instruction. But an equally
                //  effective way is to test to see if it's pointing to any
                //  address below our strating point.
                if (pc < 0x200) {
                    break;
                }
            }

            if (c == limit) {
                printf("Error: infinite loop.\nLast PC: %ux\nX: %u, Y: %u\n", pc, rX, rY);
            }

            fprintf(dump, "\n");
            uint16_t result = ram[0];
            result += ram[1] << 8;
            if(result != rX * rY) {
                printf("Error: multiplication failed.\nX: %u, Y: %u\nResult: %d\n", rX, rY, result);
                exit(4);
            }

            fprintf(output, "%u,", clockticks6502);
            clockticks6502 = 0;
        }
        fprintf(output, "\n");
    }

    fclose(output);
    fclose(dump);

    printf("Cycles executed: %u\n", clockticks6502);
    return 0;
}