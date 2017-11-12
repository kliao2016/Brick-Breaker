#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

/**
 * Function to set a pixel at (row, col) on the screen to a specific color
 *
 * @param row   the row of the pixel
 * @param col   the column of the pixel
 * @param color the color to set the pixel to
 */
void setPixel(int row, int col, u16 color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

/**
 * Function to draw rectangle with given width and height
 *
 * @param row    the row to draw the rectangle
 * @param col    the column to draw the rectangle
 * @param height the height of the rectangle
 * @param width  the width of the rectangle
 * @param color  the color of the rectangle
 */
void drawRect(int row, int col, int height, int width, volatile u16 color) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = (width) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
	}
}

/**
 * Function to draw arbitrary sized image
 *
 * @param row    the row to draw the image
 * @param col    the column to draw the image
 * @param width  the width of the image
 * @param height the height of the image
 * @param image  the pointer to the first element of the image
 */
void drawImage3(int row, int col, int height, int width, const u16* image) {
	for (int r = 0; r < height; r++) {
		DMA[3].src = (image + (r * width));
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = (width) | DMA_ON | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}

/**
 * Function to sync screen refresh rate to 60 fps
 */
void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 161);
}

/**
 * Function to draw a character on the screen with color at (row, col)
 * @param row   the row of the character
 * @param col   the column of the character
 * @param ch    the character to draw
 * @param color the color of the character being drawn
 */
void drawChar(int row, int col, char ch, u16 color) {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			if (fontdata_6x8[OFFSET(r, c, 6) + ch*48] == 1) {
				setPixel(row +r, col + c, color);
			}
		}
	}
}

/**
 * Function to draw a string on the screen with color at (row, col)
 * @param row   the row of the character
 * @param col   the column of the character
 * @param str   the string to draw
 * @param color the color of the character being drawn
 */
void drawString3(int row, int col, char str[], u16 color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}

/**
 * Function to fill the gba screen with the specified color
 *
 * @param color the color used to fill the screen
 */
void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = (160*240) | DMA_ON | DMA_DESTINATION_INCREMENT | DMA_SOURCE_FIXED;
}
