#ifndef REGISTERS_H
#define REGISTERS_H
#include "config.h"

struct chip8_registers
{
    __u_char V[CHIP8_TOTAL_DATA_REGISTERS];
    __u_short I;
    __u_char delay_timer;
    __u_char sound_timer;
    __u_short PC;
    __u_char SP;
};

#endif