#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "matrix.h"

/*
    PINs
    ----
    BTN_A   PD4/PD5/PD1 (also SWIO on PD1)
    BTN_B   PA2
    LEDs    PA1/PD6

*/

extern "C" void __cxa_pure_virtual() { while (1); }

const uint8_t LEDPIN = 1; // GPIO 1 on Port A
const uint8_t NUMLEDS = 25;

Matrix matrix;
const char scrollingText[] = "MOIN JOSCHA! HAPPY BIRTHDAY!";
uint16_t scrollPosition = 0;


struct __attribute__((packed)) pixel {
    uint16_t b;
    uint16_t r;
    uint16_t g;
};

struct __attribute__((packed)) cgain {
    unsigned int filled : 1; // Dummy to fill 16 Bits
    unsigned int b : 5;
    unsigned int r : 5;
    unsigned int g : 5;
};

struct databuffer {
    pixel LEDS[NUMLEDS];
    cgain GAIN;
};

databuffer buf;
pixel blackPixel;

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
    buf.LEDS[0].r = 0xFF00;
    buf.LEDS[0].g = 0x0000;
    buf.LEDS[0].b = 0x0000;
    buf.LEDS[1].r = 0x0000;
    buf.LEDS[1].g = 0xFF00;
    buf.LEDS[1].b = 0x0000;
    buf.LEDS[2].r = 0x0000;
    buf.LEDS[2].g = 0x0000;
    buf.LEDS[2].b = 0xFF00;

    // 0=0.71mA, 6=3mA, 11=4.7mA
    buf.GAIN.r = 0; buf.GAIN.g = 0; buf.GAIN.b = 0; // lowest gain possible to save power (is still bright)

    // Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD;

    // GPIO A1 Push-Pull (LEDs)
	GPIOA->CFGLR &= ~(0xf<<(4*LEDPIN));
	GPIOA->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LEDPIN);

    // GPIO A2 Pull-Up (BTN_B)
	GPIOA->CFGLR &= ~(0xf<<(4*2));
	GPIOA->CFGLR |= (GPIO_SPEED_IN | GPIO_CNF_IN_PUPD)<<(4*2);
    GPIOA->BSHR = 1<<2; // Set High (Pull-Up)

    // GPIO D1 Pull-Up (BTN_A)
	GPIOD->CFGLR &= ~(0xf<<(4*1));
	GPIOD->CFGLR |= (GPIO_SPEED_IN | GPIO_CNF_IN_PUPD)<<(4*1);
    GPIOD->BSHR = 1<<1; // Set High (Pull-Up)

    blackPixel.r = 0;
    blackPixel.g = 0;
    blackPixel.b = 0;
    initScroll();
}

int led_index = 0;
uint8_t cnt = 0;

//      SK9814
//      Bits encoded as pulses as follows:
//
//      "0":
//         +--------+              +--
//         |        |              |
//         |        |              |
//         |        |              |
//      ---|        |--------------|
//         +        +              +
//         | 0.24us |   0.48 0us   |
//
//      "1":
//         +-------------+        +--
//         |             |        |
//         |             |        |
//         |             |        |
//         |             |        |
//      ---+             +--------+
//         |    0.64us   | 0.24us |



void sendData() {
    uint16_t i=0; // i iterates over all Bits
    for (i=0; i < NUMLEDS*3*16 + 15; i++) { // LEDs * 3 Channels * 16 Bis per channel + 15 Current Gain bits
        if((*((uint16_t*)&buf + (i>>4))) & (0x8000 >> (i&0xF))) { // i>>4 addresses the 16
            // Queue 1
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x10000 << LEDPIN);
        } else {
            // Queue 0
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x10000 << LEDPIN);
        }
    }
}


pixel hue2rgb(uint16_t hue, uint16_t brightness = 65535) {
    // https://blog.adafruit.com/2012/03/14/constant-brightness-hsb-to-rgb-algorithm/
    const uint16_t step = 65535/3;
    pixel p;
    if(0 <= hue && hue < step) {
        p.b = 0;
        p.g = hue*3;
        p.r = 65535 - hue*3;
    } else 
    if(step <= hue && hue < step*2) {
        p.r = 0;
        p.b = (hue-step)*3;
        p.g = 65535 - (hue-step)*3;
    } else 
    if(step*2 <= hue && hue < step*3) {
        p.g = 0;
        p.r = (hue-step*2)*3;
        p.b = 65535 - (hue-step*2)*3;
    } else {
        p.g = 0;
        p.b = 0;
        p.r = 65535;
    }
    p.r = p.r * brightness / 65535;
    p.g = p.g * brightness / 65535;
    p.b = p.b * brightness / 65535;
    return p;
}

boolean Btn_A() {
    return !(GPIOD->INDR & 1<<1);
}

boolean Btn_B() {
    return !(GPIOA->INDR & 1<<2);
}

uint16_t i = 0;
uint16_t loops = 0;
long lastMillis = 0;
void loop()
{
    //matrix.drawLine(1,1,4,4,0xFFFF);
    for(uint k = 0; k < NUMLEDS; k++) {
        buf.LEDS[k] = matrix.leds[k] ? hue2rgb(i + (65535/NUMLEDS)*k, 10000) : blackPixel;
        //if(k==0) buf.LEDS[k] = hue2rgb(i + (65535/NUMLEDS)*k, 10000);
    }
    i+=255;
    sendData();
    delay(10);
    if(loops%8 == 0) {
        matrix.drawLine(4,1,1,4,0xFFFF);
        if(!isTextScrolling()) initScroll();
        scrollText();
    }
    while(Btn_A());
    while(Btn_B());
    loops++;
}

// 4,7 / 17,4
// 0.78 / 2.9