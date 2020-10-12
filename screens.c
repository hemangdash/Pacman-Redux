#include <stdio.h>
#include <stdlib.h>

#include "screens.h"
#include "gba.h"

#include "images/StartScreen.h"
#include "images/losescreen.h"
#include "images/successkid.h"
#include "images/pacmanRight.h"
#include "images/pacmanUp.h"
#include "images/pacmanDown.h"
#include "images/pacmanLeft.h"
#include "images/number1.h"
#include "images/number2.h"
#include "images/number3.h"
#include "images/goscreen.h"
#include "images/gameover.h"

pac packy;
pac *packyLoc = &packy;
food foodballs[10];
int score;
int frames;
int mins;
int secs;
int face;

void init(void) { // Initialize Pacman's location and other variables
    score = 0;
    frames = 0;
    mins = 0;
    secs = 0;
    face = 0;
    packyLoc->row = 127;
    packyLoc->col = 10;
    packyLoc->width = 20;
    packyLoc->height = 20;
    u16 colors[] = {RED, BLUE, GREEN, WHITE, CYAN, TEAL, YELLOW, MAGENTA, ORANGE, PURPLE};
    int ncolors = sizeof(colors) / sizeof(colors[0]);
    for (int i = 0; i < 10; i++) {
        foodballs[i].row = rand() % 160;
        foodballs[i].col = rand() % 240;
        foodballs[i].color = colors[i % ncolors];
        foodballs[i].eaten = 0;
    }
    fillScreenDMA(BLACK);
    drawFood();
    drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanright);
}

void drawFood(void) { // Draw food balls
    for (int i = 0; i < 10; i++) {
        drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, foodballs[i].color);
    }
}

void startScreen(u32 currentbuttons, u32 previousbuttons) { // Start Screen
    frames = 0;
    drawFullScreenImageDMA(StartScreen);
    char *buffer = "Press START to play";
    drawCenteredString(110, 100, 50, 30, buffer, WHITE);
    while(1) {
        if (KEY_DOWN(BUTTON_START, BUTTONS)) {
            break;
        }
    }
    UNUSED(currentbuttons);
    UNUSED(previousbuttons);
}

int loadScreen(void) { // Load Screen
    if (frames <= 60) {
        drawFullScreenImageDMA(numberthree);
    } else if (frames <= 120) {
        drawFullScreenImageDMA(numbertwo);
    } else if (frames <= 180) {
        drawFullScreenImageDMA(number1);
    } else if (frames <= 210) {
        drawFullScreenImageDMA(goscreen);
    } else {
        init();
        return 1;
    }
    frames++;
    return 0;
}

