
#include "i2c.h"
#include "gpio.h"

const I2CMap *getI2CMap(I2C_TypeDef *i2c) {
    for(int i = 0; i < sizeof(i2cPinMap) / sizeof(I2CMap); i++) {
        if(i2c == i2cPinMap[i].instance) {
            return &i2cPinMap[i];
        }
    }

    return NULL;
}

void i2cInit(I2C_TypeDef *i2c) {
    // Enalbe given I2C in RCC
    if(i2c == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    } else if(i2c == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    } else if(i2c == I2C3) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
    } else {
        // Return if invalid i2c
        return;
    }

    const I2CMap *map = getI2CMap(i2c);

    // Initialize SCL pin 
    gpioInit(map->sclPin.port);
    gpioPinMode(map->sclPin, ALTERNATE_FUNC);
    gpioSetAlternateFunction(map->sclPin, map->sclAf);
    gpioSetOutputType(map->sclPin, OPEN_DRAIN);

    // Initialize SDA pin
    gpioInit(map->sdaPin.port);
    gpioPinMode(map->sdaPin, ALTERNATE_FUNC);
    gpioSetAlternateFunction(map->sdaPin, map->sdaAf);
    gpioSetOutputType(map->sdaPin, OPEN_DRAIN);

    // Reset i2c register
    i2c->CR1 = I2C_CR1_SWRST;
    i2c->CR1 = 0;

    // Set i2c clock
    i2c->CR2 |= (10 << 0); // Hard code to 10 MHz for now
    i2c->CCR |= (50 << 0); // Set i2c Clock to 100 kHz
    i2c->TRISE |= (11<<0); 

    // Enable i2c peripheral
    i2c->CR1 |= I2C_CR1_PE;
}

void i2cStart(I2C_TypeDef *i2c) {
    i2c->CR1 |= I2C_CR1_START;
    while(!(i2c->SR1 & I2C_SR1_SB));
}

void i2cStop(I2C_TypeDef *i2c) {
    i2c->CR1 |= I2C_CR1_STOP;
    while(i2c->SR2 & I2C_SR2_BUSY);
}

I2CResult i2cWriteByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    // Start the i2c connection
    i2cStart(i2c);
    
    // Send device address to be targeted
    i2c->DR = devAddr;
    uint32_t timeout = I2C_TIMEOUT_TIME;
    while(!(i2c->SR1 & I2C_SR1_ADDR) && timeout--);
    (void)i2c->SR2; // Clear address reg
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    if (i2c->SR1 & I2C_SR1_AF) { // NACK received, return
        i2c->SR1 &= ~I2C_SR1_AF; // clear flag
        i2cStop(i2c);
        return I2C_NACK; 
    }
    
    // Send register address of device
    i2c->DR = regAddr;
    timeout = I2C_TIMEOUT_TIME;
    while (!(i2c->SR1 & I2C_SR1_BTF) && timeout--);
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    // Send data
    i2c->DR = data;
    timeout = I2C_TIMEOUT_TIME;
    while (!(i2c->SR1 & I2C_SR1_BTF) && timeout--);
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    // Stop connection
    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cWriteRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t data) {
    i2cStart(i2c);
 
    // Send device address to be targeted
    i2c->DR = devAddr;
    uint32_t timeout = I2C_TIMEOUT_TIME;
    while(!(i2c->SR1 & I2C_SR1_ADDR) && timeout--);
    (void)i2c->SR2; // Clear address reg
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    if (i2c->SR1 & I2C_SR1_AF) { // NACK received, return
        i2c->SR1 &= ~I2C_SR1_AF; // clear flag
        i2cStop(i2c);
        return I2C_NACK; 
    }

    // Send data
    i2c->DR = data;
    timeout = I2C_TIMEOUT_TIME;
    while(!(i2c->SR1 & I2C_SR1_BTF) && timeout--);
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cWriteBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data, uint16_t n) {
    i2cStart(i2c);

    // Send device address to be targeted
    i2c->DR = devAddr;
    uint32_t timeout = I2C_TIMEOUT;
    while(!(i2c->SR1 & I2C_SR1_ADDR) && timeout--);
    (void)i2c->SR2; // Clear address reg
    if(!timeout) {
        return I2C_TIMEOUT;
    }
    
    if (i2c->SR1 & I2C_SR1_AF) { // NACK received, return
        i2c->SR1 &= ~I2C_SR1_AF; // clear flag
        i2cStop(i2c);
        return I2C_NACK; 
    }

    for(int i = 0; i < n; i++) {
        // Send data
        i2c->DR = data[i];
        int timeout = I2C_TIMEOUT_TIME;
        while(!(i2c->SR1 & I2C_SR1_BTF) && timeout--);
        if(!timeout) {
            return I2C_TIMEOUT;
        }
    }

    i2cStop(i2c);

    return I2C_OK;
}

