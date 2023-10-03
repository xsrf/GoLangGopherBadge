#pragma once

#include "Arduino.h"

const uint8_t BTN_A_PIN = 1;
GPIO_TypeDef* BTN_A_PORT = GPIOD;

const uint8_t BTN_B_PIN = 2;
GPIO_TypeDef* BTN_B_PORT = GPIOA;

void BtnInit() {
    // Enable GPIOs
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD;

    // GPIO with Pull-Up
    BTN_B_PORT->CFGLR &= ~(0xf<<(4*BTN_B_PIN));
    BTN_B_PORT->CFGLR |= (GPIO_SPEED_IN | GPIO_CNF_IN_PUPD)<<(4*BTN_B_PIN);
    BTN_B_PORT->BSHR = 1<<BTN_B_PIN;

    // GPIO with Pull-Up
    BTN_A_PORT->CFGLR &= ~(0xf<<(4*BTN_A_PIN));
    BTN_A_PORT->CFGLR |= (GPIO_SPEED_IN | GPIO_CNF_IN_PUPD)<<(4*BTN_A_PIN);
    BTN_A_PORT->BSHR = 1<<BTN_A_PIN;
}

boolean BtnA() {
    return !(BTN_A_PORT->INDR & 1<<BTN_A_PIN);
}

boolean BtnB() {
    return !(BTN_B_PORT->INDR & 1<<BTN_B_PIN);
}