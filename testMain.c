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

int main(int argc, char **args) {
    // First, we load a binary file to memory, and wait until execution reaches the end of it.
    if (argc < 2) {
        printf("Please specify a binary file to load!\nExiting.\n");
        exit(1);
    }
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
    strcat(outputFileName, ".csv");
    strcat(dumpFileName, ".log.csv");
    FILE *output = fopen(outputFileName, "w");
    FILE *dump = fopen(dumpFileName, "w");
    fprintf(dump, "Address,A,X,Y,P,R0,R1,R2,R3\n");
    
    for (int rX = 0; rX < 256; rX++) {
        for (int rY = 0; rY < 256; rY++) {
            const int limit = 4096;
            reset6502();
            pc = 0x200;
            x = rX;
            y = rY;
            
            for (int c = 0; c < limit; c++) {
                fprintf(dump, "%04x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x\n", pc, a, x, y, status, ram[0], ram[1], ram[2], ram[3]);

                step6502();
                if (pc == 0x200 + binSize - 1) {
                    break;
                }
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