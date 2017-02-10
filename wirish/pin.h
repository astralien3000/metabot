/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file wirish/include/wirish/io.h
 * @brief Wiring-style pin I/O interface.
 */

#ifndef _WIRISH_IO_H_
#define _WIRISH_IO_H_

#include <libmaple/libmaple_types.h>

#include <periph/gpio.h>

typedef enum WiringPinMode {
    OUTPUT,
    OUTPUT_OPEN_DRAIN,
    INPUT,
    INPUT_ANALOG,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    INPUT_FLOATING,
    PWM,
    PWM_OPEN_DRAIN,
} WiringPinMode;

inline void pinMode(uint8 pin, WiringPinMode mode) {
  if(mode == OUTPUT && pin == 28) {
    gpio_init(GPIO_PIN(PORT_B, 5), GPIO_OUT);
  }
  else {
  }
}

#define HIGH 0x1
#define LOW  0x0

inline void digitalWrite(uint8 pin, uint8 value) {
  if(pin == 28 && value == HIGH) {
    gpio_set(GPIO_PIN(PORT_B, 5));
  }
  else   if(pin == 28 && value == LOW) {
    gpio_clear(GPIO_PIN(PORT_B, 5));
  }
  else {
  }
}

//inline uint32 digitalRead(uint8 pin);
//inline uint16 analogRead(uint8 pin);
//inline void togglePin(uint8 pin);
//inline void toggleLED();
//uint8 isButtonPressed(uint8 pin=BOARD_BUTTON_PIN, uint32 pressedLevel=BOARD_BUTTON_PRESSED_LEVEL);
//uint8 waitForButtonPress(uint32 timeout_millis=0);
//void shiftOut(uint8 dataPin, uint8 clockPin, uint8 bitOrder, uint8 value);

#define AFIO_REMAP_USE_MAPR2            (1U << 31)
#define AFIO_REMAP_USART1 AFIO_MAPR_USART1_REMAP

inline void afio_remap(uint32 remapping) {
  if (remapping & AFIO_REMAP_USE_MAPR2) {
    remapping &= ~AFIO_REMAP_USE_MAPR2;
    AFIO->MAPR2 |= remapping;
  } else {
    AFIO->MAPR |= remapping;
  }
}

#endif
