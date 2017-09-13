#include "leds.h"
#include "mapping.h"
#include "motors.h"

static bool is_enabled = false;

void motors_colorize()
{
    if (motors_enabled()) {
        // Colorizing the front of the robot
        colorize();
    } else {
        // Turning all leds red
        led_set_all(LED_R);
    }
}

void motors_enable()
{
    // Motors are enabled
    is_enabled = true;
    motors_colorize();
}

void motors_disable()
{
    // Motors are disabled
    is_enabled = false;
    // Marking leds as non custom
    motors_colorize();
}

bool motors_enabled()
{
    return true;
}
