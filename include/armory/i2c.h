
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

#define I2C_TIMEOUT_TIME 10000

#define I2C_CR1_PE    ( 1 <<  0 )
#define I2C_CR1_START ( 1 <<  8 )
#define I2C_CR1_STOP  ( 1 <<  9 )
#define I2C_CR1_SWRST ( 1 << 15 )

#define I2C_SR1_SB    ( 1 <<  0 )
#define I2C_SR1_ADDR  ( 1 <<  1 )
#define I2C_SR1_BTF   ( 1 <<  2 )
#define I2C_SR1_AF    ( 1 << 10 )

#define I2C_SR2_BUSY  ( 1 <<  1 )

typedef struct {
    volatile uint32_t CR1;       // 0x00: Control register 1
    volatile uint32_t CR2;       // 0x04: Control register 2
    volatile uint32_t OAR1;      // 0x08: Own address register 1
    volatile uint32_t OAR2;      // 0x0C: Own address register 2
    volatile uint32_t DR;        // 0x10: Data register
    volatile uint32_t SR1;       // 0x14: Status register 1
    volatile uint32_t SR2;       // 0x18: Status register 2
    volatile uint32_t CCR;       // 0x1C: Clock control register
    volatile uint32_t TRISE;     // 0x20: TRISE register
    volatile uint32_t FLTR;      // 0x24: Filter register
} I2C_TypeDef;

#define I2C1 ((I2C_TypeDef *)0x40005400)
#define I2C2 ((I2C_TypeDef *)0x40005800)
#define I2C3 ((I2C_TypeDef *)0x40005C00)

typedef struct {
    I2C_TypeDef *instance;

    Pin sclPin;
    AlternateFunction sclAf;

    Pin sdaPin;
    AlternateFunction sdaAf;
} I2CMap;

// I2C pin mappings:
// I2C1:
//  SCL->PB6
//  SDA->PB7
// I2C2:
//  SCL->B10
//  SDA->B11
//
//  I2C3 remains unmapped for now
//
//  NOTE: These can be mapped to multiple different pins. There were
//  arbitrarily chosen and are not set in stone
static const I2CMap i2cPinMap[] = {
    { I2C1, B6,  AF4, B7,  AF4 },
    { I2C2, B10, AF4, B11, AF4 }
    //{ I2C3,  }
};

typedef enum {
    I2C_OK,
    I2C_TIMEOUT,
    I2C_NACK,
    I2C_ERROR
} I2CResult;

const I2CMap *getI2CMap(I2C_TypeDef *i2c);

void i2cInit(I2C_TypeDef *i2c);
void i2cStart(I2C_TypeDef *i2c);
void i2cStop(I2C_TypeDef *i2c);

I2CResult i2cSendAddr(I2C_TypeDef *i2c, uint8_t addr, bool read);
I2CResult i2cSendData(I2C_TypeDef *i2c, uint8_t data);

I2CResult i2cWriteByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t data);
I2CResult i2cWriteRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t data);
I2CResult i2cWriteBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data, uint16_t n);

I2CResult i2cReadByte(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *data);
I2CResult i2cReadRaw(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *data);
I2CResult i2cReadBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint16_t len);
I2CResult i2cReadRawBytes(I2C_TypeDef *i2c, uint8_t devAddr, uint8_t *buffer, uint16_t len);


#endif // !I2C_H
