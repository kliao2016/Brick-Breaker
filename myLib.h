typedef unsigned int u32;
typedef unsigned short u16;


#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define SCREENHEIGHT 149

#define BG2_ENABLE (1<<10)
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0,0,31)
#define YELLOW COLOR(31,31,0)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define DKGRAY COLOR(15, 15, 15)


#define OFFSET(row, col, rowlen)  ((row)*(rowlen)+(col))


/* Buttons */

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130

/* DMA */
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)


#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

extern unsigned short *videoBuffer;
extern const unsigned char fontdata_6x8[12288];

/* Prototypes */
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, unsigned short color);
void drawImage3(int row, int col, int height, int width, const u16* image);
void waitForVblank();
void drawChar(int row, int col, char ch, unsigned short color);
void drawString3(int row, int col, char str[], unsigned short color);
void fillScreen(volatile u16 color);
