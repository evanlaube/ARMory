
#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

// FLASH_ACR
#define FLASH_ACR_LATENCY_2WS   (2 << 0)
#define FLASH_ACR_PRFTEN        (1 << 8)
#define FLASH_ACR_ICEN          (1 << 9)
#define FLASH_ACR_DCEN          (1 << 10)

// FLASH base
#define FLASH_BASE      (0x40023C00UL)

// Register layout for FLASH
typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
} FLASH_TypeDef;

#define FLASH ((FLASH_TypeDef *) FLASH_BASE)

#endif // FLASH_H
