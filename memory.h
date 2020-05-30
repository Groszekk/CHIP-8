#ifndef MEMORY_H
#define MEMORY_H
#include "config.h"

typedef struct chip8_memory
{
    __u_char memory[CHIP8_MEMORY_SIZE];
} memory;

void CHIP8MemorySet(struct chip8_memory* memory, int index, __u_char val);
__u_char CHIP8MemoryGet(struct chip8_memory* memory, int index);
__u_short CHIP8MemoryGetShort(struct chip8_memory* memory, int index);

#endif