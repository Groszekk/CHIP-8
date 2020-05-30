#ifndef SCREEN_H
#define SCREEN_H
#include "config.h"
#include <stdbool.h>
#include <assert.h>

struct chip8_screen
{
    bool pixels[CHIP8_HEIGHT][CHIP8_WIDTH];
};

void CHIP8ScreenClear(struct chip8_screen* screen);
void CHIP8ScreenSet(struct chip8_screen* screen, int x, int y);
bool CHIP8ScreenIsSet(struct chip8_screen* screen, int x, int y);
bool CHIP8ScreenDrawSprite(struct chip8_screen* screen, int x, int y, const char* sprite, int num);

#endif