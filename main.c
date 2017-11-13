#include <stdio.h>
#include "myLib.h"
#include "game.h"
#include "graphics.h"
#include "main.h"

GAMEState state = START;
Slider slider;
Slider *sldptr = &slider;
Ball ball;
Ball *ballptr = &ball;
Brick bricks[18];
Brick *brptr = bricks;
int numBricks;
int bricksSize;
int level = 1;
int lives = 3;

int main() {
    // Enable GBA Mode3
    REG_DISPCTL = MODE3 | BG2_ENABLE;

    int enterPressed = 0;

    // Game Loop
    while(1) {
        waitForVblank();
        switch (state) {
        case START:
            startScreen();
            state = START_NODRAW;
            break;

        case START_NODRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = LEVEL;
                enterPressed = 1;
            }
            break;

        case LEVEL:
            setGameStage();
            state = LEVEL_NODRAW;
            break;

        case LEVEL_NODRAW:
            startLevel();
            // Collision resolution for when ball hits bottom of screen
            if ((ballptr->row + BALLSIZE) >= SCREENHEIGHT) {
                lives -= 1;
                if (lives == 0) {
                    state = GAME_OVER;
                }
            }
            if (numBricks <= 0) {
                if (level < MAXLEVEL) {
                    level++;
                    state = LEVEL;
                }
            }
            break;

        case GAME_OVER:
            fillScreen(BGCOLOR);
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
    fillScreen(BGCOLOR);
    drawImage3(0, 0, START_SCREEN_HEIGHT, START_SCREEN_WIDTH, start_screen);
    drawString3(105, 50, "Press ENTER to Start Game", YELLOW);
}

/**
 * Function to generate basic game stage
 */
void setGameStage() {
    fillScreen(BGCOLOR);
    drawString3(5, 5, "Level 1", GREEN);
    drawSlider(sldptr);
    createBall(sldptr->row - 5, sldptr->col + (SLIDERWIDTH / 2), BALLSIZE, ballptr);
    numBricks = level * 3;
    bricksSize = level * 3;
    generateBricks(brptr, bricksSize);
}

/**
 * Function to begin the level
 */
void startLevel() {
    enableSlider(sldptr);
    ballMovement(ballptr, sldptr, brptr, &numBricks, bricksSize);
}
