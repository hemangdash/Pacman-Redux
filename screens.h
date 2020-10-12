#ifndef SCREENS_H
#define SCREENS_H

#include "gba.h"

typedef struct PAC {
    int row;
    int col;
    int width;
    int height;
} pac;

typedef struct FOOD {
    int row;
    int col;
    u16 color;
    int eaten;
} food;

void init(void); // Initialize Pacman's location and other variables
void drawFood(void); // Draw food balls
int eating(int i); // What happens if Pacman eats a food ball

void startScreen(u32 currentbuttons, u32 previousbuttons); // Start Screen
int loadScreen(void); // Load Screen
int gameScreen(u32 buttons); // Game Screen
void endScreen(u32 buttons, int result); // End Screen

#endif