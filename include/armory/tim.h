
#ifndef TIM_H
#define TIM_H

#include <stdint.h>

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

// Typedef for timer channels
typedef enum {
    CH1 = 1,
    CH2 = 2,
    CH3 = 3,
    CH4 = 4,
    CH5 = 5
} TimerChannel;

#endif // !TIM_H
