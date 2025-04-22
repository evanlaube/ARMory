
#ifndef GPIO_H
#define GPIO_H

#include <stddef.h>
#include <stdint.h>

// Basee address for Reset and Clock Control (RCC)
#define RCC_BASE 0x40023800

// Define RCC register offsets
#define RCC_RC_OFFSET          0x00
#define RCC_PLLCFGR_OFFSET     0x04
#define RCC_CFGR_OFFSET        0x08
#define RCC_CIR_OFFSET         0x0C
#define RCC_AHB1RSTR_OFFSET    0x10
#define RCC_AHB2RSTR_OFFSET    0x14
#define RCC_APB1RSTR_OFFSET    0x20
#define RCC_APB2RSTR_OFFSET    0x24
#define RCC_AHB1ENR_OFFSET     0x30
#define RCC_AHB2ENR_OFFSET     0x34
#define RCC_APB1ENR_OFFSET     0x40
#define RCC_APB2ENR_OFFSET     0x44
#define RCC_AHB1L_PENR_OFFSET  0x50
#define RCC_AHB2L_PENR_OFFSET  0x54
#define RCC_APB1L_PENR_OFFSET  0x60
#define RCC_APB2L_PENR_OFFSET  0x64
#define RCC_BDCR_OFFSET        0x70
#define RCC_CSR_OFFSET         0x74
#define RCC_SSCGR_OFFSET       0x80
#define RCC_PLLI2SCFGR_OFFSET  0x84
#define RCC_DCKCFGR_OFFSET     0x8c

// Bit definitions for enabling differnet i2cs
#define RCC_APB1ENR_I2C1EN     (1U << 21)
#define RCC_APB1ENR_I2C2EN     (1U << 22)
#define RCC_APB1ENR_I2C3EN     (1U << 23)

// Typdef for easy access to RCC registers
typedef struct {
    volatile uint32_t CR;             // 0x00
    volatile uint32_t PLLCFGR;        // 0x04  
    volatile uint32_t CFGR;           // 0x08
    volatile uint32_t CIR;            // 0x0C
    volatile uint32_t AHB1RSTR;       // 0x10
    volatile uint32_t AHB2RSTR;       // 0x14
    uint32_t RESERVED0[2];            // 0x18 & 0x1C
    volatile uint32_t APB1RSTR;       // 0x20 
    volatile uint32_t APB2RSTR;       // 0x24
    uint32_t RESERVED1[2];            // 0x28 & 0x2C
    volatile uint32_t AHB1ENR;        // 0x30
    volatile uint32_t AHB2ENR;        // 0x34
    uint32_t RESERVED2[2];            // 0x38 & 0x3C
    volatile uint32_t APB1ENR;        // 0x40
    volatile uint32_t APB2ENR;        // 0x44
    uint32_t RESERVED3[2];            // 0x48 & 0x4C
    volatile uint32_t AHB1LPENR;      // 0x50
    volatile uint32_t AHB2LPENR;      // 0x54 
    uint32_t RESERVED4[2];            // 0x58 & 0x5C
    volatile uint32_t APB1LPENR;      // 0x60
    volatile uint32_t APB2LPENR;      // 0x64
    uint32_t RESERVED5[2];            // 0x68 & 0x6C
    volatile uint32_t BDCR;           // 0x70
    volatile uint32_t CSR;            // 0x74
    uint32_t RESERVED6[2];            // 0x78 & 0x7C
    volatile uint32_t SSCGR;          // 0x80
    volatile uint32_t PLLI2SCFGR;     // 0x84
    uint32_t RESERVED8;                // 0x88
    volatile uint32_t DCKCFGR;        // 0x8C

} RCC_TypeDef;

// Define RCC at base offset of RCC
#define RCC ((RCC_TypeDef *) RCC_BASE)

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
#define A0 ((Pin)  {GPIOA, 0})
#define A1 ((Pin)  {GPIOA, 1})
#define A2 ((Pin)  {GPIOA, 2})
#define A3 ((Pin)  {GPIOA, 3})
#define A4 ((Pin)  {GPIOA, 4})
#define A5 ((Pin)  {GPIOA, 5})
#define A6 ((Pin)  {GPIOA, 6})
#define A7 ((Pin)  {GPIOA, 7})
#define A8 ((Pin)  {GPIOA, 8})
#define A9 ((Pin)  {GPIOA, 9})
#define A10 ((Pin) {GPIOA, 10})
#define A11 ((Pin) {GPIOA, 11})
#define A12 ((Pin) {GPIOA, 12})
#define A13 ((Pin) {GPIOA, 13})
#define A14 ((Pin) {GPIOA, 14})
#define A15 ((Pin) {GPIOA, 15})

// Pins in port B
#define B0 ((Pin)  {GPIOB, 0})
#define B1 ((Pin)  {GPIOB, 1})
#define B2 ((Pin)  {GPIOB, 2})
#define B3 ((Pin)  {GPIOB, 3})
#define B4 ((Pin)  {GPIOB, 4})
#define B5 ((Pin)  {GPIOB, 5})
#define B6 ((Pin)  {GPIOB, 6})
#define B7 ((Pin)  {GPIOB, 7})
#define B8 ((Pin)  {GPIOB, 8})
#define B9 ((Pin)  {GPIOB, 9})
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

