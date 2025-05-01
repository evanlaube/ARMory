#include <armory/gpio.h>
#include <armory/pwm.h>
#include <armory/adc.h>
#include <armory/i2c.h>
#include <armory/timing.h>
#include <stdint.h>

#include "io.h"
#include "sh1106.h"


#define PADDLE_WIDTH  18
#define PADDLE_HEIGHT  3

#define BALL_SIZE 1

#define BLOCK_ROWS    4
#define BLOCK_COLS    11
#define BLOCK_SPACING 1
#define BLOCK_WIDTH   11
#define BLOCK_HEIGHT  4

static uint8_t blocks[BLOCK_COLS][BLOCK_ROWS];

void drawBlocks() {
    for(int bx = 0; bx < BLOCK_COLS; bx++) {
        for(int by = 0; by <  BLOCK_ROWS; by++) {
            // Don't draw anything if the block has been broken
            if(!blocks[bx][by]) {
                continue;
            }
            int x = (BLOCK_SPACING + BLOCK_WIDTH) * bx;
            int y = (BLOCK_SPACING + BLOCK_HEIGHT) * by;

            oledDrawRectangle(x, y, BLOCK_WIDTH, BLOCK_HEIGHT, 1);
        }
    }
}

int main() {
    // Give OLED time to turn on before sending init sequence
    delay_ms(30);

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

    uint8_t paddleX = 128/2 - PADDLE_WIDTH/2;
    uint8_t paddleY = 64-4-PADDLE_HEIGHT;
    uint8_t paddlePX = 128/2 - PADDLE_WIDTH/2;
    uint8_t paddlePY = 64-4-PADDLE_HEIGHT;
    oledDrawRectangle(paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 1);

    for(int i = 0; i < BLOCK_COLS; i++) {
        for(int j = 0; j < BLOCK_ROWS; j++) {
            blocks[i][j] = 1;
        }
    }

    // Blocks only need to be drawn once, then erased gradually
    drawBlocks();

    int8_t ballX = SH1106_WIDTH / 4 + BLOCK_WIDTH / 2;
    int8_t ballY = 32;
    int8_t ballPX = SH1106_WIDTH / 4 + BLOCK_WIDTH / 2;
    int8_t ballPY = 32;

    int8_t velX = 1;
    int8_t velY = -1;

    while(1) {
        readJoystick();

        paddlePX = paddleX;
        paddlePY = paddleY;
        if(deltaX > 0 && paddleX < SH1106_WIDTH-PADDLE_WIDTH) {
            if(paddleX > SH1106_WIDTH-PADDLE_WIDTH-2) { 
                paddleX = SH1106_WIDTH - PADDLE_WIDTH;
            } else {
                paddleX += 2;
            }
        } else if(deltaX < 0 && paddleX > 0) {
            if(paddleX < 2){
                paddleX = 0;
            } else {
                paddleX -= 2;
            }
        }

        ballPX = ballX;
        ballPY = ballY;

        ballX += velX;
        ballY += velY;

        // Check ball against paddle
        if(ballX >= paddleX && ballX <= paddleX + PADDLE_WIDTH - BALL_SIZE
                && ballY >= paddleY && ballY <= paddleY + PADDLE_HEIGHT - BALL_SIZE) {

            if(ballPX > paddleX && ballPX <= paddleX + PADDLE_WIDTH - BALL_SIZE){ 
                uint8_t distFromCenter = absVal((paddleX+PADDLE_WIDTH/2) - ballX);

                // normalize velX 
                velX /= absVal(velX);

                if(distFromCenter < PADDLE_WIDTH / 6 + 1) {
                    velX *= 1;
                    velY = -2;
                } else if(distFromCenter < (PADDLE_WIDTH * 2) / 6) {
                    velX *= 1;
                    velY = -1;
                } else {
                    velX *= 2;
                    velY = -1;
                }

                // Step forward once to prevent being inside paddle;
                ballX += velX;
                ballY += velY;
            } else {
                // Otherwise, the side of the paddle was hit
                velX *= -1;
            }

        }


        // TODO: Check ball against blocks
        for(uint8_t bx = 0; bx < BLOCK_COLS; bx++) {
            for(uint8_t by = 0; by < BLOCK_ROWS; by++) {
                // Ignore empty blocks
                if(!blocks[bx][by]) {
                    continue;
                }

                uint8_t blockX = (BLOCK_WIDTH + BLOCK_SPACING) * bx;
                uint8_t blockY = (BLOCK_HEIGHT + BLOCK_SPACING) * by;

                // Check if ball overlaps block
                if(ballX >= blockX && ballX <= blockX + BLOCK_WIDTH-BALL_SIZE 
                        && ballY >= blockY && ballY <= blockY + BLOCK_HEIGHT - BALL_SIZE ) {
                    blocks[bx][by] = 0;
                    // Clear block on display
                    oledDrawRectangle(blockX, blockY, BLOCK_WIDTH, BLOCK_HEIGHT, 0);
                    // Clear block in array
                    blocks[bx][by] = 0;

                    if(ballPX >= blockX && ballPX <= blockX + BLOCK_WIDTH - BALL_SIZE) {
                        velY *= -1;
                        ballY += velY;
                    } else if(ballPY >= blockY && ballPY <= blockY + BLOCK_HEIGHT - BALL_SIZE) {
                        velX *= -1;
                        ballX += velX;
                    }

                    // Ball can only hit one block per frame
                    break;
                }

            }
        }

        // Check ball against boundaries
        if(ballX >= SH1106_WIDTH-BALL_SIZE) {
            velX *= -1;
        } else if(ballX <= 0) {
            velX *= -1;
        }

        if(ballY <= 0) {
            velY *= -1;
        } else if(ballY >= SH1106_HEIGHT-BALL_SIZE) {
            for(int i = 0; i < 3; i++) {
                int n = 0;
                while(n < 255) {
                    pwmWrite(RED_LED, n++);
                    delay_ms(1);
                }

                while(n > 0) {
                    pwmWrite(RED_LED, --n);
                    delay_ms(1);
                }

            }
            // Wait for joystick press and recall main
            while(gpioDigitalRead(JOY_SW));
            for(int i = 0; i < 3; i++) {
                gpioWrite(GREEN_LED, HIGH);
                delay_ms(100);
                gpioWrite(GREEN_LED, LOW);
                delay_ms(100);
            }
            main();
        }

        // Draw paddle
        if(paddleX != paddlePX) {
            // Erase previous paddle
            oledDrawRectangle(paddlePX, paddlePY, PADDLE_WIDTH, PADDLE_HEIGHT, 0);
            // Draw paddle at new position
            oledDrawRectangle(paddleX, paddleY, PADDLE_WIDTH, PADDLE_HEIGHT, 1);
        }
        
        // Draw Ball
        oledDrawRectangle(ballPX, ballPY, BALL_SIZE, BALL_SIZE, 0);
        oledDrawRectangle(ballX, ballY, BALL_SIZE, BALL_SIZE, 1);



        oledUpdate();
        delay_ms(30);
    }

}
