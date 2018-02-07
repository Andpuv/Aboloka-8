#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

/*create a cpu object*/
struct System *initCPU(){
    struct System *sys = (struct System *) malloc(sizeof(sys));

    /*0 out CPU struct*/
    sys->cpu.pc   = 0x00;
    sys->cpu.regA = 0x00;
    sys->cpu.regX = 0x00;
    sys->cpu.regY = 0x00;
    sys->cpu.regZ = 0x00;


    return sys;
    
}


/*execute a given program*/
void executeProgram(struct System *sys, uint8_t *program){
    /*execute each opcode until 'exit' (0xEA) is reached*/
    while(program[sys->cpu.pc] != 0xEA){
        /*switch statement for opcodes*/
        switch(program[sys->cpu.pc]){
        case 0x10:
            /*load the value into memory*/
            cpu_load(sys, program[++sys->cpu.pc], program[++sys->cpu.pc],
                     program[++sys->cpu.pc]);
            
            break;
        case 0x11:
            /*load the value into the X register*/
            sys->cpu.regX = program[++sys->cpu.pc];
            break;
        case 0x12:
            /*load the value into the Y register*/
            sys->cpu.regY = program[++sys->cpu.pc];
            break;
        case 0x13:
            /*load the value into the Z register*/
            sys->cpu.regZ = program[++sys->cpu.pc];
            break;
        case 0x14:
            /*load the value into the accumulator*/
            sys->cpu.regA = program[++sys->cpu.pc];
            break;
        case 0x20:
            /*jump to the next location*/
            sys->cpu.pc = program[++sys->cpu.pc];
            break;
        case 0x21:
            /*check which register is being jumped to
              X: 0, Y: 1, Z: 2, A: 3
             */
            switch(program[++sys->cpu.pc]){
            case 0x00:
                sys->cpu.pc = sys->cpu.regX;
            case 0x01:
                sys->cpu.pc = sys->cpu.regY;
            case 0x02:
                sys->cpu.pc = sys->cpu.regZ;
            case 0x03:
                sys->cpu.pc = sys->cpu.regA;
            }
            break;
            
        }
        /*consume previous byte*/
        sys->cpu.pc++;
    }
}

/*load value into memory*/
void cpu_load(struct System *sys, uint8_t addrB1, uint8_t addrB2, uint8_t val){
    uint16_t address = addrB1 | (addrB2 << 8);
    sys->memory[address] = val;
}