// ADC1 base address 
#define ADC1_BASE 0x40012000

// ADC register offsets
#define ADC1_SR_OFFSET        0x00
#define ADC1_CR1_OFFSET       0x04
#define ADC1_CR2_OFFSET       0x08
#define ADC1_SMPR1_OFFSET     0x0C
#define ADC1_SMPR2_OFFSET     0x10
#define ADC1_JOFR1_OFFSET     0x14
#define ADC1_JOFR2_OFFSET     0x18
#define ADC1_JOFR3_OFFSET     0x1C
#define ADC1_JOFR4_OFFSET     0x20
#define ADC1_HTR_OFFSET       0x24
#define ADC1_LTR_OFFSET       0x28
#define ADC1_SQR1_OFFSET      0x2C
#define ADC1_SQR2_OFFSET      0x30
#define ADC1_SQR3_OFFSET      0x34
#define ADC1_JSQR_OFFSET      0x38
#define ADC1_JDR1_OFFSET      0x3C
#define ADC1_JDR2_OFFSET      0x40
#define ADC1_JDR3_OFFSET      0x44
#define ADC1_JDR4_OFFSET      0x48
#define ADC1_DR_OFFSET        0x4C

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

// Define ADC1 at base offset of ADC1
#define ADC1 ((ADC_TypeDef *)(ADC1_BASE))

// Port table to access GPIO ports during runtime
static GPIO_TypeDef * const gpio_port_table[] = {
    GPIOA, GPIOB, GPIOC, GPIOD, NULL, NULL, NULL, GPIOH
};

// Pin mask to get mask to set/reset pin in GPIOx_BSRR register
#define PIN_MASK(pin)        (1U << (pin))
#define PIN_RESET_MASK(pin)  (1U << ((pin) + 16))

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

// General Purpose Timer definitions (TIMx)
#define TIM1_BASE 0x40010000
#define TIM2_BASE 0x40000000
#define TIM3_BASE 0x40000400
#define TIM4_BASE 0x40000800
#define TIM5_BASE 0x40000C00

// Define TIM register offsets in typedef struct
typedef struct {
    volatile uint32_t CR1;        // 0x00
    volatile uint32_t CR2;        // 0x04
    volatile uint32_t SMCR;       // 0x08
    volatile uint32_t DIER;       // 0x0C
    volatile uint32_t SR;         // 0x10
    volatile uint32_t EGR;        // 0x14
    volatile uint32_t CCMR1;      // 0x18
    volatile uint32_t CCMR2;      // 0x1C
    volatile uint32_t CCER;       // 0x20
    volatile uint32_t CNT;        // 0x24
    volatile uint32_t PSC;        // 0x28
    volatile uint32_t ARR;        // 0x2C
    volatile uint32_t RCR;        // 0x30
    volatile uint32_t CCR1;       // 0x34
    volatile uint32_t CCR2;       // 0x38
    volatile uint32_t CCR3;       // 0x3C
    volatile uint32_t CCR4;       // 0x40
    volatile uint32_t BDTR;       // 0x44
    volatile uint32_t DCR;        // 0x48
    volatile uint32_t DMAR;       // 0x4C
} TIM_TypeDef;

#define TIM1 ((TIM_TypeDef *) TIM1_BASE)
#define TIM2 ((TIM_TypeDef *) TIM2_BASE)
#define TIM3 ((TIM_TypeDef *) TIM3_BASE)
#define TIM4 ((TIM_TypeDef *) TIM4_BASE)
#define TIM5 ((TIM_TypeDef *) TIM5_BASE)

// Define TIM  CR1 register bit offsets
#define TIM_CR1_CEN     (1 << 0)
#define TIM_CR1_ARPE    (1 << 7)

// DEFINE TIME CCMR1 register bit offsets
#define TIM_CCMR1_OC1M_Pos 4
#define TIM_CCMR1_OC1M     (0b111 << TIM_CCMR1_OC1M_Pos)
#define TIM_CCMR1_OC1PE    (1 << 3)

// Define TIM CCER register bit offsets
#define TIM_CCER_CC1E      (1 << 0)

// Define TIM EGR register bit offsets
#define TIM_EGR_UG         (1 << 0)

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

// Typedef for timer channels
typedef enum {
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4,
    CH5 = 5
} TimerChannel;

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
static const PwmChannelMap pwmPinMap[] = {
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

void gpioInit(GPIO_TypeDef *gpio);
void gpioInitAll(void);
void gpioPinMode(Pin pin, PinMode mode);
void gpioWrite(Pin pin, PinState value);
void gpioSetPull(Pin pin, PinPullMode pull);
PinState gpioDigitalRead(Pin pin);

void adcInit(void);
AdcChannel gpioToAdcChannel(Pin pin);
uint16_t adcReadChannel(AdcChannel channel);
uint16_t adcReadPin(Pin pin);

void pwmInitAll(void);
void pwmInitPin(Pin pin);
void pwmInitTimer(TIM_TypeDef *timer, TimerChannel channel);
//void pwmAttach(Pin pin, uint32_t frequency);
void pwmWrite(Pin pin, uint8_t dutyCycle);
const PwmChannelMap *getPwmMap(Pin pin);


#endif // !GPIO_H
