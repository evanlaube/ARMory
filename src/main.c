#include "gpio.h"
#define RED_LED B0
#define GREEN_LED B1
#define BLUE_LED A8

#define BUTTON_PIN A0
#define POT_PIN A1

#define GPIOC_MODER  (*(volatile unsigned int*)0x40020800)
#define GPIOC_ODR    (*(volatile unsigned int*)0x40020814)

int main(void) {
    gpioInitAll(); 
    adcInit();

    gpioPinMode(RED_LED, OUTPUT);
    gpioPinMode(GREEN_LED, OUTPUT);
    gpioPinMode(BLUE_LED, OUTPUT);


    while(1) {
        uint16_t potValue = adcReadPin(POT_PIN);
        uint16_t potValueScaled = (potValue * 100) / 4095; // Scale to 0-100
        if (potValueScaled < 33) {
            gpioWrite(RED_LED, HIGH);
            gpioWrite(GREEN_LED, LOW);
            gpioWrite(BLUE_LED, LOW);
        } else if (potValueScaled < 66) {
            gpioWrite(RED_LED, LOW);
            gpioWrite(GREEN_LED, HIGH);
            gpioWrite(BLUE_LED, LOW);
        } else {
            gpioWrite(RED_LED, LOW);
            gpioWrite(GREEN_LED, LOW);
            gpioWrite(BLUE_LED, HIGH);
        }

        if(!gpioDigitalRead(BUTTON_PIN)) {
            gpioWrite(RED_LED, HIGH);
            gpioWrite(GREEN_LED, HIGH);
            gpioWrite(BLUE_LED, HIGH);
        }
    }
}

