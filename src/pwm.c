
#include "armory/pwm.h"
#include "armory/gpio.h"
#include "armory/tim.h"
#include "armory/rcc.h"

// NOTE: Most pins can be mapped to multiple PWM channels. These were chosen 
// arbitrarily and are not set in stone, however, they were chosen to utilize
// only timers 1-4 when all PWM pins are initialized.
const PwmChannelMap pwmPinMap[] = {
    { A0,  TIM2, &TIM2->CCR1, CH1, AF1 },
    { A1,  TIM2, &TIM2->CCR2, CH2, AF1 },
    { A2,  TIM2, &TIM2->CCR3, CH3, AF1 },
    { A3,  TIM2, &TIM2->CCR4, CH4, AF1 },
    { A6,  TIM3, &TIM3->CCR1, CH1, AF2 },
    { A7,  TIM3, &TIM3->CCR2, CH2, AF2 },
    { A8,  TIM1, &TIM1->CCR1, CH1, AF1 },
    { A9,  TIM1, &TIM1->CCR2, CH2, AF1 },
    { A10, TIM1, &TIM1->CCR3, CH3, AF1 },
    { A15, TIM2, &TIM2->CCR1, CH1, AF1 },
    { B0,  TIM3, &TIM3->CCR3, CH3, AF2 },
    { B1,  TIM3, &TIM3->CCR4, CH4, AF2 },
    { B4,  TIM3, &TIM3->CCR1, CH1, AF2 },
    { B5,  TIM3, &TIM3->CCR2, CH2, AF2 },
    { B6,  TIM4, &TIM4->CCR1, CH1, AF2 },
    { B7,  TIM4, &TIM4->CCR2, CH2, AF2 },
    { B8,  TIM4, &TIM4->CCR3, CH3, AF2 },
    { B9,  TIM4, &TIM4->CCR4, CH4, AF2 },
};

const PwmChannelMap *getPwmMap(Pin pin) {
    for (int i = 0; i < sizeof(pwmPinMap) / sizeof(PwmChannelMap); i++) {
        if (pwmPinMap[i].pin.port == pin.port && pwmPinMap[i].pin.pin == pin.pin) {
            return &pwmPinMap[i];
        }
    }
    return NULL;
}

void pwmInitTimer(TIM_TypeDef *timer, TimerChannel channel) {
    // Enable to timer clock
    if(timer == TIM1) {
        RCC->APB2ENR |= (1 << 0);
    } else if(timer == TIM2) {
        RCC->APB1ENR |= (1 << 0);
    } else if(timer == TIM3) {
        RCC->APB1ENR |= (1 << 1);
    } else if(timer == TIM4) {
        RCC->APB1ENR |= (1 << 2);
    } else if(timer == TIM5) {
        // Current configuration does not use TIM5, but include for future proofing
        RCC->APB1ENR |= (1<<3);
    } else {
        // Invalid PWM pin
        return;
    }

    // Setup timer registers
    timer->PSC = 327; // Roughly 1khz PWM frequency
    timer->ARR = 255; // Allow PWM values to be written to 8 bits
    timer->CR1 |= (1 << 7); // ARPE
    timer->CR1 |= (1 << 0); // CEN
    //timer->CCMR1 |= (0x06 << 4); // Set OC1 mode to PWM
    //timer->CCMR1 |= (1 << 3); // Enable OC1 Preload
    //timer->CCER |= (1 << 0); //Enable capture/compare CH1 output
    //timer->DIER |= (1 << 0); //Enable update interrupt    

    // Set channel mode (PWM Mode 1) + preload + enable output
    switch (channel) {
        case CH1:
            timer->CCMR1 &= ~(0b111 << 4);
            timer->CCMR1 |=  (0b110 << 4);
            timer->CCMR1 |=  (1 << 3); // OC1PE
            timer->CCER   |=  (1 << 0); // CC1E
            break;
        case CH2:
            timer->CCMR1 &= ~(0b111 << 12);
            timer->CCMR1 |=  (0b110 << 12);
            timer->CCMR1 |=  (1 << 11); // OC2PE
            timer->CCER   |=  (1 << 4); // CC2E
            break;
        case CH3:
            timer->CCMR2 &= ~(0b111 << 4);
            timer->CCMR2 |=  (0b110 << 4);
            timer->CCMR2 |=  (1 << 3); // OC3PE
            timer->CCER   |=  (1 << 8); // CC3E
            break;
        case CH4:
            timer->CCMR2 &= ~(0b111 << 12);
            timer->CCMR2 |=  (0b110 << 12);
            timer->CCMR2 |=  (1 << 11); // OC4PE
            timer->CCER   |=  (1 << 12); // CC4E
            break;
        case CH5:
            // Don't implement CH5 for now.
            break;
    }

    if(timer == TIM1) {
        timer->BDTR |= (1 << 15);
    }
}

void pwmInitPin(Pin pin) {
    // First get the pwmMap for the given pin
    const PwmChannelMap *map = getPwmMap(pin);

    // Make sure that the GPIO reg that the pin is on is enabled
    gpioInit(pin.port);
    // Set the gpio mode for the pin to alternate function
    gpioPinMode(pin, ALTERNATE_FUNC); 
    
    // Set which alternate function to use for the pin
    gpioSetAlternateFunction(pin, map->af);
                                                                      //
    // Initialize timer attached to pin
    pwmInitTimer(map->timer, map->channel);
}

void pwmWrite(Pin pin, uint8_t dutyCycle) {
    const PwmChannelMap *map = getPwmMap(pin);
    // Return if the pin is not PWM available
    if(!map) return;
    *(map->ccr) = dutyCycle;
}
