
#include <armory/gpio.h>
#include <armory/i2c.h>
#include <armory/timing.h>
#include <stdint.h>

#include "io.h"
#include "sh1106.h"


#define PADDLE_WIDTH  18
#define PADDLE_HEIGHT  3

#define BLOCK_ROWS    4
#define BLOCK_COLS    10
#define BLOCK_SPACING 1
#define BLOCK_WIDTH   11
#define BLOCK_HEIGHT  2

static uint8_t blocks[BLOCK_ROWS][BLOCK_COLS];

int main() {
    ioInit();
    i2cInit(I2C1);
    oledInit(I2C1);

    uint8_t frameCount = 0;


    int8_t x = 0;
    int8_t y = 0;
    uint8_t w = 30;
    uint8_t h = 20;
    int8_t vx = 1;
    int8_t vy = 1;

    while(1) {
        readJoystick();
        gpioWrite(GREEN_LED, joystickPressed);

        uint8_t redBrightness  = absVal(deltaX);
        uint8_t blueBrightness = absVal(deltaY);

        pwmWrite(RED_LED, redBrightness);
        pwmWrite(BLUE_LED, blueBrightness);

        oledDrawRectangle(x, y, w, h, 0);

        x += vx;
        y += vy;

        if(x + w > SH1106_WIDTH-1) {
            x--;
            vx *= -1;
        } else if(x < 0) {
            x++;
            vx *=-1;
        }

        if(y + h > SH1106_HEIGHT-1) {
            y--;
            vy *= -1;
        } else if(y < 0) {
            y++;
            vy *= -1;
        }
        oledDrawRectangle(x,y,w,h,1);

        oledUpdate();
        delay_ms(20);
    }

}
