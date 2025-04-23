
#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t*)0xE0001004)
#define DEMCR       (*(volatile uint32_t*)0xE000EDFC)

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);



#endif // !TIMING_H
