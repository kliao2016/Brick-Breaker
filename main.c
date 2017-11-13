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
int level = INITIAL_LEVEL;
int lives = INITIAL_LIVES;
char textBuffer[75];
int enterPressed = 0;
int zPressed = 0;
int backPressed = 0;

int main() {
    // Enable GBA Mode3
    REG_DISPCTL = MODE3 | BG2_ENABLE;

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
            // Collision resolution for when ball hits bottom of screen
            if (ballptr->row + BALLSIZE >= sldptr->row + SLIDERHEIGHT) {
                lives -= 1;
                if (lives > 0) {
                    state = LOSE_LIFE;
                } else {
                    state = GAME_OVER;
                }
            }
            startLevel();
            if (numBricks <= 0) {
                if (level < MAXLEVEL) {
                    level++;
                    state = NEXT_LEVEL;
                } else {
                    state = WIN_GAME;
                }
            }
            backToMain();
            break;

        case NEXT_LEVEL:
            setUpNextLevel();
            state = NEXT_LEVEL_NO_DRAW;
            break;

        case NEXT_LEVEL_NO_DRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = LEVEL;
                enterPressed = 1;
            }
            backToMain();
            break;

        case LOSE_LIFE:
            tryAgain();
            state = LOSE_LIFE_NO_DRAW;
            break;

        case LOSE_LIFE_NO_DRAW:
            if (!zPressed && KEY_DOWN_NOW(BUTTON_A)) {
                state = LEVEL;
                enterPressed = 1;
            }
            backToMain();
            break;

        case GAME_OVER:
            gameOver();
            state = GAME_OVER_NO_DRAW;
            break;

        case GAME_OVER_NO_DRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                state = START;
                enterPressed = 1;
            }
            backToMain();
            break;

        case WIN_GAME:
            gameWon();
            state = WIN_GAME_NO_DRAW;
            break;

        case WIN_GAME_NO_DRAW:
            if (!enterPressed && KEY_DOWN_NOW(BUTTON_START)) {
                resetGame();
                enterPressed = 1;
            }
            backToMain();
            break;
        }

        if (!KEY_DOWN_NOW(BUTTON_START)) {
            enterPressed = 0;
        }
        if (!KEY_DOWN_NOW(BUTTON_A)) {
            zPressed = 0;
        }
        if (!KEY_DOWN_NOW(BUTTON_SELECT)) {
            backPressed = 0;
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
    updateScreenText();
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

/**
 * Function to notify player that the game has ended and they lost
 */
void gameOver() {
    fillScreen(BGCOLOR);
    drawString3(70, 99, "GAME OVER.", RED);
    drawString3(79, 50, "PRESS ENTER TO PLAY AGAIN", RED);
}

/**
 * Function to notify that the player has lost a life
 */
void tryAgain() {
    fillScreen(BGCOLOR);
    drawString3(70, 67, "YOU LOST A LIFE.", RED);
    drawString3(79, 60, "HIT Z TO TRY AGAIN", RED);
}

/**
 * Function to notify that the player has won the game
 */
void gameWon() {
    fillScreen(BGCOLOR);
    drawString3(70, 56, "CONGRATULATIONS, YOU WON!", GREEN);
    drawString3(79, 53, "PRESS ENTER TO PLAY AGAIN", GREEN);
}

/**
 * Function to reset the game
 */
void resetGame() {
    level = INITIAL_LEVEL;
    lives = INITIAL_LIVES;
    state = START;
}

/**
 * Function to notify player that they made it to the next level
 */
void setUpNextLevel() {
    fillScreen(BGCOLOR);
    sprintf(textBuffer, "LEVEL %d. PRESS ENTER TO START", level);
    drawString3(79, 36, textBuffer, YELLOW);
}

/**
 * Function to update text on the screen
 */
void updateScreenText() {
    sprintf(textBuffer, "LEVEL %d", level);
    drawString3(SCREENHEIGHT - 10, 5, textBuffer, GREEN);
    sprintf(textBuffer, "LIVES: %d", lives);
    drawString3(SCREENHEIGHT - 10, SCREENWIDTH - sizeof(textBuffer) + 20, textBuffer, GREEN);
}

/**
 * Function to go back to start screen
 */
void backToMain() {
    if (!backPressed && KEY_DOWN_NOW(BUTTON_SELECT)) {
        state = START;
        backPressed = 1;
    }
}
