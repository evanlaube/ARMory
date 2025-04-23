
#include "armory/timing.h"
#include <stdint.h>

void delay_ms(uint32_t ms) {
    // Enable DWT
    DEMCR |= (1 << 24);       // Enable TRCENA
    DWT_CTRL |= 1;            // Enable CYCCNT
    uint32_t start = DWT_CYCCNT;

    uint32_t target = ms * 84000;  // 84 MHz = 84k cycles per ms

    while ((DWT_CYCCNT - start) < target);
}

void delay_us(uint32_t us) {
    // Enable DWT
    DEMCR |= (1 << 24);       // Enable TRCENA
    DWT_CTRL |= 1;            // Enable CYCCNT
    uint32_t start = DWT_CYCCNT;

    uint32_t target = us * 84;  // 84 MHz = 84 cycles per us

    while ((DWT_CYCCNT - start) < target);
}