int gameScreen(u32 buttons) { // Game Screen
    if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) { // Move Pacman left
        drawRectDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, BLACK);
        fillScreenDMA(BLACK);
        packyLoc->col -= 1;
        if (packyLoc->col < 0) {
            packyLoc->col = 0;
        }
        drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanleft);
        face = 1;
    } else if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) { // Move Pacman right
        drawRectDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, BLACK);
        fillScreenDMA(BLACK);
        packyLoc->col += 1;
        if (packyLoc->col > 220) {
            packyLoc->col = 220;
        }
        drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanright);
        face = 2;
    } else if (KEY_DOWN(BUTTON_UP, BUTTONS)) { // Move Pacman up
        drawRectDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, BLACK);
        fillScreenDMA(BLACK);
        packyLoc->row -= 1;
        if (packyLoc->row < 0) {
            packyLoc->row = 0;
        }
        drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanup);
        face = 3;
    } else if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) { // Move Pacman down
        drawRectDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, BLACK);
        fillScreenDMA(BLACK);
        packyLoc->row += 1;
        if (packyLoc->row > 127) {
            packyLoc->row = 127;
        }
        drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmandown);
        face = 4;
    }
    for (int i = 0; i < 10; i++) { // Randomize movement of the Food Balls
        int dir = rand() % 4;
        if (foodballs[i].eaten == 1) {
            continue;
        }
        switch(dir) {
            case 0:
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, BLACK);
                if (foodballs[i].color == RED) {
                    foodballs[i].col -= 10;
                } else {
                    foodballs[i].col -= 2;
                }
                if (foodballs[i].col < 0) {
                    foodballs[i].col = 0;
                }
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, foodballs[i].color);
                break;
            case 1:
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, BLACK);
                if (foodballs[i].color == RED) {
                    foodballs[i].col += 10;
                } else {
                    foodballs[i].col += 2;
                }
                if (foodballs[i].col > 235) {
                    foodballs[i].col = 235;
                }
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, foodballs[i].color);
                break;
            case 2:
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, BLACK);
                if (foodballs[i].color == RED) {
                    foodballs[i].row -= 10;
                } else {
                    foodballs[i].row -= 2;
                }
                if (foodballs[i].row < 0) {
                    foodballs[i].row = 0;
                }
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, foodballs[i].color);
                break;
            case 3:
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, BLACK);
                if (foodballs[i].color == RED) {
                    foodballs[i].row += 10;
                } else {
                    foodballs[i].row += 2;
                }
                if (foodballs[i].row > 142) {
                    foodballs[i].row = 142;
                }
                drawRectDMA(foodballs[i].row, foodballs[i].col, 5, 5, foodballs[i].color);
                break;
        }
    }
    for (int i = 0; i < 10; i++) { // Check for collisions
        if (foodballs[i].eaten != 1) {
            switch(face) {
                case 1:
                    if ((foodballs[i].col >= (packyLoc->col) && (foodballs[i].col + 5) <= (packyLoc->col + 10)) && (foodballs[i].row >= (packyLoc->row + 5) && (foodballs[i].row + 5) <= (packyLoc->row + 16))) {
                        if (eating(i) == 1) {
                            waitForVBlank();
                            return 1;
                        }
                    }
                    break;
                case 2:
                    if ((foodballs[i].col >= (packyLoc->col + 10) && (foodballs[i].col + 5) <= (packyLoc->col + 20)) && (foodballs[i].row >= (packyLoc->row + 5) && (foodballs[i].row + 5) <= (packyLoc->row + 16))) {
                        if (eating(i) == 1) {
                            waitForVBlank();
                            return 1;
                        }
                    }
                    break;
                case 3:
                    if ((foodballs[i].col >= (packyLoc->col + 5) && (foodballs[i].col + 5) <= (packyLoc->col + 16)) && (foodballs[i].row >= (packyLoc->row) && (foodballs[i].row + 5) <= (packyLoc->row + 10))) {
                        if (eating(i) == 1) {
                            waitForVBlank();
                            return 1;
                        }
                    }
                    break;
                case 4:
                    if ((foodballs[i].col >= (packyLoc->col + 5) && (foodballs[i].col + 5) <= (packyLoc->col + 16)) && (foodballs[i].row >= (packyLoc->row + 10) && (foodballs[i].row + 5) <= (packyLoc->row + 20))) {
                        if (eating(i) == 1) {
                            waitForVBlank();
                            return 1;
                        }
                    }
                    break;
            }
        }
    }
    switch(face) { // Redraw the Pacman
        case 1:
            drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanleft);
            break;
        case 2:
            drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanright);
            break;
        case 3:
            drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmanup);
            break;
        case 4:
            drawImageDMA(packyLoc->row, packyLoc->col, packyLoc->width, packyLoc->height, pacmandown);
            break;
    }
    int win = 1;
    for (int i = 0; i < 10; i++) { // Checks if all Food Balls except for the Red Food Ball have been eaten
        if (foodballs[i].color == RED) {
            continue;
        } else {
            if (foodballs[i].eaten != 1) {
                win = 0;
            }
        }
    }
    if (win == 1) { // Game Over with a Win Result
        return 2;
    }
    drawRectDMA(147, 0, 240, 13, BLUE);
    char scoreBuffer[10];
    sprintf(scoreBuffer, "Score: %d", score);
    drawString(150, 2, scoreBuffer, YELLOW);
    char framesBuffer[10];
    if (frames % 60 == 0) {
        if (secs < 10) {
            sprintf(framesBuffer, "Timer: %d:0%d", mins, secs);
        } else {
            sprintf(framesBuffer, "Timer: %d:%d", mins, secs);
        }
    }
    drawString(150, 170, framesBuffer, YELLOW);
    UNUSED(buttons);
    frames++;
    if (frames % 3600 == 0) {
        mins++;
    }
    if (frames % 60 == 0) {
        if (frames % 3600 == 0) {
            secs = 0;
        } else {
            secs++;
        }
    }
    return 0;
}

int eating(int i) { // What happens if Pacman eats a food ball
    foodballs[i].eaten = 1;
    if (foodballs[i].color != RED) {
        score += (i + 1);
    } else {
        return 1;
    }
    foodballs[i].color = BLACK;
    return 0;
}

void endScreen(u32 buttons, int result) { // End Screen
    drawFullScreenImageDMA(gameover); // Game Over Screen
    delay(100);
    if (!KEY_DOWN(BUTTON_SELECT, BUTTONS)) {
        // Win Screen
        if (result == 1) {
            drawFullScreenImageDMA(successkid);
            drawString(40, 130, "Oh Damn! YOU WON!", BLACK);
            char scoreBuffer[10];
            sprintf(scoreBuffer, "Score: %d", score);
            drawString(70, 150, scoreBuffer, BLACK);
            char timeBuffer[10];
            if (secs < 10) {
                sprintf(timeBuffer, "Time: %d:0%d", mins, secs);
            } else {
                sprintf(timeBuffer, "Time: %d:%d", mins, secs);
            }
            drawString(100, 150, timeBuffer, BLACK);
        }
        // Lose Screen
        else {
            drawFullScreenImageDMA(losescreen);
            drawString(45, 160, "I'm deeply", BLACK);
            drawString(60, 160, "sorry for", BLACK);
            drawString(75, 160, "your loss.", BLACK);
            char scoreBuffer[10];
            sprintf(scoreBuffer, "Score: %d", score);
            drawString(115, 160, scoreBuffer, BLACK);
        }
    }
    while(1) { // Go back to Start Screen if START or SELECT are pressed
        if (KEY_DOWN(BUTTON_START, BUTTONS) || KEY_DOWN(BUTTON_SELECT, BUTTONS)) {
            delay(10);
            break;
        }
    }
    UNUSED(buttons);
}