
#include <armory/gpio.h>
#include <armory/adc.h>
#include <armory/pwm.h>
#include <stdint.h>
#include "io.h"

volatile int16_t deltaX = 0;
volatile int16_t deltaY = 0;
volatile PinState joystickPressed = HIGH;

void ioInit(void) {
    gpioInitAll();
    adcInit();

    pwmInitPin(RED_LED);
    gpioPinMode(GREEN_LED, OUTPUT);
    pwmInitPin(BLUE_LED);

    gpioPinMode(JOY_X,  ANALOG);
    gpioPinMode(JOY_Y,  ANALOG);
    gpioPinMode(JOY_SW, INPUT);

    gpioSetPull(JOY_X, NO_PULL);
    gpioSetPull(JOY_Y, NO_PULL);
    gpioSetPull(JOY_SW, PULL_UP);
}

int16_t absVal(int16_t a) {
    if(a < 0) {
        return -a;
    }
    return a;
}

static int16_t scaleJoystick(int raw) {
    int centered = raw - JOY_CENTER;

    if (centered > JOY_DEADZONE) {
        return ((centered - JOY_DEADZONE) * JOY_OUTPUT_MAX) / (JOY_RAW_MAX - JOY_CENTER - JOY_DEADZONE);
    } else if (centered < -JOY_DEADZONE) {
        return ((centered + JOY_DEADZONE) * JOY_OUTPUT_MAX) / (JOY_CENTER - JOY_RAW_MIN - JOY_DEADZONE);
    } else {
        return 0;
    }
}

void readJoystick(void) {
    int rawX = adcReadPin(JOY_X);
    int rawY = adcReadPin(JOY_Y);

    deltaX = scaleJoystick(rawX);
    deltaY = scaleJoystick(rawY);

    joystickPressed = !gpioDigitalRead(JOY_SW); // active low
}
