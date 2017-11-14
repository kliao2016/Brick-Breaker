#include <stdlib.h>
#include "myLib.h"
#include "game.h"
#include "graphics.h"

/**
 * Function to draw the slider on every screen
 *
 * @param sldptr the pointer to the slider to be drawn
 */
void drawSlider(Slider *sldptr) {
    sldptr->height = SLIDER_HEIGHT;
    sldptr->width = SLIDER_WIDTH;
    sldptr->row = SCREENHEIGHT - 15 - sldptr->height;
    sldptr->col = (SCREENWIDTH / 2) - 1 - (sldptr->width / 2);
    sldptr->oldRow = sldptr->row;
    sldptr->oldCol = sldptr->col;
    sldptr->cdel = 2;
    drawImage3(sldptr->row, sldptr->col, sldptr->height, sldptr->width, slider_image);
}

/**
 * Function to enable movement of slider across sides of screen
 *
 * @param sldptr the pointer to the slider to be moved
 */
void enableSlider(Slider *sldptr) {

    int cdel = sldptr->cdel;
    const int height = sldptr->height;

    if (KEY_DOWN_NOW(BUTTON_RIGHT)) {

        if (sldptr->cdel < 0) {
            sldptr->cdel *= -1;
        }

        // Get the width of the slider
        int width = sldptr->width;

        // Erase previous slider position
        drawRect(sldptr->oldRow, sldptr->oldCol, height, width, BGCOLOR);
        sldptr->oldRow = sldptr->row;
        sldptr->oldCol = sldptr->col;

        sldptr->col = sldptr->col + cdel;
        if (sldptr->col + width > SCREENWIDTH) {
            sldptr->col = SCREENWIDTH - width;
        }
        drawImage3(sldptr->row, sldptr->col, sldptr->height, sldptr->width, slider_image);
    }

    if (KEY_DOWN_NOW(BUTTON_LEFT)) {

        if (sldptr->cdel > 0) {
            sldptr->cdel *= -1;
        }

        // Get the width of the slider
        int width = sldptr->width;

        // Erase previous slider position
        drawRect(sldptr->oldRow, sldptr->oldCol, height, width, BGCOLOR);
        sldptr->oldRow = sldptr->row;
        sldptr->oldCol = sldptr->col;

        sldptr->col = sldptr->col + cdel;
        if (sldptr->col < 0) {
            sldptr->col = 0;
        }
        drawImage3(sldptr->row, sldptr->col, sldptr->height, sldptr->width, slider_image);
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
 * @param brptr the pointer to the array of bricks in the game
 * @param numBricks the pointer to the number of bricks in the game
 * @param bricksSize the size of the array of bricks
 */
void ballMovement(Ball *ballptr, Slider *sldptr, Brick *brptr, int *numBricks, int bricksSize) {
    drawRect(ballptr->oldRow, ballptr->oldCol, ballptr->size, ballptr->size, BGCOLOR);
    ballptr->col += ballptr->xDir;
    ballptr->row += ballptr->yDir;

    handleCollisions(ballptr, sldptr);
    handleBrickCollisions(brptr, ballptr, numBricks, bricksSize);

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

    if (ballptr->col < 0) {
        ballptr->col = 0;
        ballptr->xDir *= -1;
    }

    if (ballptr->row < 0) {
        ballptr->row = 0;
        ballptr->yDir *= -1;
    }

    if (ballptr->col + BALLSIZE > SCREENWIDTH) {
        ballptr->col = SCREENWIDTH - BALLSIZE;
        ballptr->xDir *= -1;
    }

    // Collision resolution for when bottom of ball hits top of slider
    if ((ballptr->row + BALLSIZE) > sldptr->row
            && ballptr->col >= (sldptr->col - BALLSIZE)
            && ballptr->col <= (sldptr->col + SLIDER_WIDTH)) {
        ballptr->row = sldptr->row - BALLSIZE;
        ballptr->yDir *= -1;
        if (ballptr->xDir > 0 && sldptr->cdel > 0) {
            ballptr->xDir = 1;
        } else if (ballptr->xDir < 0 && sldptr->cdel > 0) {
            ballptr->xDir = 1;
        } else if (ballptr->xDir > 0 && sldptr->cdel < 0) {
            ballptr->xDir = -1;
        } else {
            ballptr->xDir = -1;
        }
    }

}

/**
 * Function to generate bricks on the level
 *
 * @param brptr the pointer to the first brick of the array of bricks
 * @param numBricks the number of bricks on the screen
 */
void generateBricks(Brick *brptr, int numBricks) {
    u16 colors[] = {RED, DKGRAY, GREEN, BLUE, MAGENTA, YELLOW, WHITE};
    int numcolors = sizeof(colors)/sizeof(colors[0]);
    int row = 25;
    int col = 45;
    for (int i = 0; i < numBricks; i++) {
        Brick *cur = brptr + i;
        u16 color = colors[rand() % numcolors];
        cur->row = row;
        cur->col = col;
        cur->height = BRICKHEIGHT;
        cur->width = BRICKWIDTH;
        cur->color = color;
        cur->isHit = 0;
        col = col + 30 + (cur->width);
        if ((i + 1) % 3 == 0) {
            row += BRICKHEIGHT + 3;
            col = 45;
        }
    }
}

/**
 * Function to handle ball collision with a brick
 *
 * @param brptr the pointer to the brick
 * @param ballptr the pointer to the ball
 * @param numBricks the pointer to the number of bricks to generate
 * @param bricksSize the number of bricks in the array
 */
void handleBrickCollisions(Brick *brptr, Ball *ballptr, int *numBricks, int bricksSize) {

    for (int i = 0; i < bricksSize; i++) {
        Brick *cur = brptr + i;
        if (cur->isHit) {
            drawRect(cur->row, cur->col, BRICKHEIGHT, BRICKWIDTH, BGCOLOR);
            for (int j = i; j < bricksSize - 1; j++) {
                brptr[j] = brptr[j + 1];
            }
            bricksSize = *(numBricks);
            i--;
        } else {
            if (((ballptr->col == (cur->col + BRICKWIDTH))
                    || ((ballptr->col + BALLSIZE) == cur->col))
                    && ((ballptr->row <= (cur->row + BRICKHEIGHT))
                    && (ballptr->row >= (cur->row - BALLSIZE)))) {
                *(numBricks) -= 1;
                cur->isHit = 1;
                ballptr->xDir *= -1;
                ballptr->yDir *= ((rand() % 3) - 1);
                if (ballptr->yDir == 0) {
                    ballptr->yDir = 1;
                }
                ballptr->col += ballptr->xDir;
                ballptr->row += ballptr->yDir;
            } else if ((((ballptr->row + BALLSIZE) == cur->row)
                        || (ballptr->row == (cur->row + BRICKHEIGHT)))
                        && ((ballptr->col >= (cur->col - BALLSIZE))
                        && (ballptr->col <= (cur->col + BRICKWIDTH)))) {
                *(numBricks) -= 1;
                cur->isHit = 1;
                ballptr->yDir *= -1;
                ballptr->xDir *= ((rand() % 3) - 1);
                if (ballptr->xDir == 0) {
                    ballptr->xDir = 1;
                }
                ballptr->row += ballptr->yDir;
                ballptr->col += ballptr->xDir;
            }
            drawRect(cur->row, cur->col, BRICKHEIGHT, BRICKWIDTH, cur->color);
        }
    }

}
