#include <stdio.h>
#include "myLib.h"
#include "game.h"

int main() {
    // Enable GBA Mode3
    REG_DISPCTL = MODE3 | BG2_ENABLE;

    GAMEState state = START;
    Slider slider;
    Slider *sldptr = &slider;
    Ball ball;
    Ball *ballptr = &ball;

    int enterPressed = 0;

    // Game Loop
    while(1) {
        waitForVblank();
        switch (state) {
        case START:
            drawString3(79, 50, "Press ENTER to Start Game", YELLOW);
            state = START_NODRAW;
            break;

        case START_NODRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = LEVEL1;
                enterPressed = 1;
            }
            break;

        case LEVEL1:
            drawSlider(sldptr);
            createBall(sldptr->row - 5, sldptr->col + (sldptr->width / 2), 5, ballptr);
            state = LEVEL1_NODRAW;
            break;

        case LEVEL1_NODRAW:
            enableSlider(sldptr);
            startLevel(ballptr, sldptr);
            break;
        }

        if (!KEY_DOWN_NOW(BUTTON_START)) {
            enterPressed = 0;
        }
    }

    return 0;

}
