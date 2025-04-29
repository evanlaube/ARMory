
#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)
#define DWT_CYCCNT  (*(volatile uint32_t*)0xE0001004)
#define DEMCR       (*(volatile uint32_t*)0xE000EDFC)

/**
 * @brief Delay the program for a specified number of milliseconds.
 *
 * @param ms Number of milliseconds to delay for.
 */
void delay_ms(uint32_t ms);

/**
 * @brief Delay the program for a specified number of microseconds.
 *
 * @param us Number of microseconds to delay for.
 */
void delay_us(uint32_t us);

#endif // !TIMING_H
