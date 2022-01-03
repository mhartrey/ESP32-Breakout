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

### Upload and Serial via ESP-PROG
It is possible to use a single USB connection to the ESP-PROG with debugging, program uploads and serial interface all using this single interface.

To do this:
- Assuming you want to power the ESP32 from the ESP-PROG connect VCC of ESP-PROG (Pin 1) to VCC of ESP32 <br/> **Note: DO NOT power ESP32 via USB at the same time!!!**
- Set <code> upload_protocol = esp-prog</code> in platformio.ini
- Connect RX & TX from ESP-PROG to ESP32 (See below)

### Summary of ESP-PROG connections
A total of 8 wires are required (if uploading and powering via ESP-PROG USB only)
<br>
2x5 connector:
<pre>
Pin 1 - VDD (VCC) <--> ESP32 VCC (Only connect if powering ESP32)
Pin 2 - JTAG - ESP-TMS <--> ESP32 GPIO 14
Pin 3 - GND (unconnected)
Pin 4 - JTAG - ESP-TCK <--> ESP32 GPIO 13
Pin 5 - GND (unconnected)
Pin 6 - JTAG - ESP-TDO <--> ESP32 GPIO 15
Pin 7 - GND (unconnected)
Pin 8 - JTAG - ESP-TDI <--> ESP32 GPIO 12
Pin 9 - GND
Pin 10 - NC
</pre>
2x3 connector (only required if using serial interface via ESP-PROG):
<pre>
Pin 1 - ESP_EN (unconnected)
Pin 2 - 3.3V (unconnected)
Pin 3 - ESP_TXD <--> ESP32-TX (only needed if serial via ESP-PROG)
Pin 4 - GND (unconnected)
Pin 5 - ESP_RXD <--> ESP32-RX (only needed if serial via ESP-PROG)
Pin 6 - ESP_IO0 (unconnected)
</pre>

