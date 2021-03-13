# ESP32-Breakout
## Breakout game on an ESP32 using an ILI9341 touch screen

<p align="center">
<img src="/images/ESP32-Breakout.jpg" width="50%">
</p>  
<p align="center"> 
<em>ESP32 with ILI9341 touch screen and ESP-PROG debugger (using JTAG pins)</em>
</p>

### Based on code from Bytes N Bits - https://www.youtube.com/watch?v=Oh9vgomyuOI

Ported to ESP32 and using PlatformIO IDE

Using the Bodmer TFT_eSPI library
Touch screen calibration stored in LITTLEFS (SPIFFS)
- to recalibrate set REPEAT_CALIBRATION to true in touchHelper.h
