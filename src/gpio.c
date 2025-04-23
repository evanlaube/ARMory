
#include "gpio.h"

void rccInit(void) {
    // Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    // Configure the PLL: PLLCLK = (HSE / PLL_M) * PLL_N / PLL_P
    // (25 MHz / 25) * (336 / 4) = 84 MHz
    RCC->PLLCFGR = (25 << RCC_PLLCFGR_PLLM_POS) |    // M = 25
                   (336 << RCC_PLLCFGR_PLLN_POS) |   // N = 336
                   (1 << RCC_PLLCFGR_PLLP_POS)  |    // P = 4 (0b00)
                   (RCC_PLLCFGR_PLLSRC_HSE);         // HSE as PLL source


    // Set flash latency (2 wait states for 84 MHz)
    FLASH->ACR = FLASH_ACR_LATENCY_2WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    // Set AHB = SYSCLK, APB1 = SYSCLK/2, APB2 = SYSCLK
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;     // AHB prescaler = 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;    // APB1 = /2 (42 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;    // APB2 = /1 (84 MHz)
    
    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Switch system clock to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_MSK) != RCC_CFGR_SWS_PLL);
}

void gpioInit(GPIO_TypeDef *gpio) {
    // Check if the GPIO port is valid
    if(gpio == NULL) {
        return; // Invalid GPIO port
    }

    if(gpio == GPIOA) {
        RCC->AHB1ENR |= (1<<0);
    } else if(gpio == GPIOB) {
        RCC->AHB1ENR |= (1<<1);
    } else if(gpio == GPIOC) {
        RCC->AHB1ENR |= (1<<2);
    } else if(gpio == GPIOD) {
        RCC->AHB1ENR |= (1<<3);
    } else if(gpio == GPIOH) {
        RCC->AHB1ENR |= (1<<7);
    }
}

void gpioInitAll(void) {
    // Initialize all GPIO ports
    gpioInit(GPIOA);
    gpioInit(GPIOB);
    gpioInit(GPIOC);
    gpioInit(GPIOD);
    gpioInit(GPIOH);
}

void gpioPinMode(Pin pin, PinMode mode) {
    pin.port->MODER &= ~(0b11 << (pin.pin * 2)); 
    pin.port->MODER |= (mode << (pin.pin * 2)); 
}

void gpioSetAlternateFunction(Pin pin, AlternateFunction af) {
    pin.port->AFR[pin.pin / 8] &= ~(0xF << ((pin.pin % 8) * 4)); // clear
    pin.port->AFR[pin.pin / 8] |=  (af  << ((pin.pin % 8) * 4)); // set
}

void gpioWrite(Pin pin, PinState value) {
    if (value == HIGH) {
        pin.port->BSRR = PIN_MASK(pin.pin); // Set the pin high
    } else {
        pin.port->BSRR = PIN_RESET_MASK(pin.pin); // Set the pin low
    }
}

void gpioSetPull(Pin pin, PinPullMode pullMode) {
    // Clear bits in GPIOx_PUPDR at pin index
    pin.port->PUPDR &= ~(0b11 << (pin.pin*2));
    // Set bits to desired pinmode
    pin.port->PUPDR |= (pullMode << (pin.pin*2));
}

void gpioSetOutputType(Pin pin, OutputType otype) {
    // Clear bit
    pin.port->OTYPER &= ~(1 << pin.pin);
    // Set bit
    pin.port->OTYPER |= (otype << pin.pin);
}

PinState gpioDigitalRead(Pin pin) {
    if((pin.port->IDR) & (1<<pin.pin)) {
        return HIGH;
    }
    return LOW;
}

void adcInit(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= (1 << 8);
}

AdcChannel gpioToAdcChannel(Pin pin) {
    if (pin.port == GPIOA && pin.pin <= 7) return pin.pin;
    if (pin.port == GPIOB && pin.pin <= 1) return pin.pin + 8;
    if (pin.port == GPIOC && pin.pin <= 5) return pin.pin + 10;
    return 0xFF; // Not a valid ADC pin
}

uint16_t adcReadPin(Pin pin) {
    return adcReadChannel(gpioToAdcChannel(pin));
}

uint16_t adcReadChannel(AdcChannel channel) {
    if(channel == ADC_INVALID || channel > 15) {
        return 0; // Invalid channel
    }

    ADC1->SQR3 = channel;
    ADC1->CR2 |= ADC_CR2_ADON;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    ADC1->CR2 &= ~(ADC_CR2_ADON);
    return ADC1->DR & 0x0FFF;

}

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
