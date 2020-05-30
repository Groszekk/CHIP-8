#include "keyboard.h"
#include <assert.h>

void KeyboardBound(int key)
{
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

void CHIP8KeyboardSetMap(struct chip8_keyboard* keyboard, const char* map)
{
    keyboard->keyboard_map = map;
}

int CHIP8KeyboardMap(struct chip8_keyboard* keyboard, char key)
{
    
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++)
    {
        if (keyboard->keyboard_map[i] == key)
        {
            return i;
        }
    }

    return -1;
}

void CHIP8KeyboardDown(struct chip8_keyboard* keyboard, int key)
{
    keyboard->keyboard[key] = true;
}

void CHIP8KeyboarUp(struct chip8_keyboard* keyboard, int key)
{
    keyboard->keyboard[key] = false;
}

bool CHIP8KeyboarIsDown(struct chip8_keyboard* keyboard, int key)
{
    return keyboard->keyboard[key];
}