
#include "gpio.h"

#define RED_LED     A8
#define GREEN_LED   A9
#define BLUE_LED   A10

#define JOY_X    A6 
#define JOY_Y    A7 
#define JOY_SW   B5

#define JOY_DEADZONE 400
#define JOY_RAW_MIN  500
#define JOY_RAW_MAX  3000
#define JOY_CENTER   2048
#define JOY_OUTPUT_MAX 127

#define OLED_SCL B6
#define OLED_SDA B7

int16_t absVal(int16_t a) {
    if(a < 0) {
        return -a;
    }
    return a;
}

volatile int16_t deltaX = 0;
volatile int16_t deltaY = 0;
volatile PinState joystickPressed = HIGH;


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

int main() {
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

    while(1) {
        readJoystick();
        gpioWrite(GREEN_LED, joystickPressed);

        uint8_t redBrightness  = absVal(deltaX);
        uint8_t blueBrightness = absVal(deltaY);

        pwmWrite(RED_LED, redBrightness);
        pwmWrite(BLUE_LED, blueBrightness);
    }
    
}
