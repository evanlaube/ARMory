
#ifndef PWM_H
#define PWM_H

#include "armory/gpio.h"


// PWM Channel map top map each pin to a pwm channel
typedef struct {
    Pin pin;
    TIM_TypeDef *timer;
    volatile uint32_t *ccr;
    TimerChannel channel;
    AlternateFunction af;
} PwmChannelMap;

// A0-A3, A6-A10, A15, B0-B9
// Format: Pin, *timer, *ccr, channel, alternateFunction 
extern const PwmChannelMap pwmPinMap[];

void pwmInitAll(void);
void testomatic(void);
void pwmInitPin(Pin pin);
void pwmInitTimer(TIM_TypeDef *timer, TimerChannel channel);
void pwmWrite(Pin pin, uint8_t dutyCycle);
const PwmChannelMap *getPwmMap(Pin pin);

#endif // !PWM_H
