
#ifndef GPIO_H
#define GPIO_H

#include <stddef.h>
#include <stdint.h>

#include <armory/tim.h>

// Define General Purpose Input/Output (GPIO) base addresses
#define GPIOA_BASE 0x40020000
#define GPIOB_BASE 0x40020400
#define GPIOC_BASE 0x40020800
#define GPIOD_BASE 0x40020C00
#define GPIOH_BASE 0x40021C00

// Define GPIO register offsets
#define GPIOx_MODER_OFFSET    0x00
#define GPIOx_OTYPER_OFFSET   0x04
#define GPIOx_OSPEEDR_OFFSET  0x08
#define GPIOx_PUPDR_OFFSET    0x0C
#define GPIOx_IDR_OFFSET      0x10
#define GPIOx_ODR_OFFSET      0x14
#define GPIOx_BSRR_OFFSET     0x18
#define GPIOx_LCKR_OFFSET     0x1C
#define GPIOx_AFRL_OFFSET     0x20
#define GPIOx_AFRH_OFFSET     0x24

// Typedef for easy access to GPIO registers
typedef struct {
    volatile uint32_t MODER;    // 0x00
    volatile uint32_t OTYPER;   // 0x04
    volatile uint32_t OSPEEDR;  // 0x08
    volatile uint32_t PUPDR;    // 0x0C
    volatile uint32_t IDR;      // 0x10
    volatile uint32_t ODR;      // 0x14
    volatile uint32_t BSRR;     // 0x18
    volatile uint32_t LCKR;     // 0x1C
    volatile uint32_t AFR[2];     // 0x20 & 0x24
} GPIO_TypeDef;

// Define GPIO registers for GPIOA, GPIOB, and GPIOC
#define GPIOA ((GPIO_TypeDef*)(GPIOA_BASE))
#define GPIOB ((GPIO_TypeDef*)(GPIOB_BASE))
#define GPIOC ((GPIO_TypeDef*)(GPIOC_BASE))
#define GPIOD ((GPIO_TypeDef*)(GPIOD_BASE))
#define GPIOH ((GPIO_TypeDef*)(GPIOH_BASE))

#define PORT_A 0x00
#define PORT_B 0x01
#define PORT_C 0x02
#define PORT_D 0x03

// Pin typedef - stores port (A, B, C) and pin number
typedef struct {
    GPIO_TypeDef *port;
    uint8_t pin;
} Pin;

// Pins in Port A
#define A0  ((Pin) {GPIOA, 0})
#define A1  ((Pin) {GPIOA, 1})
#define A2  ((Pin) {GPIOA, 2})
#define A3  ((Pin) {GPIOA, 3})
#define A4  ((Pin) {GPIOA, 4})
#define A5  ((Pin) {GPIOA, 5})
#define A6  ((Pin) {GPIOA, 6})
#define A7  ((Pin) {GPIOA, 7})
#define A8  ((Pin) {GPIOA, 8})
#define A9  ((Pin) {GPIOA, 9})
#define A10 ((Pin) {GPIOA, 10})
#define A11 ((Pin) {GPIOA, 11})
#define A12 ((Pin) {GPIOA, 12})
#define A13 ((Pin) {GPIOA, 13})
#define A14 ((Pin) {GPIOA, 14})
#define A15 ((Pin) {GPIOA, 15})

// Pins in port B
#define B0  ((Pin) {GPIOB, 0})
#define B1  ((Pin) {GPIOB, 1})
#define B2  ((Pin) {GPIOB, 2})
#define B3  ((Pin) {GPIOB, 3})
#define B4  ((Pin) {GPIOB, 4})
#define B5  ((Pin) {GPIOB, 5})
#define B6  ((Pin) {GPIOB, 6})
#define B7  ((Pin) {GPIOB, 7})
#define B8  ((Pin) {GPIOB, 8})
#define B9  ((Pin) {GPIOB, 9})
#define B10 ((Pin) {GPIOB, 10})
#define B11 ((Pin) {GPIOB, 11})
#define B12 ((Pin) {GPIOB, 12})
#define B13 ((Pin) {GPIOB, 13})
#define B14 ((Pin) {GPIOB, 14})
#define B15 ((Pin) {GPIOB, 15})

// Pins in port C
#define C13 ((Pin) {GPIOC, 13})
#define C14 ((Pin) {GPIOC, 14})
#define C15 ((Pin) {GPIOC, 15})

// Pin mask to get mask to set/reset pin in GPIOx_BSRR register
#define PIN_MASK(pin)        (1U << (pin))
#define PIN_RESET_MASK(pin)  (1U << ((pin) + 16))

// Port table to access GPIO ports during runtime
static GPIO_TypeDef * const gpio_port_table[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, NULL, NULL, NULL, GPIOH
};

// Typedef to easily write alternate functions
typedef enum {
    AF0 = 0,
    AF1 = 1,
    AF2 = 2,
    AF3 = 3,
    AF4 = 4,
    AF5 = 5,
    AF6 = 6,
    AF7 = 7,
    AF8 = 8,
    AF9 = 9,
    AF10 = 10,
    AF11 = 11,
    AF12 = 12,
    AF13 = 13,
    AF14 = 14,
    AF15 = 15
} AlternateFunction;


// Typedef for GPIO States
typedef enum {
    LOW =   0,
    HIGH =  1
} PinState;

// Typedef for GPIO Modes
typedef enum {
    INPUT =          0x00,
    OUTPUT =         0x01,
    ALTERNATE_FUNC = 0x02,
    ANALOG =         0x03
} PinMode;

// Typedef for GPIO Pull-up/Pull-down
typedef enum {
    NO_PULL =   0x00,
    PULL_UP =   0x01,
    PULL_DOWN = 0x02,
    REVERSED =  0x03
} PinPullMode;

// Typedef for pin output types
typedef enum {
    PUSH_PULL  = 0x00,
    OPEN_DRAIN = 0x01
} OutputType;


void gpioInit(GPIO_TypeDef *gpio);
void gpioInitAll(void);
void gpioPinMode(Pin pin, PinMode mode);
void gpioSetAlternateFunction(Pin pin, AlternateFunction af);
void gpioSetOutputType(Pin pin, OutputType otype);
void gpioWrite(Pin pin, PinState value);
void gpioSetPull(Pin pin, PinPullMode pull);
PinState gpioDigitalRead(Pin pin);




#endif // !GPIO_H
