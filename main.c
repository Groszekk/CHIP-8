#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "SDL2/SDL.h"
#include "chip8.h"
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void StackBound(struct chip8* chip8)
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

void MemoryBound(int index)
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

const char keyboard_map[CHIP8_TOTAL_KEYS] = {
    SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
    SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
    SDLK_c, SDLK_d, SDLK_e, SDLK_f};

const char character_set[] = {
    0xf0, 0x90, 0x90, 0x90, 0xf0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xf0, 0x10, 0xf0, 0x80, 0xf0,
    0xf0, 0x10, 0xf0, 0x10, 0xf0,
    0x90, 0x90, 0xf0, 0x10, 0x10,
    0xf0, 0x80, 0xf0, 0x10, 0xf0,
    0xf0, 0x80, 0xf0, 0x90, 0xf0,
    0xf0, 0x10, 0x20, 0x40, 0x40,
    0xf0, 0x90, 0xf0, 0x90, 0xf0,
    0xf0, 0x90, 0xf0, 0x10, 0xf0,
    0xf0, 0x90, 0xf0, 0x90, 0x90,
    0xe0, 0x90, 0xe0, 0x90, 0xe0,
    0xf0, 0x80, 0x80, 0x80, 0xf0,
    0xe0, 0x90, 0x90, 0x90, 0xe0,
    0xf0, 0x80, 0xf0, 0x80, 0xf0, 
    0xf0, 0x80, 0xf0, 0x80, 0x80
};

void CHIP8Init(struct chip8* chip8)
{
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, character_set, sizeof(character_set));
}

void CHIP8Load(struct chip8* chip8, const char* buf, size_t size)
{
    assert(size+CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROGRAM_LOAD_ADDRESS], buf, size);
    chip8->registers.PC = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void chip8_exec_extended_eight(struct chip8* chip8, __u_short opcode)
{
    __u_char x = (opcode >> 8) & 0x000f;
    __u_char y = (opcode >> 4) & 0x000f;
    __u_char final_four_bits = opcode & 0x000f;
    __u_short tmp = 0;
    switch(final_four_bits)
    {
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
        break;

        case 0x01:
            chip8->registers.V[x] = chip8->registers.V[x] | chip8->registers.V[y];
        break;

        case 0x02:
            chip8->registers.V[x] = chip8->registers.V[x] & chip8->registers.V[y];
        break;

        case 0x03:
            chip8->registers.V[x] = chip8->registers.V[x] ^ chip8->registers.V[y];
        break;

        case 0x04:
            tmp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0f] = false;
            if (tmp > 0xff)
            {
                chip8->registers.V[0x0f] = true;
            }

            chip8->registers.V[x] = tmp;
        break;

        case 0x05:
            chip8->registers.V[0x0f] = false;
            if (chip8->registers.V[x] > chip8->registers.V[y])
            {
                chip8->registers.V[0x0f] = true;
            }
            chip8->registers.V[x] = chip8->registers.V[x] - chip8->registers.V[y];
        break;

        case 0x06:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0x01;
            chip8->registers.V[x] = chip8->registers.V[x] / 2;
        break;

        case 0x07:
            chip8->registers.V[0x0f] = chip8->registers.V[y] > chip8->registers.V[x];
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
        break;

        case 0x0E:
            chip8->registers.V[0x0f] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] = chip8->registers.V[x] * 2;
        break;
    }
}

char chip8_wait_for_key_press(struct chip8* chip8)
{
    SDL_Event event;
    while(SDL_WaitEvent(&event))
    {
        if (event.type != SDL_KEYDOWN)
            continue;
        
        char c = event.key.keysym.sym;
        char chip8_key = CHIP8KeyboardMap(&chip8->keyboard, c);
        if (chip8_key != -1)
        {
            return chip8_key;
        }
    }

    return -1;
}

void chip8_exec_extended_F(struct chip8* chip8, __u_short opcode)
{
    __u_char x = (opcode >> 8) & 0x000f;
    switch (opcode & 0x00ff)
    {
        case 0x07:
            chip8->registers.V[x] = chip8->registers.delay_timer;
        break;

        case 0x0A:
        {
            char pressed_key = chip8_wait_for_key_press(chip8);
            chip8->registers.V[x] = pressed_key;
        }
        break; 

        case 0x15:
            chip8->registers.delay_timer = chip8->registers.V[x];
        break;

        case 0x18:
            chip8->registers.sound_timer = chip8->registers.V[x];
        break;


        case 0x1e:
            chip8->registers.I += chip8->registers.V[x];
        break;

        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
        break;

        case 0x33:
        {
            __u_char hundreds = chip8->registers.V[x] / 100;
            __u_char tens = chip8->registers.V[x] / 10 % 10;
            __u_char units = chip8->registers.V[x] % 10;
            CHIP8MemorySet(&chip8->memory, chip8->registers.I, hundreds);
            CHIP8MemorySet(&chip8->memory, chip8->registers.I+1, tens);
            CHIP8MemorySet(&chip8->memory, chip8->registers.I+2, units);
        }
        break;

        case 0x55:
        {
            for (int i = 0; i <= x; i++)
            {
                CHIP8MemorySet(&chip8->memory, chip8->registers.I+i, chip8->registers.V[i]);
            }
        }
        break;

        case 0x65:
        {
            for (int i = 0; i <= x; i++)
            {
                chip8->registers.V[i] = CHIP8MemoryGet(&chip8->memory, chip8->registers.I+i);
            }
        }
        break;

    }
}

