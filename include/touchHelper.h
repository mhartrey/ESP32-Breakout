
// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData1.txt"

// Set REPEAT_CALIBRATION to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CALIBRATION false

void mountLITTLEFS()
{
  // Initialise LITTLEFS system
  if (!LITTLEFS.begin(false))
  {
    Serial.println("LITTLEFS Mount Failed.");
    while (1)
      delay(1);
  }
  else
  {
    Serial.println("LITTLEFS Mount Successful.");
  }
}

void touch_calibrate()
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  mountLITTLEFS();

  // check if calibration file exists and size is correct
  if (LITTLEFS.exists(CALIBRATION_FILE))
  {
    if (REPEAT_CALIBRATION)
    {
      // Delete existing calibration file as we want to re-calibrate
      LITTLEFS.remove(CALIBRATION_FILE);
      Serial.println("Deleted touch calibration file");
    }
    else
    {
      // Open existing calibration file
      File f = LITTLEFS.open(CALIBRATION_FILE, "r");
      if (f)
      {
        if (f.readBytes((char *)calData, 14) == 14)
        {
          calDataOK = 1;
          Serial.println("Successfully read touch calibration file");
        }
        f.close();
      }
      // Dump calData to serial
      Serial.println("Dumping calData:");
      for (int i = 0; i < 5; i++)
      {
        Serial.printf("%04X, ", calData[i]);
      }
      Serial.println("");
    }
  }

  if (calDataOK && !REPEAT_CALIBRATION)
  {
    // calibration touch with existing data
    tft.setTouch(calData);
    Serial.println("Applied existing touch calibration");
  }
  else
  {
    // data not valid so recalibrate
    Serial.println("Need to re-calibrate");

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CALIBRATION)
    {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CALIBRATION to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store calibration data
    File f = LITTLEFS.open(CALIBRATION_FILE, "w");
    if (f)
    {
      f.write((const unsigned char *)calData, 14);
      f.close();
      Serial.println("Saved touch calibration data");
    }
    // Dump calData to serial
    Serial.println("Dumping calData:");
    for (int i = 0; i < 5; i++)
    {
      Serial.printf("%04X, ", calData[i]);
    }
    Serial.println("");
  }
}
