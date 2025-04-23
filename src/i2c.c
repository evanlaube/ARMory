
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

I2CResult i2cSendAddr(I2C_TypeDef *i2c, uint8_t addr, bool read) {
    uint8_t fullAddr = (addr << 1) | (read ? 1 : 0);
    i2c->DR = fullAddr;

    uint16_t timeout = I2C_TIMEOUT_TIME;
    while (!(i2c->SR1 & I2C_SR1_ADDR) && timeout--);
    if (!timeout) {
        return I2C_TIMEOUT;
    }    

    (void)i2c->SR2;

    // Check for NACK
    if (i2c->SR1 & I2C_SR1_AF) {
        i2c->SR1 &= ~I2C_SR1_AF;
        return I2C_NACK;
    }

    return I2C_OK;
}

I2CResult i2cSendData(I2C_TypeDef *i2c, uint8_t data) {
    i2c->DR = data;
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
    i2cStart(i2c);

    // Send device address to be targeted
    I2CResult res = i2cSendAddr(i2c, devAddr, 0);
    if(res != I2C_OK) {
        i2cStop(i2c);
        return res;
    }

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

    *data = (uint8_t)i2c->DR;

    i2cStop(i2c);
    return I2C_OK;
}

I2CResult i2cReadBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t len) {
    // Todo
}

I2CResult i2cReadRawBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *buffer, uint16_t len) {
    // Todo
}
