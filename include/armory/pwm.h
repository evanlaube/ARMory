
#ifndef PWM_H
#define PWM_H

#include "armory/gpio.h"


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
extern const PwmChannelMap pwmPinMap[];

/**
 * @brief Initializes PWM functionality for a given pin.
 *
 * Initializes the GPIO port of the given pin, and set's the pins mode to 
 * ALTERNATE_FUNC, and set's  the alternate function to the value for the PWM
 * timer in the pwmPinMap. Then, initializes the PWM timer for the pin.
 *
 * @param pin The pin to initialize PWM on.
 */
void pwmInitPin(Pin pin);

/**
 * @brief Initializes a timer for PWM functionality for a specific channel.
 *
 * Enables the timer clock, then sets the prescaler for a PWM frequency of
 * approximately 1kHz. Sets the PWM to 8-bit. Sets the channel mode to PWM 
 * mode 1, and enables the output for the channel.
 *
 * @param timer The timer to initialize for PWM.
 * @param channel The channel to initialize for PWM.
 */
void pwmInitTimer(TIM_TypeDef *timer, TimerChannel channel);

/**
 * @brief Writes a PWM duty cycle to a specific pin.
 *
 * Writes the specified duty cycle (8 bit) to the CCR register that corrosponds
 * with the given pin.
 *
 * @param pin The pin to write the duty cycle to.
 * @param dutyCycle The duty cycle to write (0-255).
 */
void pwmWrite(Pin pin, uint8_t dutyCycle);

/**
 * @brief Gets the PwmChannelMap for a given pin.
 *
 * Searches the pwmPinMap for the given pin, and returns a pointer to the
 * PwmChannelMap that corrosponds with the given pin.
 *
 * @param pin The pin to get the PwmChannelMap for.
 * 
 * @return Pointer to the PwmChannelMap for the given pin, or NULL if not found.
 */
const PwmChannelMap *getPwmMap(Pin pin);

#endif // !PWM_H
