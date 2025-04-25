
#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#include "armory/gpio.h"

// ADC CR2 Register bit definitions
#define ADC_CR2_ADON         (1U << 0)  // ADC ON/OFF
#define ADC_CR2_CONT        (1U << 1)  // Continuous conversion mode
#define ADC_CR2_DMA         (1U << 8)  // DMA mode
#define ADC_CR2_EOCS        (1U << 9)  // End of conversion interrupt
#define ADC_CR2_EXTEN       (1U << 10) // External trigger enable
#define ADC_CR2_EXTSEL      (1U << 11) // External trigger selection
#define ADC_CR2_JSWSTART    (1U << 22) // Start conversion for injected channels
#define ADC_CR2_SWSTART     (1U << 30) // Start conversion for regular channels

// ADC SR Register bit definitions
#define ADC_SR_AWD         (1U << 0)  // Analog watchdog flag
#define ADC_SR_EOC         (1U << 1)  // End of conversion
#define ADC_SR_JEOC        (1U << 2)  // End of injected conversion
#define ADC_SR_JSTRT       (1U << 3)  // Injected channel start flag
#define ADC_SR_STRT        (1U << 4)  // Regular channel start flag
#define ADC_SR_OVR         (1U << 5)  // Overrun flag

// ADC Common abse address
#define ADC_COMMON_BASE 0x40012300
#define ADC_CCR (*(volatile uint32_t *)(ADC_COMMON_BASE + 0x04))

// ADC1 base address 
#define ADC1_BASE 0x40012000

// Typedef for easy access to ADC registers
typedef struct {
    volatile uint32_t SR;         // 0x00
    volatile uint32_t CR1;        // 0x04
    volatile uint32_t CR2;        // 0x08
    volatile uint32_t SMPR1;      // 0x0C
    volatile uint32_t SMPR2;      // 0x10
    volatile uint32_t JOFR1;      // 0x14
    volatile uint32_t JOFR2;      // 0x18
    volatile uint32_t JOFR3;      // 0x1C
    volatile uint32_t JOFR4;      // 0x20
    volatile uint32_t HTR;        // 0x24
    volatile uint32_t LTR;        // 0x28
    volatile uint32_t SQR1;       // 0x2C
    volatile uint32_t SQR2;       // 0x30
    volatile uint32_t SQR3;       // 0x34
    volatile uint32_t JSQR;       // 0x38
    volatile uint32_t JDR1;       // 0x3C
    volatile uint32_t JDR2;       // 0x40
    volatile uint32_t JDR3;       // 0x44
    volatile uint32_t JDR4;       // 0x48
    volatile uint32_t DR;         // 0x4C
} ADC_TypeDef;

typedef enum {
    CHANNEL_0  =  0, // PA0
    CHANNEL_1  =  1, // PA1
    CHANNEL_2  =  2, // PA2
    CHANNEL_3  =  3, // PA3
    CHANNEL_4  =  4, // PA4
    CHANNEL_5  =  5, // PA5
    CHANNEL_6  =  6, // PA6
    CHANNEL_7  =  7, // PA7
    CHANNEL_8  =  8, // PB0
    CHANNEL_9  =  9, // PB1
    CHANNEL_10 = 10, // PC0
    CHANNEL_11 = 11, // PC1
    CHANNEL_12 = 12, // PC2
    CHANNEL_13 = 13, // PC3
    CHANNEL_14 = 14, // PC4
    CHANNEL_15 = 15, // PC5
    ADC_INVALID = 0xFF
} AdcChannel;

// Define ADC1 at base offset of ADC1
#define ADC1 ((ADC_TypeDef *)(ADC1_BASE))

/**
 * @brief Initializes ADC1 on the microcontroller.
 *
 * Enables ADC1 clock, sets the prescaler, and configures the sample time
 * for ADC1.
 *
 * @note This must be called before calling any other ADC functions.
 */
void adcInit(void);

/**
 * @brief Gets the AdcChannel to use for a given pin.
 *
 * @param pin The GPIO pin to map to an ADC channel. 
 * @return The corresponding AdcChannel, or ADC_INVALID if unsupported
 */
AdcChannel gpioToAdcChannel(Pin pin);

/**
 * @brief Reads an analog value from a given AdcChannel.
 *
 * @return The value read from the channel (0 - 4095)
 */
uint16_t adcReadChannel(AdcChannel channel);

/**
 * @brief Reads an analog value from a supported analog pin.
 *
 * Converts the analog value on the given pin to a 12-bit digital value
 * values 0 - 4095.
 *
 * @return The value read from the pin (0 - 4095)
 */
uint16_t adcReadPin(Pin pin);

#endif // !ADC_H
