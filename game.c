#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "screens.h"

// States of the Game
typedef enum {
  START,
  LOAD,
  PLAY,
  WIN,
  LOSE,
} GBAState;

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3.
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial game state
  GBAState state = START;

  while (1) {
    currentButtons = BUTTONS;  // Load the current state of the buttons

    waitForVBlank();

    //On pressing SELECT, you go to the START state
    if (KEY_DOWN(BUTTON_SELECT, BUTTONS)) {
      state = START;
    }

    int result;
    
    switch (state) {
      case START:
        startScreen(currentButtons, previousButtons);
        state = LOAD;
        break;
      case LOAD:
        result = loadScreen();
        if (result == 1) {
          state = PLAY;
        } 
        break;
      case PLAY:
        result = gameScreen(currentButtons);
        if (result == 1) {
          state = LOSE;
        } else if (result == 2) {
          state = WIN;
        }
        break;
      case WIN:
        endScreen(currentButtons, 1);
        state = START;
        break;
      case LOSE:
        endScreen(currentButtons, 0);
        state = START;
        break;
    }

    previousButtons = currentButtons;  // Store the current state of the buttons
  }

  UNUSED(previousButtons);  // You can remove this once previousButtons is used

  return 0;
}
