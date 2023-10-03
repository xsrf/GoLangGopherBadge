#include "Arduino.h"


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

const uint8_t LEDPIN = 1;
const uint8_t NUMLEDS = 25;

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
    //buf.GAIN = (1 << 1) | (1 << 6) | (3 << 11);
    //buf.GAIN = 0x03;
    buf.GAIN.r = 0; buf.GAIN.g = 0; buf.GAIN.b = 0;
    // Enable GPIOs
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;

    // GPIO A1 Push-Pull
	GPIOA->CFGLR &= ~(0xf<<(4*LEDPIN));
	GPIOA->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LEDPIN);

    // GPIO A2 Push-Pull
	GPIOA->CFGLR &= ~(0xf<<(4*2));
	GPIOA->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*2);
}

int led_index = 0;
uint8_t cnt = 0;

void inline debugT() {
    GPIOA->BSHR = ((uint32_t)0x00001 << 2);
    GPIOA->BSHR = ((uint32_t)0x10000 << 2);
}

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

uint16_t i = 0;
void loop()
{
    for(uint k = 0; k < NUMLEDS; k++) {
        //buf.LEDS[k] = hue2rgb(i + (65535/NUMLEDS)*k); // 30mA in total
        buf.LEDS[k] = hue2rgb(i + (65535/NUMLEDS)*k, 10000); // 16mA in total
        //buf.LEDS[k] = hue2rgb(i + (65535/NUMLEDS)*k, 4096); // 14mA in total
    }
    i+=255;
    debugT();
    sendData();
    delay(10);
}

// 4,7 / 17,4
// 0.78 / 2.9