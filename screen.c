#include "screen.h"

void ScreenBound(int x, int y)
{
    assert(x >= 0 && x < CHIP8_WIDTH && y >= 0 && y < CHIP8_HEIGHT);
}

void CHIP8ScreenSet(struct chip8_screen* screen, int x, int y)
{
    ScreenBound(x, y);
    screen->pixels[y][x] = true;
}


void CHIP8ScreenClear(struct chip8_screen* screen)
{
    memset(screen->pixels, 0, sizeof(screen->pixels));
}

bool CHIP8ScreenIsSet(struct chip8_screen* screen, int x, int y)
{
    ScreenBound(x, y);
    return screen->pixels[y][x];
}

bool CHIP8ScreenDrawSprite(struct chip8_screen* screen, int x, int y, const char* sprite, int num)
{
    bool pixel_collison = false;

    for (int ly = 0; ly < num; ly++)
    {
        char c = sprite[ly];
        for (int lx = 0; lx < 8; lx++)
        {
            if ((c & (0b10000000 >> lx)) == 0)
                continue;
            
            if (screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH])
            {
                pixel_collison = true;
            }

            screen->pixels[(ly+y) % CHIP8_HEIGHT][(lx+x) % CHIP8_WIDTH] ^= true;
        }
    }
    return pixel_collison;
}