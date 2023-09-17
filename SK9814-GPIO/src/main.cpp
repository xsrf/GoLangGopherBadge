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
const uint8_t NUMLEDS = 3;

uint8_t currentgain[] =  { 0x02, 0x02, 0x02 };  // BRG value

struct pixel {
    uint16_t b;
    uint16_t r;
    uint16_t g;
};

struct cgain {
    uint8_t b;
    uint8_t r;
    uint8_t g;
};

struct pixel LEDS[NUMLEDS];
uint8_t LEDGAIN; // will set same for r,g,b. 5 bits only.

void setup() 
{
    delay(100);
    LEDS[0].r = 0xFF00;
    LEDS[0].g = 0x0000;
    LEDS[0].b = 0x0000;
    LEDS[1].r = 0x0000;
    LEDS[1].g = 0xFF00;
    LEDS[1].b = 0x0000;
    LEDS[2].r = 0x0000;
    LEDS[2].g = 0x0000;
    LEDS[2].b = 0xFF00;

    LEDGAIN = 1; // 1=0.71mA, 7=3mA, 13=5mA

    //pinMode(LEDPIN,OUTPUT);

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

void inline sendBit0() {
    GPIOA->BSHR = ((uint32_t)0x0001 << LEDPIN); // target 240ns
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BCR = ((uint32_t)0x0001 << LEDPIN); // target 480ns
}

void inline sendBit1() {
    GPIOA->BSHR = ((uint32_t)0x0001 << LEDPIN); // target 640ns
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BSHR = 0;
    GPIOA->BCR = ((uint32_t)0x0001 << LEDPIN); // target 240ns
}


void sendDataX() {
    uint16_t col, bit;
    int i=0;
    for (i=0; i < NUMLEDS*3; i++) {
        col = *((uint16_t*)&LEDS + i);
        for (bit=0; bit<16; bit++){
            if ( (col & (0x8000 >> bit) )  ) { // && (led == led_index)
                sendBit1();
            } else {
                sendBit0();
            }
        }
    }
    for (i=0; i < 3; i++) {
        for (bit=0; bit<5; bit++){
            if ( (LEDGAIN & (0x10 >> bit) )  ) { // && (led == led_index)
                sendBit1();
            } else {
                sendBit0();
            }
        }
    }
}


void sendData() {
    uint16_t col, bit;
    int i=0;

    for (i=0; i < NUMLEDS*3*16; i++) {
        if((*((uint16_t*)&LEDS + (i>>4))) & (0x8000 >> (i&4))) {
            // Queue 1
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = ((uint32_t)0x10000 << LEDPIN);
        } else {
            // Queue 0
            GPIOA->BSHR = ((uint32_t)0x00001 << LEDPIN);
            GPIOA->BSHR = 0;
            GPIOA->BSHR = ((uint32_t)0x10000 << LEDPIN);
        }
    }


    for (i=0; i < 3; i++) {
        for (bit=0; bit<5; bit++){
            if ( (LEDGAIN & (0x10 >> bit) )  ) { // && (led == led_index)
                sendBit1();
            } else {
                sendBit0();
            }
        }
    }
}




void loop()
{
    // Init data with only one led ON
    //digitalWrite(LEDPIN,HIGH);
    //delay(1);
    //sendData();
    //digitalWrite(LEDPIN,LOW);
    /*
    sendBit(0);
    sendBit(0);
    sendBit(1);
    sendBit(1);
    sendBit(0);
    sendBit(1);
    sendBit(0);
    sendBit(1);
    */
    debugT();
    sendData();
    delay(1);
}

// 4,7 / 17,4
// 0.78 / 2.9