void chip8_exec_extended(struct chip8* chip8, __u_short opcode)
{
    __u_short nnn = opcode & 0x0fff;
    __u_char x = (opcode >> 8) & 0x000f;
    __u_char y = (opcode >> 4) & 0x000f;
    __u_char kk = opcode & 0x00ff;
    __u_char n = opcode & 0x000f;
    switch(opcode & 0xf000)
    {
        case 0x1000:
            chip8->registers.PC = nnn;
        break;

        case 0x2000:
            CHIP8StackPush(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
        break;

        case 0x3000:
            if (chip8->registers.V[x] == kk)
            {
                chip8->registers.PC += 2;
            }
        break;

        case 0x4000:
            if (chip8->registers.V[x] != kk)
            {
                chip8->registers.PC += 2;
            }
        break;

        case 0x5000:
            if (chip8->registers.V[x] == chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        case 0x6000:
            chip8->registers.V[x] = kk;
        break;

        case 0x7000:
            chip8->registers.V[x] += kk;
        break;

        case 0x8000:
            chip8_exec_extended_eight(chip8, opcode);
        break;

        case 0x9000:
            if (chip8->registers.V[x] != chip8->registers.V[y])
            {
                chip8->registers.PC += 2;
            }
        break;

        case 0xA000:
            chip8->registers.I = nnn;
        break;

        case 0xB000:
            chip8->registers.PC = nnn + chip8->registers.V[0x00];
        break;

        case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
        break;

        case 0xD000:
        {
            const char* sprite = (const char*) &chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = CHIP8ScreenDrawSprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
        }
        break;

        case 0xE000:
        {
            switch(opcode & 0x00ff)
            {
                case 0x9e:
                    if (CHIP8KeyboarIsDown(&chip8->keyboard, chip8->registers.V[x]))
                    {
                        chip8->registers.PC += 2;
                    }
                break;

                case 0xa1:
                    if (!CHIP8KeyboarIsDown(&chip8->keyboard, chip8->registers.V[x]))
                    {
                        chip8->registers.PC += 2;
                    }
                break;
            }
        }
        break;
        
        case 0xF000:
            chip8_exec_extended_F(chip8, opcode);
        break;

    }
}

void CHIP8Exec(struct chip8* chip8, __u_short opcode)
{
    switch(opcode)
    {
        case 0x00E0:
            CHIP8ScreenClear(&chip8->screen);
        break;

        case 0x00EE:
            chip8->registers.PC = CHIP8StackPop(chip8);
        break;

        default:
            chip8_exec_extended(chip8, opcode);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("usage chip8 <file.rom>\n");
        return -1;
    }

    const char* filename = argv[1];
    printf("%s loading...\n", filename);

    FILE* file = fopen(filename, "rb");
    if (!file)
    {
        printf("Failed to open the file");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buf[size];
    int res = fread(buf, size, 1, file);
    if (res != 1)
    {
        printf("Failed to read from file, please check filetype");
        return -1;
    }
    
    struct chip8 chip8;
    CHIP8Init(&chip8);
    CHIP8Load(&chip8, buf, size);
    CHIP8KeyboardSetMap(&chip8.keyboard, keyboard_map);
   

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
        EMULATOR_WINDOW_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        CHIP8_WIDTH * CHIP8_WINDOW_MULTIPLIER,
        CHIP8_HEIGHT * CHIP8_WINDOW_MULTIPLIER, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    for(;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                goto out;
                break;

            case SDL_KEYDOWN:
            {
                char key = event.key.keysym.sym;
                int vkey = CHIP8KeyboardMap(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    CHIP8KeyboardDown(&chip8.keyboard, vkey);
                }
            }
            break;

            case SDL_KEYUP:
            {
                char key = event.key.keysym.sym;
                int vkey = CHIP8KeyboardMap(&chip8.keyboard, key);
                if (vkey != -1)
                {
                    CHIP8KeyboarUp(&chip8.keyboard, vkey);
                }
            }
            break;
            };
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int x = 0; x < CHIP8_WIDTH; x++)
        {
            for (int y = 0; y < CHIP8_HEIGHT; y++)
            {
                if (CHIP8ScreenIsSet(&chip8.screen, x, y))
                {
                    SDL_Rect r;
                    r.x = x * CHIP8_WINDOW_MULTIPLIER;
                    r.y = y * CHIP8_WINDOW_MULTIPLIER;
                    r.w = CHIP8_WINDOW_MULTIPLIER;
                    r.h = CHIP8_WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);
        if (chip8.registers.delay_timer > 0)
        {
            SDL_Delay(5);
            chip8.registers.delay_timer -=1;
        }

        if (chip8.registers.sound_timer > 0)
        {
            chip8.registers.sound_timer = 0;
        }
        
        __u_short opcode = CHIP8MemoryGetShort(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        CHIP8Exec(&chip8, opcode);
        //printf("%x\n", opcode);
    }

out:
    SDL_DestroyWindow(window);
    return 0;
}