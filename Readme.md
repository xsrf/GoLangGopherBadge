# Go Gopher PCB Badge
Small Badge based on CH32V003A4M6 and 25 SK9814-EC10 RGB LEDs.
Designed with ❤ for [@joschahenningsen](https://github.com/joschahenningsen) and friends.

<img src="pcb/preview_front.svg" width="33%">

## USB Programming
The Badge features a USB-C port and USB bootloader that allows for flashing custom code without special hardware. The bootloader is based on https://github.com/cnlohr/rv003usb 

Turn the Badge on while connected to USB to boot into the bootloader.

## CH32V003A4M6
<img src="https://raw.githubusercontent.com/Tengo10/pinout-overview/main/pinouts/CH32v003/ch32v003j4m6.svg">

GND: Pin 2
Vcc: Pin 4
USB: Pin 5,6,7 (PC1 PC2 PC4)
LEDs: Pin 1 (PA1)
BTN_A: Pin 8 (PD1)
BTN_B: Pin 3 (PA2)