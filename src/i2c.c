
#include "armory/i2c.h"
#include "armory/gpio.h"
#include "armory/rcc.h"

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

    // Set i2c clock for fast mode
    i2c->CR2 = 42;          // PCLK1 = 42 MHz
    i2c->CCR = (1 << 15) | (0 << 14) | 30;  // FAST=1, DUTY=1, CCR=52 -> 400kHz
    i2c->TRISE = 30;
    

    // Enable i2c peripheral
    i2c->CR1 |= I2C_CR1_PE;
}

void i2cStart(I2C_TypeDef *i2c) {
    // Set the start bit in the control register
    i2c->CR1 |= I2C_CR1_START;
    // Wait for the start bit to be set in the status register
    while(!(i2c->SR1 & I2C_SR1_SB));
}

void i2cStop(I2C_TypeDef *i2c) {
    // Set the stop bit in the control register
    i2c->CR1 |= I2C_CR1_STOP;
    // Wait until the busy bit is cleared in the status register
    while(i2c->SR2 & I2C_SR2_BUSY);
}

I2CResult i2cSendAddr(I2C_TypeDef *i2c, uint8_t addr, bool read) {
    // Set the LSB to the read bit (1 = read, 0 = write)
    uint8_t fullAddr = (addr << 1) | (read ? 1 : 0);

    // Write the full address to the i2c data register
    i2c->DR = fullAddr;

    // Wait for the address to be sent, checking timeout
    uint16_t timeout = I2C_TIMEOUT_TIME;
    while (!(i2c->SR1 & I2C_SR1_ADDR) && timeout--);
    if (!timeout) {
        return I2C_TIMEOUT;
    }    

    // Clear the ADDR flag by reading the status register
    (void)i2c->SR2;

    // Check for NACK
    if (i2c->SR1 & I2C_SR1_AF) {
        i2c->SR1 &= ~I2C_SR1_AF;
        return I2C_NACK;
    }

    return I2C_OK;
}

I2CResult i2cSendData(I2C_TypeDef *i2c, uint8_t data) {
    // Write the data to the i2c data register
    i2c->DR = data;

    // Wait for the data to be sent, checking timeout
    uint16_t timeout = I2C_TIMEOUT_TIME;
    while (!(i2c->SR1 & I2C_SR1_BTF) && timeout--);
    if(!timeout) {
        return I2C_TIMEOUT;
    }

    // Check for NACK
    if (i2c->SR1 & I2C_SR1_AF) {
        i2c->SR1 &= ~I2C_SR1_AF;
        return I2C_NACK;
    }

    return I2C_OK;
}

I2CResult i2cWriteByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t data) {
    // Start the i2c connection
    i2cStart(i2c);
    
    // Send device address to be targeted
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Send register address of device
    res = i2cSendData(i2c, regAddr);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Send data
    res = i2cSendData(i2c, data);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Stop connection
    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cWriteRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t data) {
    i2cStart(i2c);
 
    // Send device address to be targeted
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Send data
    res = i2cSendData(i2c, data);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cWriteBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data, uint16_t n) {
    if (!data || n == 0) {
        return I2C_ERROR;
    }

    i2cStart(i2c);

    // Send device address to be targeted
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // For each byte in data array
    for(int i = 0; i < n; i++) {
        // Send data
        res = i2cSendData(i2c, data[i]);
        if(res != I2C_OK) {
            i2cStop(i2c);
            return res;
        }
    }

    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cReadByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *data) {
    i2cStart(i2c);

    // Send device address
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Send register address
    res = i2cSendData(i2c, regAddr);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }
    
    // Restart by sending start and stop
    i2cStop(i2c);
    i2cStart(i2c);

    // Re send device address with read signal
    res = i2cSendAddr(i2c, devAddr, 1);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Read data byte from data register
    *data = (uint8_t)i2c->DR;

    i2cStop(i2c);
    return I2C_OK;
}

I2CResult i2cReadRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data) {
    i2cStart(i2c);

    // Send device address with read flag
    I2CResult res = i2cSendAddr(i2c, devAddr, 1);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Read data byte from i2c data register
    *data = (uint8_t)i2c->DR;

    i2cStop(i2c);
    return I2C_OK;
}

uint8_t i2cReceiveData(I2C_TypeDef *i2c, bool ack) {
    // Set ACK/NACK bit
    if(ack) {
        i2c->CR1 |= I2C_CR1_ACK;
    } else {
        i2c->CR1 &= ~I2C_CR1_ACK;
    }

    // Wait until the data register is  not empty
    while (!(i2c->SR1 & I2C_SR1_RXNE));

    // Return the data register
    return (uint8_t) i2c->DR;
}

I2CResult i2cReadBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t len) {
    i2cStart(i2c);

    // Send device address with write bit
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Send register address 
    res = i2cSendData(i2c, regAddr);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Restart i2c connection
    i2cStop(i2c);
    i2cStart(i2c);

    // Send device address with read flag
    res = i2cSendAddr(i2c, devAddr, 1);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Receive bytes
    for(uint16_t i = 0; i < len; i++) {
        if(i == len-1) {
            // Last byte, send NACK
            buffer[i] = i2cReceiveData(i2c, false);
        } else {
            // Not last byte, send ACK
            buffer[i] = i2cReceiveData(i2c, true);
        }
    }

    // Stop the i2c connection
    i2cStop(i2c);

    return I2C_OK;
}

I2CResult i2cReadRawBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *buffer, uint16_t len) {
    i2cStart(i2c);

    // Send the device address with the read flag
    I2CResult res = i2cSendAddr(i2c, devAddr, 1);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

    // Receive bytes
    for(uint16_t i = 0; i < len; i++) {
        if(i == len-1) {
            // Last byte, send NACK
            buffer[i] = i2cReceiveData(i2c, false);
        } else {
            // Not last byte, send ACK
            buffer[i] = i2cReceiveData(i2c, true);
        }
    }

    // Stop the i2c connection
    i2cStop(i2c);

    return I2C_OK;
}
