/* Game Structs */
typedef struct {
    int height;
    int width;
    int row;
    int col;
    int isHit;
    unsigned short color;

} Brick;

typedef enum {
    START,
    START_NODRAW,
    LEVEL,
    LEVEL_NODRAW,
    NEXT_LEVEL,
    NEXT_LEVEL_NO_DRAW,
    LOSE_LIFE,
    LOSE_LIFE_NO_DRAW,
    GAME_OVER,
    GAME_OVER_NO_DRAW,
    WIN_GAME,
    WIN_GAME_NO_DRAW,

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
void ballMovement(Ball *ballptr, Slider *sldptr, Brick *brptr, int *numBricks, int bricksSize);
void handleCollisions(Ball *ballptr, Slider *sldptr);
void generateBricks(Brick *brptr, int numBricks);
void handleBrickCollisions(Brick *brptr, Ball *ballptr, int *numBricks, int bricksSize);
