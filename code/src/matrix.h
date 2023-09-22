#pragma once

#include "Adafruit_GFX.h"
#include "Font5x5Fixed.h"


class Matrix : public Adafruit_GFX {
    public:
        Matrix();
        void clearDisplay(void);
        void drawPixel(int16_t x, int16_t y, uint16_t color);
        uint8_t getPixel(int16_t x, int16_t y);
        uint8_t leds[25];
    private:
        const uint8_t _d = 5;
};

Matrix::Matrix() : Adafruit_GFX(5, 5) {
    //setFont(&FontWordClock4x7);
    setFont(&Font5x5Fixed);
    setTextWrap(false);
}


void Matrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if(x < 0 || y < 0) return;
    if(x >= _d || y >= _d) return;
    leds[y*_d + x] = color ? 255 : 0;
}
