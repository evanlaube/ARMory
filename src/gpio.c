
#include "gpio.h"

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

PinState gpioDigitalRead(Pin pin) {
    return (PinState) (pin.port->IDR) & (1 << pin.pin);
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
    for (int i = 0; i < sizeof(pwm_pin_map) / sizeof(PwmChannelMap); i++) {
        if (pwm_pin_map[i].pin.port == pin.port && pwm_pin_map[i].pin.pin == pin.pin) {
            return &pwm_pin_map[i];
        }
    }
    return NULL;
}

