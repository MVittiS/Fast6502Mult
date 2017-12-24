#include<time.h>
#include<stdlib.h>
#include<stdio.h>

uint8_t ram[0x10000];

extern void reset6502();
extern void exec6502(uint32_t tickcount);
extern void step6502();
extern void irq6502();
extern void nmi6502();
extern void hookexternal(void *funcptr);
extern uint32_t clockticks6502;
extern uint32_t instructions;

uint8_t read6502(uint16_t address) {
    return ram[address];
}

void write6502(uint16_t address, uint8_t value){
    ram[address] = value;
}

int main(int argc, char **args) {
    reset6502();
    printf("Cycles executed: %u\n", clockticks6502);
    return 0;
}