#include "myLib.h"
#include "game.h"

/**
 * Function to draw the slider on every screen
 *
 * @param sldptr the pointer to the slider to be drawn
 */
void drawSlider(Slider *sldptr) {
    sldptr->height = 5;
    sldptr->width = 50;
    sldptr->row = 159 - sldptr->height;
    sldptr->col = 119 - (sldptr->width / 2);
    sldptr->oldRow = sldptr->row;
    sldptr->oldCol = sldptr->col;
    fillScreen(BLACK);
    drawRect(sldptr->row, sldptr->col, sldptr->height, sldptr->width, YELLOW);
}

/**
 * Function to enable movement of slider across sides of screen
 *
 * @param sldptr the pointer to the slider to be moved
 */
void enableSlider(Slider *sldptr) {

    int cdel = 2;
    const int height = sldptr->height;

    if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
        // Get the width of the slider
        int width = sldptr->width;

        // Erase previous slider position
        drawRect(sldptr->oldRow, sldptr->oldCol, height, width, BLACK);
        sldptr->oldRow = sldptr->row;
        sldptr->oldCol = sldptr->col;

        sldptr->col = sldptr->col + cdel;
        if (sldptr->col + width > 240) {
            sldptr->col = 240 - width;
        }
        //drawRect(oldrow, oldcol, height, width, BLACK);
        drawRect(sldptr->row, sldptr->col, height, width, YELLOW);
    }

    if (KEY_DOWN_NOW(BUTTON_LEFT)) {
        // Get the width of the slider
        int width = sldptr->width;

        // Erase previous slider position
        drawRect(sldptr->oldRow, sldptr->oldCol, height, width, BLACK);
        sldptr->oldRow = sldptr->row;
        sldptr->oldCol = sldptr->col;

        sldptr->col = sldptr->col - cdel;
        if (sldptr->col < 0) {
            sldptr->col = 0;
        }
        //drawRect(sldptr->oldRow, sldptr->oldCol, height, width, BLACK);
        drawRect(sldptr->row, sldptr->col, height, width, YELLOW);
    }
}

/**
 * Function to create the game ball
 *
 * @param row the row to start the ball
 * @param col the col to start the ball
 * @param size the size of the ball
 * @param ballptr the pointer to the ball to move
 */
void createBall(int row, int col, int size, Ball *ballptr) {
    ballptr->xDir = 1;
    ballptr->yDir = -1;
    ballptr->row = row;
    ballptr->col = col;
    ballptr->oldRow = ballptr->row;
    ballptr->oldCol = ballptr->col;
    ballptr->size = size;
    drawRect(row, col, size, size, WHITE);
}

/**
 * Function to start ball movement
 *
 * @param ballptr the pointer to the ball to move
 * @param sldptr the pointer to the slider of the level
 */
void startLevel(Ball *ballptr, Slider *sldptr) {
    drawRect(ballptr->oldRow, ballptr->oldCol, ballptr->size, ballptr->size, BLACK);
    ballptr->col += ballptr->xDir;
    ballptr->row += ballptr->yDir;

    handleCollisions(ballptr, sldptr);

    ballptr->oldRow = ballptr->row;
    ballptr->oldCol = ballptr->col;
    drawRect(ballptr->row, ballptr->col, ballptr->size, ballptr->size, WHITE);
}

/**
 * Function to handle collisions in the game
 *
 * @param ballptr the pointer to the ball of the game
 * @param sldptr the pointer to the slider of the game
 */
void handleCollisions(Ball *ballptr, Slider *sldptr) {
    int size = ballptr->size;

    if (ballptr->col < 0) {
        ballptr->col = 0;
        ballptr->xDir *= -1;
    }

    if (ballptr->col + size > 240) {
        ballptr->col = 240 - size;
        ballptr->xDir *= -1;
    }

    if (ballptr->row < 0) {
        ballptr->row = 0;
        ballptr->yDir *= -1;
    }

    // Collision resolution for when bottom of ball hits top of slider
    if ((ballptr->row + size) > sldptr->row
            && ballptr->col >= sldptr->col
            && (ballptr->col + size) <= (sldptr->col + sldptr->width)) {
        ballptr->row = sldptr->row - ballptr->size;
        ballptr->yDir *= -1;
    }
}
