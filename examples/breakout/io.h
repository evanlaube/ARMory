
#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <armory/gpio.h>

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

extern volatile int16_t deltaX;
extern volatile int16_t deltaY;
extern volatile PinState joystickPressed;

void ioInit(void);
void readJoystick(void);
int16_t absVal(int16_t a); 

#endif // !IO_H
