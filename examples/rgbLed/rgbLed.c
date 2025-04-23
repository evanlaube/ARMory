#include "../../src/gpio.h"
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

    gpioPinMode(BUTTON_PIN, INPUT);
    gpioSetPull(BUTTON_PIN, PULL_UP);

    gpioPinMode(POT_PIN, ANALOG);
    gpioSetPull(POT_PIN, NO_PULL);

    pwmInitPin(BLUE_LED);
    pwmInitPin(RED_LED);
    pwmInitPin(GREEN_LED);

    uint8_t redBrightness   = 200;
    uint8_t greenBrightness = 51;
    uint8_t blueBrightness  = 255;

    // Keep track of mode of color picker:
    // 0 -> Display color
    // 1 -> Set Red
    // 2 -> Set Green
    // 3 -> Set Blue
    uint8_t mode = 1;
    
    // Software debounce button (kind of)
    uint16_t buttonWait = 1000;
    uint16_t potValue = 4095;
    uint8_t potValueScaled;
    while(1) {
        uint16_t potValue = adcReadPin(POT_PIN); // 0–4095
        if(potValue < 24) {
            potValue = 0;
        }
        uint8_t scaledR = (redBrightness   * potValue) / 4095;
        uint8_t scaledG = (greenBrightness * potValue) / 4095;
        uint8_t scaledB = (blueBrightness  * potValue) / 4095;

        if (mode == 0) {
            pwmWrite(RED_LED,   scaledR);
            pwmWrite(GREEN_LED, scaledG);
            pwmWrite(BLUE_LED,  scaledB);
        } else if (mode == 1) {
            redBrightness = (potValue * 255) / 4095;
            pwmWrite(RED_LED,   redBrightness);
            pwmWrite(GREEN_LED, 0);
            pwmWrite(BLUE_LED,  0);
        } else if (mode == 2) {
            greenBrightness = (potValue * 255) / 4095;
            pwmWrite(RED_LED,   0);
            pwmWrite(GREEN_LED, greenBrightness);
            pwmWrite(BLUE_LED,  0);
        } else if (mode == 3) {
            blueBrightness = (potValue * 255) / 4095;
            pwmWrite(RED_LED,   0);
            pwmWrite(GREEN_LED, 0);
            pwmWrite(BLUE_LED,  blueBrightness);
        }

        // Mode switch with software debounce
        if (!gpioDigitalRead(BUTTON_PIN) && buttonWait > 2000) {
            mode = (mode + 1) % 4;
            buttonWait = 0;
        }

        if (buttonWait <= 2000) {
            buttonWait++;
        }
    }
}

