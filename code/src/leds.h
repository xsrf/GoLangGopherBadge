#pragma once

#include "Arduino.h"
#include "misc.h"

const uint8_t NUMLEDS = 25;


const uint8_t LED_PIN = 1;
GPIO_TypeDef* LED_PORT = GPIOA;

struct pixel {
    uint16_t b;
    uint16_t r;
    uint16_t g;
};

struct cgain {
    unsigned int filled : 1; // Dummy to fill 16 Bits
    unsigned int b : 5;
    unsigned int r : 5;
    unsigned int g : 5;
};

struct databuffer {
    pixel LEDS[NUMLEDS] __attribute__((packed)) ;
    cgain GAIN __attribute__((packed));
};

databuffer buf __attribute__((aligned(64)));
pixel cBlack;
pixel cWhite;

void LEDsInit() {
    // Enable GPIOs
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;

    // GPIO A1 Push-Pull (LEDs)
    LED_PORT->CFGLR &= ~(0xf<<(4*LED_PIN));
    LED_PORT->CFGLR |= (GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)<<(4*LED_PIN);

    // Standard Colors
    cWhite.r = cWhite.g = cWhite.b = 0x00FF;
    cBlack.r = cBlack.g = cBlack.b = 0;
    buf.GAIN.r = buf.GAIN.g = buf.GAIN.b = 0;
    }

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
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x10000 << LED_PIN);
        } else {
            // Queue 0
            LED_PORT->BSHR = ((uint32_t)0x00001 << LED_PIN);
            LED_PORT->BSHR = 0;
            LED_PORT->BSHR = ((uint32_t)0x10000 << LED_PIN);
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


uint8_t getRandomPixel() {
  return floor( rand16()/(0xFFFF/(NUMLEDS-1)) );
}

pixel fadeBlack(pixel color) {
    color.r = color.r >> 1;
    color.g = color.g >> 1;
    color.b = color.b >> 1;
    return color;
}

void setColor(pixel &c, uint16_t r, uint16_t g, uint16_t b) {
    c.r = r;
    c.g = g;
    c.b = b;
}