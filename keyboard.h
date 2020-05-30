#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "config.h"

typedef struct chip8_keyboard
{
    bool keyboard[CHIP8_TOTAL_KEYS];
    const char* keyboard_map;
} keyboard;

void CHIP8KeyboardSetMap(struct chip8_keyboard* keyboard, const char* map);
int CHIP8KeyboardMap(struct chip8_keyboard* keyboard, char key);
void CHIP8KeyboardDown(struct chip8_keyboard* keyboard, int key);
void CHIP8KeyboardUp(struct chip8_keyboard* keyboard, int key);
bool CHIP8KeyboarIsDown(struct chip8_keyboard* keyboard, int key);

#endif