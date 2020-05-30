#include "memory.h"
#include <assert.h>

static void MemoryBound(int index)
{
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void CHIP8MemorySet(struct chip8_memory* memory, int index, __u_char val)
{
    MemoryBound(index);
    memory->memory[index] = val;
}

__u_char CHIP8MemoryGet(struct chip8_memory* memory, int index)
{
    MemoryBound(index);
    return memory->memory[index];
}

__u_short CHIP8MemoryGetShort(struct chip8_memory* memory, int index)
{
    __u_char byte1 = CHIP8MemoryGet(memory, index);
    __u_char byte2 = CHIP8MemoryGet(memory, index+1);
    return byte1 << 8 | byte2;
}