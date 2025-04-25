
#include "armory/rcc.h"
#include "armory/flash.h"

void rccInit(void) {
    // Enable HSE
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY));

    // Configure the PLL: PLLCLK = (HSE / PLL_M) * PLL_N / PLL_P
    // (25 MHz / 25) * (336 / 4) = 84 MHz
    RCC->PLLCFGR = (25 << RCC_PLLCFGR_PLLM_POS) |    // M = 25
                   (336 << RCC_PLLCFGR_PLLN_POS) |   // N = 336
                   (1 << RCC_PLLCFGR_PLLP_POS)  |    // P = 4 (0b00)
                   (RCC_PLLCFGR_PLLSRC_HSE);         // HSE as PLL source


    // Set flash latency (2 wait states for 84 MHz)
    FLASH->ACR = FLASH_ACR_LATENCY_2WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    // Set AHB = SYSCLK, APB1 = SYSCLK/2, APB2 = SYSCLK
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;     // AHB prescaler = 1
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;    // APB1 = /2 (42 MHz)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;    // APB2 = /1 (84 MHz)
    
    // Enable the PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Switch system clock to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_MSK) != RCC_CFGR_SWS_PLL);
}
