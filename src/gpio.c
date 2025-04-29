
#include "armory/gpio.h"
#include "armory/rcc.h"


void gpioInit(GPIO_TypeDef *gpio) {
    // Check if the GPIO port is valid
    if(gpio == NULL) {
        return; // Invalid GPIO port
    }

    // Enable the GPIO clock for the port.
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
    // Clear bits in GPIOx_MODER at pin index
    pin.port->MODER &= ~(0b11 << (pin.pin * 2)); 
    // Set bits to desired pin mode
    pin.port->MODER |= (mode << (pin.pin * 2)); 
}

void gpioSetAlternateFunction(Pin pin, AlternateFunction af) {
    // Clear bits in GPIOx_AFRL and GPIOx_AFRH at pin index
    pin.port->AFR[pin.pin / 8] &= ~(0xF << ((pin.pin % 8) * 4)); 
    // Set bits to desired alternate function
    pin.port->AFR[pin.pin / 8] |=  (af  << ((pin.pin % 8) * 4)); 
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
    // Set bits to desired pull mode 
    pin.port->PUPDR |= (pullMode << (pin.pin*2));
}

void gpioSetOutputType(Pin pin, OutputType otype) {
    // Clear bits in GPIOx_OTYPER at pin index
    pin.port->OTYPER &= ~(1 << pin.pin);
    // Set bits to desired output type
    pin.port->OTYPER |= (otype << pin.pin);
}

PinState gpioDigitalRead(Pin pin) {
    // Check if pin is driven high in the input data register
    if((pin.port->IDR) & (1<<pin.pin)) {
        return HIGH;
    }

    return LOW;
}


