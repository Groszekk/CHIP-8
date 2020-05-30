#ifndef CHIP8_H
#define CHIP8_H

#include "config.h"
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include "memory.h"
#include "keyboard.h"
#include "registers.h"
#include "screen.h"
#include "stack.h"

struct chip8
{
    struct chip8_memory memory;
    struct chip8_stack stack;
    struct chip8_registers registers;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};

void CHIP8Init(struct chip8* chip8);
void CHIP8Load(struct chip8* chip8, const char* buf, size_t size);
void CHIP8Exec(struct chip8* chip8, __u_short opcode);

#endif