
#ifndef RCC_H
#define RCC_H

#include <stdint.h>

// Base address for Reset and Clock Control (RCC)
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

// RCC_CR
#define RCC_CR_HSEON            (1 << 16)
#define RCC_CR_HSERDY           (1 << 17)
#define RCC_CR_PLLON            (1 << 24)
#define RCC_CR_PLLRDY           (1 << 25)

// RCC_PLLCFGR positions
#define RCC_PLLCFGR_PLLM_POS    0
#define RCC_PLLCFGR_PLLN_POS    6
#define RCC_PLLCFGR_PLLP_POS    16
#define RCC_PLLCFGR_PLLSRC_HSE  (1 << 22)

// RCC_CFGR
#define RCC_CFGR_SW_PLL         (0b10 << 0)
#define RCC_CFGR_SWS_PLL        (0b10 << 2)
#define RCC_CFGR_SWS_MSK        (0b11 << 2)

#define RCC_CFGR_HPRE_DIV1      (0b0000 << 4)
#define RCC_CFGR_PPRE1_DIV2     (0b100 << 10)
#define RCC_CFGR_PPRE2_DIV1     (0b000 << 13)

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

void rccInit(void);

#endif // !RCC_H
