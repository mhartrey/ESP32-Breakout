#include "main.h"

// Forward declarations
void drawLives();
void drawScore();
void drawLevel();
void initGameBoard();
boolean checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
void moveBat();
void moveBall();
void clearOldBallPos();
void checkHitBat();
boolean checkBallLost();
void newBall();

void initGame()
{
    tft.fillScreen(TFT_BLACK);
    tft.drawFastHLine(0, topBorder - 1, tftWidth, TFT_BLUE);
    tft.setCursor(0, 5);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print("SCORE :");
    tft.setCursor((tftWidth / 2), 5);
    tft.print("LIVES :");
    tft.setCursor(tftWidth - 75, 5);
    tft.print("LEVEL :");

    batY = tftHeight - batHeight - 30;
    playerLives = 3;
    playerScore = 0;
    level = 0;

    drawLives();
    drawScore();
    drawLevel();

    initGameBoard();
}

void drawLives()
{
    // clear old lives
    tft.fillRect((tftWidth / 2) + 50, 5, 25, 10, TFT_BLACK);

    // print new score
    tft.setCursor((tftWidth / 2) + 50, 5);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print(playerLives);
}

void drawScore()
{
    // clear old score
    tft.fillRect(50, 5, 25, 10, TFT_BLACK);

    // print new score
    tft.setCursor(50, 5);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print(playerScore);
}

void drawLevel()
{
    // clear old level
    tft.fillRect(tftWidth - 25, 5, 25, 10, TFT_BLACK);

    // print new score
    tft.setCursor(tftWidth - 25, 5);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE);
    tft.print(level + 1);
}

// bounding box collision detection
boolean checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
    boolean hit = false;
    if (
        (((x2 + width2) >= x1) &&
         (x2 <= (x1 + width1))) &&
        (((y2 + height2) >= y1) &&
         (y2 <= (y1 + height1))))
    {
        hit = true;
    }

    return hit;
}

void moveBat()
{
    int16_t newBatX;
    if (tft.getTouch(&spX, &spY))
    {
        newBatX = spX - (batWidth / 2);
        if (newBatX < 0)
            newBatX = 0;
        if (newBatX >= (tftWidth - batWidth))
            newBatX = tftWidth - 1 - batWidth;

        if (abs(newBatX - batX) > 4)
        {
            tft.fillRect(batX, batY, batWidth, batHeight, TFT_BLACK);
            batX = newBatX;
            tft.fillRect(batX, batY, batWidth, batHeight, TFT_RED);
        }
    }
}

void moveBall()
{
    float newX, newY;
    newX = xPos + xVel;
    newY = yPos + yVel;
    if (newX < (float)ballSize)
    {
        newX = (float)ballSize;
        xVel = -xVel;
    }
    if (newX > (float)(tftWidth - ballSize - 1))
    {
        newX = (float)(tftWidth - ballSize - 1);
        xVel = -xVel;
    }
    if (newY < topBorder + (float)ballSize)
    {
        newY = topBorder + (float)ballSize;
        yVel = -yVel;
    }
    if ((round(newX) != round(xPosLast)) || (round(newY) != round(yPosLast)))
    {
        // draw ball
        clearOldBallPos();
        tft.fillCircle(round(newX), round(newY), ballSize, TFT_GREEN);
        xPosLast = newX;
        yPosLast = newY;
    }
    xPos = newX;
    yPos = newY;
}

void clearOldBallPos()
{
    tft.fillCircle(round(xPosLast), round(yPosLast), ballSize, TFT_BLACK);
}

void checkHitBat()
{
    // check bat and bottom half of ball
    float xInc;
    boolean hit = checkCollision(batX, batY, batWidth, batHeight, (int)round(xPos) - ballSize, (int)round(yPos), ballSize * 2, ballSize);
    if (hit)
    {
        // reverse ball y direction but increase speed
        yVel += 0.05;
        if (yVel > 5)
        {
            yVel = 5;
        }
        yVel = -yVel;
        // rounded bounce
        xInc = (xPos - (float)(batX + (batWidth / 2))) / (float)batWidth;
        xVel += 6 * xInc;
        if (abs(xVel) > 6)
        {
            if (xVel < 0)
            {
                xVel = -6;
            }
            else
            {
                xVel = 6;
            }
        }
        // make sure ball not hitting bat
        yPos = (float)(batY - ballSize - 1);
    }
}

void initGameBoard()
{
    int row, col;
    int colour = TFT_BLACK; // prevent compiler warning
    int score = 0; // prevent compiler warning
    clearOldBallPos();
    xPosLast = xPos = 0;
    yPosLast = yPos = 90;
    xVel = 2;
    yVel = 2 + (level);
    gameState = 2;

    for (row = 0; row < 5; row++)
    {
        for (col = 0; col < 16; col++)
        {
            switch (row)
            {
            case 0:
            case 1:
                colour = TFT_BLUE;
                score = 50;
                break;
            case 2:
            case 3:
                colour = TFT_MAGENTA;
                score = 30;
                break;
            case 4:
            case 5:
                colour = TFT_YELLOW;
                score = 10;
                break;
            }
            blocks[row][col] = Block(col * 20, (row * 10) + 30, 19, 9, colour, score);
        }
    }
    // Set special blocks here for now
    blocks[3][3].modifyBlock(TFT_GREEN, 100, 2, normal);
    blocks[3][12].modifyBlock(TFT_GREEN, 100, 2, normal);
    blocks[2][6].modifyBlock(TFT_BLACK, 200, 2, normal);
    blocks[2][9].modifyBlock(TFT_BLACK, 200, 2, normal);
    blocks[4][7].modifyBlock(TFT_RED, 10, 3, longbat);
    blocks[4][8].modifyBlock(TFT_RED, 10, 3, longbat);
}

boolean checkBallLost()
{
    if (yPos > tftHeight + ballSize)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void newBall()
{
    xPos = 0;
    yPos = 90;
    xVel = yVel = 2;
    moveBall();
    delay(1000);
}

void checkHitBlock()
{
    int row, col;
    for (row = 0; row < 5; row++)
    {
        for (col = 0; col < 16; col++)
        {
            if (blocks[row][col].isActive && blocks[row][col].isHit(xPos, yPos, ballSize * 2, ballSize * 2))
            {
                //blocks[row][col].removeBlock();
                blocks[row][col].hitBlock(); // Block may be removed depending upon number of hits required
                playerScore += blocks[row][col].score;
                drawScore();
                yVel = -yVel;
                return;
            }
        }
    }
}

boolean checkAllBlocksHit()
{
    int row, col;
    for (row = 0; row < 5; row++)
    {
        for (col = 0; col < 16; col++)
        {
            if (blocks[row][col].isActive)
            {
                return false;
            }
        }
    }
    return true;
}