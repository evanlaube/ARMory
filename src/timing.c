
#include "armory/timing.h"
#include <stdint.h>

// Enable the DWT cycle counter exactly once. The delay functions call this on
// entry so callers don't need a separate init step; the static guard makes
// every call after the first a cheap early return.
static void dwtInit(void) {
    static uint8_t initialized = 0;
    if (initialized) return;

    DEMCR |= (1 << 24);       // Enable TRCENA
    DWT_CTRL |= 1;            // Enable CYCCNT
    initialized = 1;
}

void delay_ms(uint32_t ms) {
    dwtInit();
    uint32_t start = DWT_CYCCNT;

    uint32_t target = ms * 84000;  // 84 MHz = 84k cycles per ms

    while ((DWT_CYCCNT - start) < target);
}

void delay_us(uint32_t us) {
    dwtInit();
    uint32_t start = DWT_CYCCNT;

    uint32_t target = us * 84;  // 84 MHz = 84 cycles per us

    while ((DWT_CYCCNT - start) < target);
}
