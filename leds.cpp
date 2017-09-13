#include <stdio.h>
#include "leds.h"

#include <dynamixel.h>

static char leds[12];
static bool leds_custom_flag;

extern uart_half_duplex_t stream;

static int led_value_to_dxl(int val)
{
    char dxlv = 0;
    if (val & LED_R) dxlv |= 1;
    if (val & LED_G) dxlv |= 2;
    if (val & LED_B) dxlv |= 4;
    return dxlv;
}

char leds_are_custom()
{
    return leds_custom_flag;
}

void leds_decustom()
{
    leds_custom_flag = false;
}

void led_set(int index, int value, bool custom)
{
    if (custom) {
        leds_custom_flag = true;
    }
    leds[index-1] = value;

    dynamixel_t dev;
    dynamixel_init(&dev, &stream, index);
    dynamixel_write8(&dev, XL320_LED, led_value_to_dxl(value));
}

void led_set_all(int value, bool custom)
{
    if (custom) {
        leds_custom_flag = true;
    }
    for (unsigned int i=0; i<sizeof(leds); i++) {
        leds[i] = value;
    }

    dynamixel_t dev;
    dynamixel_init(&dev, &stream, 0xFF);
    dynamixel_write8(&dev, XL320_LED, led_value_to_dxl(value));
}
