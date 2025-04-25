
#include "armory/adc.h"
#include "armory/gpio.h"
#include "armory/rcc.h"

void adcInit(void) {
    // Enable ADC1 clock
    RCC->APB2ENR |= (1 << 8);

    // Set ADC prescaler: APB2 / 4 = 21 MHz
    ADC_CCR &= ~(0b11 << 16);           // Clear prescaler bits
    ADC_CCR |=  (0b01 << 16);           // DIV4 (0b01)

    // Set sample time to maximum (480 cycles) for all channels
    ADC1->SMPR2 = 0xFFFFFFFF;
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
