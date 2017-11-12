#include <stdio.h>
#include "myLib.h"
#include "game.h"
#include "graphics.h"

int main() {
    // Enable GBA Mode3
    REG_DISPCTL = MODE3 | BG2_ENABLE;

    GAMEState state = START;
    Slider slider;
    Slider *sldptr = &slider;
    Ball ball;
    Ball *ballptr = &ball;
    int level;
    //int lives = 3;

    int enterPressed = 0;

    // Game Loop
    while(1) {
        waitForVblank();
        switch (state) {
        case START:
            state = START_NODRAW;
            break;

        case START_NODRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = LEVEL1;
                enterPressed = 1;
            }
            break;

        case LEVEL1:
            level = 1;
            drawSlider(sldptr);
            createBall(sldptr->row - 5, sldptr->col + (sldptr->width / 2), 5, ballptr);
            generateBricks(level * 3);
            drawString3(5, 5, "Level 1", GREEN);
            state = LEVEL1_NODRAW;
            break;

        case LEVEL1_NODRAW:
            enableSlider(sldptr);
            startLevel(ballptr, sldptr);

            // Collision resolution for when ball hits bottom of screen
            if ((ballptr->row + ballptr->size) > 160) {
                state = GAME_OVER;
            }
            break;

        case GAME_OVER:
            fillScreen(BLACK);
            drawString3(79, 90, "GAME OVER", YELLOW);
            state = GAME_OVER_NO_DRAW;
            break;

        case GAME_OVER_NO_DRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = START;
                enterPressed = 1;
            }
            break;
        }

        if (!KEY_DOWN_NOW(BUTTON_START)) {
            enterPressed = 0;
        }
    }

    return 0;

}

/**
 * Function to setup the start screen
 */
void startScreen() {
    fillScreen(BLACK);
    drawImage3(0, 0, START_SCREEN_HEIGHT, START_SCREEN_WIDTH, start_screen);
    drawString3(105, 50, "Press ENTER to Start Game", YELLOW);
}
