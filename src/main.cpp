#include <Arduino.h>
#include "main.h"
#include "touchHelper.h"
#include "screenHelper.h"

void setup()
{
  Serial.begin(115200);
  log_d("Initialise screen");

  tft.init();

  // Set rotation as required before calibration, the touch calibration MUST match this
  // 0 & 2 Portrait. 1 & 3 landscape
  tft.setRotation(3);

  // call screen calibration
  touch_calibrate();

  tft.fillScreen(TFT_BLACK);
  tftWidth = tft.width();
  tftHeight = tft.height();
}

void loop()
{

  // limit frame rate to 50 fps
  while ((millis() - lastFrame) < 20)
    ;
  lastFrame = millis();

  switch (gameState)
  {
  case 1: // start
    gameState = 11;
    break;

  case 2: // play

    moveBall();
    moveBat();
    checkHitBat();
    checkHitBlock();

    if (checkBallLost())
    {
      playerLives--;
      drawLives();
      if (playerLives > 0)
      {
        newBall();
      }
      else
      {
        gameState = 3; // end game
      }
    }
    if (checkAllBlocksHit())
    {
      gameState = 4;
    }

    // Need to return bat to normal size at some point
    if((batWidth != NORMALBATWIDTH) && (--batTime == 0))
    {
      // Reset bat size
      batWidth = NORMALBATWIDTH;
      moveBat(); // Fudge for now - really need to redraw Bat when it changes size
    }

    break;

  case 3: // end
    tft.fillScreen(TFT_BLACK);
    tft.setCursor((tftWidth / 2) - 150, 50);
    tft.setTextSize(3);
    tft.setTextColor(TFT_WHITE);
    tft.print("You Scored ");
    tft.print(playerScore);
    gameState = 11; // click to play
    break;

  case 4: // new blocks
    delay(1000);
    level++;
    drawLevel();
    initGameBoard();
    break;

  case 11: // click to play
    tft.fillRect((tftWidth / 2) - 100, (tftHeight / 2) - 20, 200, 40, TFT_GREEN);
    tft.setCursor((tftWidth / 2) - 100 + 25, (tftHeight / 2) - 20 + 12);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.print("CLICK TO PLAY");
    gameState = 12;
    break;

  case 12: // wait for click to play
    if (tft.getTouch(&spX, &spY))
    {
      //log_d("(%d,%d)", spX, spY);
      if (checkCollision(spX, spY, 1, 1, (tftWidth / 2) - 50, (tftHeight / 2) - 20, 100, 40))
      {
        initGame();
        gameState = 2;
      }
    }
    break;
  }
}
