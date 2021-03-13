// Ensure this header file is included only once
#ifndef _ESP32BREAKOUT_
#define _ESP32BREAKOUT_

// ===================== LittleFS ========================
// SPIFFS (in-memory SPI File System) replacement
#include <LITTLEFS.h>

// ===================== Touch Screen ====================
// Touch Screen Display
#include <TFT_eSPI.h> // Hardware-specific library

// Instantiate screen (object) using hardware SPI. Defaults to 320H x 240W
TFT_eSPI tft = TFT_eSPI();

// Game globals
unsigned long lastFrame = millis(); // Used for frame rate
int16_t tftWidth = 0, tftHeight = 0;
int8_t topBorder = 20;

// Touch coordinates (already in pixels)
uint16_t spX = 0, spY = 0;

// Bat variables
#define NORMALBATWIDTH 30
#define LARGEBATWIDTH 100
#define BATWIDTHTIME 50 * 20 // ~20 seconds
int8_t batWidth = NORMALBATWIDTH;
int8_t batHeight = 7;
int16_t batX = 0, batY = 0;
int16_t batTime = 0; // Remaining time before bat width returns to normal

// Ball variables
float xPos = 0.0, yPos = 0.0;
float xPosLast = 0.0, yPosLast = 0.0;
float xVel = 0.0, yVel = 0.0;
int8_t ballSize = 3;

int playerLives = 3;
int playerScore = 0;
int level = 0;
int gameState = 1; // 1=start 2=playing 3=gameover

// Forward declaration
boolean checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);

enum BlockType { normal, longbat };

class Block
{
public:
    int x;
    int y;
    int width;
    int height;
    int colour;
    int score;
    int hits;  // Number of hits to remove block
    BlockType type;
    boolean isActive;

    // default constructor
    Block() {}

    Block(int xpos, int ypos, int bwidth, int bheight, int bcol, int bscore)
    {
        x = xpos;
        y = ypos;
        width = bwidth;
        height = bheight;
        colour = bcol;
        score = bscore;
        hits = 1; // Default of 1 hit
        type = normal;
        isActive = true;
        drawBlock();
    }
    void drawBlock()
    {
        tft.fillRect(x, y, width, height, colour);
    }

    void modifyBlock(int bcol, int bscore, int bhits, BlockType btype)
    {
        colour = bcol;
        score = bscore;
        hits = bhits;
        type = btype;
        drawBlock();
    }

    void removeBlock()
    {
        tft.fillRect(x, y, width, height, TFT_BLACK);
        isActive = false;
    }

    void hitBlock()
    {
        if (--hits == 0)
        {
            removeBlock();

            if (type == longbat)
            {
                batWidth = LARGEBATWIDTH;
                batTime = BATWIDTHTIME;
            }
        }
    }

    boolean isHit(float x1, float y1, int w1, int h1)
    {
        if (checkCollision((int)round(x1), (int)round(y1), w1, h1, x, y, width, height))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

Block blocks[5][16];

#endif
