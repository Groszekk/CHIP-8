#ifndef STACK_H
#define STAKC_H
#include <stdint.h>
#include "config.h"

struct chip8;
struct chip8_stack
{
    __u_short stack[CHIP8_TOTAL_STACK_DEPTH];
};

void CHIP8StackPush(struct chip8* chip8, __u_short val);
__u_short CHIP8StackPop(struct chip8* chip8);

#endif