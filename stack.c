#include "stack.h"
#include "chip8.h"
#include <assert.h>

static void StackBound(struct chip8* chip8)
{
    assert(chip8->registers.SP < sizeof(chip8->stack.stack));    
}

void CHIP8StackPush(struct chip8* chip8, __u_short val)
{
    chip8->registers.SP += 1;
    StackBound(chip8);

    chip8->stack.stack[chip8->registers.SP] = val;
}

__u_short CHIP8StackPop(struct chip8* chip8)
{
    StackBound(chip8);
    
    __u_short result = chip8->stack.stack[chip8->registers.SP];
    chip8->registers.SP -=1;
    return result;
}