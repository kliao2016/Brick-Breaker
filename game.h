/* Game Structs */
typedef struct {
    int height;
    int width;
    int row;
    int col;
    unsigned short color;

} Brick;

typedef enum {
    START,
    START_NODRAW,
    LEVEL1,
    LEVEL1_NODRAW,
    GAME_OVER,
    GAME_OVER_NO_DRAW

} GAMEState;

typedef struct {
    int height;
    int width;
    int row;
    int col;
    int oldRow;
    int oldCol;
    unsigned short color;

} Slider;

typedef struct {
    int row;
    int col;
    int size;
    int oldRow;
    int oldCol;
    int xDir;
    int yDir;
    unsigned short color;
} Ball;

/* Prototypes */
void drawSlider(Slider *sldptr);
void enableSlider(Slider *sldptr);
void createBall(int row, int col, int size, Ball *ballptr);
void startLevel(Ball *ballptr, Slider *sldptr);
void handleCollisions(Ball *ballptr, Slider *sldptr);
void generateBricks(int numBricks);
