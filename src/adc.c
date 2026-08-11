
#include "armory/adc.h"
#include "armory/gpio.h"
#include "armory/rcc.h"
#include "armory/timing.h"

void adcInit(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= (1 << 8);

    // Set ADC prescaler: APB2 / 4 = 21 MHz
    ADC_CCR &= ~(0b11 << 16);           // Clear prescaler bits
    ADC_CCR |=  (0b01 << 16);           // DIV4 (0b01)

    // Set sample time to 84 cycles for channels 0-15 (each 3-bit field = 0b100).
    // TODO: This should be configurable at some point
    ADC1->SMPR2 = 0x24924924; // channels 0-9
    ADC1->SMPR1 = 0x00024924; // channels 10-15

    // Power on the ADC once and let it stabilize (tSTAB). Leaving it on
    // avoids re-paying the stabilization time on every conversion.
    ADC1->CR2 |= ADC_CR2_ADON;
    delay_us(10);
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

    // ADC is powered on once in adcInit; just select, trigger, and read.
    // SWSTART self-clears when the conversion starts, and reading DR
    // clears EOC, so no manual cleanup is needed here.
    ADC1->SQR3 = channel;
    ADC1->CR2 |= ADC_CR2_SWSTART;
    while (!(ADC1->SR & ADC_SR_EOC));
    return ADC1->DR & 0x0FFF;
}
