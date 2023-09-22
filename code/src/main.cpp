#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "matrix.h"
#include "buttons.h"
#include "leds.h"
#include "misc.h"

/*
    PINs
    ----
    BTN_A   PD4/PD5/PD1 (also SWIO on PD1)
    BTN_B   PA2
    LEDs    PA1/PD6

*/

extern "C" void __cxa_pure_virtual() { while (1); }

unsigned long BTN_PRESS_CNT,GLOBAL_CNT;
byte MODE,SUBMODE = 0;
const uint8_t LONG_PRESS_CNT = 20;

Matrix matrix;
const char scrollingText[] = "MOIN JOSCHA! HAPPY BIRTHDAY!";
uint16_t scrollPosition = 0;

void scrollText() {
    matrix.fillScreen(0);
    matrix.setCursor(4-scrollPosition,7);
    matrix.setTextColor(0xFFFF);
    matrix.print(scrollingText);
    if(matrix.getCursorX() >= 0) scrollPosition++;
}

bool isTextScrolling() {
    if(matrix.getCursorX() < 0) return false;
    return true;
}

void initScroll() {
    scrollPosition = 0;
    matrix.setCursor(0,6); // set here, as it is used to detect text on the screen
}

void setup() 
{
    delay(100);

    // 0=0.71mA, 6=3mA, 11=4.7mA
    buf.GAIN.r = 0; buf.GAIN.g = 0; buf.GAIN.b = 0; // lowest gain possible to save power (is still bright)

    BtnInit();
    LEDsInit();

    initScroll();
}

uint16_t i = 0;
uint16_t loops = 0;

void btnCheck() {
  // Check Button press
  if(BtnA()) {
    BTN_PRESS_CNT++;
    if(BTN_PRESS_CNT%LONG_PRESS_CNT == 0) {
      // Long press action every ~second
      SUBMODE++;
      GLOBAL_CNT = 0;
      pixel c;
      c.r = c.g = c.b = 0xFFFF;
      buf.LEDS[0] = c;
      buf.LEDS[4] = c;
      buf.LEDS[20] = c;
      buf.LEDS[24] = c;
    }
  } else {
    if(BTN_PRESS_CNT > 1 && BTN_PRESS_CNT < LONG_PRESS_CNT) {
      // Single press action
      MODE++;
      SUBMODE = 0; // reset Sub-Mode when changing to next mode/animation
      GLOBAL_CNT = 0;
    }
    BTN_PRESS_CNT = 0;
  }
}


void scrollTextRGB() {
    if(GLOBAL_CNT % 4 == 0) {
        if(!isTextScrolling()) initScroll();
        scrollText();
    }

    for(uint k = 0; k < NUMLEDS; k++) {
        buf.LEDS[k] = matrix.leds[k] ? hue2rgb(GLOBAL_CNT*255 + (65535/NUMLEDS)*k, 10000) : cBlack;
    }
}

void white_sparkles_fade(uint8_t div = 1) {
    if(GLOBAL_CNT % div == 0) for(uint8_t i=0; i<NUMLEDS; i++) buf.LEDS[i] = fadeBlack(buf.LEDS[i]);
    if((GLOBAL_CNT % (2 + SUBMODE)) == 0) {
        pixel c;
        c.g = c.b = 0x3FFF;
        buf.LEDS[getRandomPixel()] = c;
    }
}

void colorbands(uint8_t speed = 4, uint8_t bands = 6) {
    for(uint k = 0; k < NUMLEDS; k++) {
        buf.LEDS[k] = hue2rgb(GLOBAL_CNT*(300-(10*speed)) + (65535/bands)*(k%5), 10000);
    }
}

void heart(uint8_t speed = 4, uint8_t bands = 6) {
    for(uint k = 0; k < NUMLEDS; k++) {
        buf.LEDS[k] = hue2rgb(GLOBAL_CNT*(300-(10*speed)) + (65535/bands)*(k%5), 10000);
    }
    //setColor(buf.LEDS[0], 0, 0, 0);
    /*
    setColor(buf.LEDS[2], 0, 0, 0);
    setColor(buf.LEDS[4], 0, 0, 0);
    setColor(buf.LEDS[15], 0, 0, 0);
    setColor(buf.LEDS[19], 0, 0, 0);
    setColor(buf.LEDS[20], 0, 0, 0);
    setColor(buf.LEDS[21], 0, 0, 0);
    setColor(buf.LEDS[23], 0, 0, 0);
    setColor(buf.LEDS[24], 0, 0, 0);
    */
}

void loop()
{
    switch (MODE) {
        case __COUNTER__: scrollTextRGB(); break;
        case __COUNTER__: white_sparkles_fade(1); break;
        case __COUNTER__: white_sparkles_fade(2); break;
        case __COUNTER__: white_sparkles_fade(4); break;
        case __COUNTER__: colorbands(SUBMODE+1,5); break;
        case __COUNTER__: colorbands(SUBMODE+1,10); break;
        case __COUNTER__: colorbands(SUBMODE+1,100); break;
        default: MODE = 0; break;
    }
    btnCheck();
    sendData();
    GLOBAL_CNT++;
    delay(20); // 50fps
}
